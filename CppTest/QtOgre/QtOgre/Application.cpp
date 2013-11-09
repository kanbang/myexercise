#include "Application.h"

#include "EventHandlingOgreWidget.h"
#include "FPSDialog.h"
#include "GameLogic.h"
#include "GraphicsSettingsWidget.h"
#include "LogManager.h"
#include "SettingsDialog.h"

#include <OgreRenderSystem.h>
#include <OgreRoot.h>

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QTimer>

//Q_INIT_RESOURCE cannot be called from within a namespace, so we provide
//this function. See the Q_INIT_RESOURCE documentation for an explanation.
inline void initQtResources()
{
    Q_INIT_RESOURCE( resources );
}

namespace QtOgre
{
    void qtMessageHandler( QtMsgType type, const char* msg )
    {
        switch ( type )
        {
        case QtDebugMsg:
            qApp->_systemLog()->logMessage( msg, LL_DEBUG );
            break;
        case QtWarningMsg:
            qApp->_systemLog()->logMessage( msg, LL_WARNING );
            break;
        case QtCriticalMsg:
            qApp->_systemLog()->logMessage( msg, LL_ERROR );
            break;
            //     case QtFatalMsg:
            ////We don't override this one as we are dying and wouldn't see it.
            //         fprintf(stderr, "Fatal: %s\n", msg);
            //         abort();
        }
    }

    Application::Application( int& argc, char** argv, GameLogic* gameLogic )
        : QApplication( argc, argv )
        , mOpenGLRenderSystem( 0 )
        , mDirect3D9RenderSystem( 0 )
        , mFrameCounter( 0 )
    {
        mGameLogic = gameLogic;
        if( mGameLogic != 0 )
        {
            mGameLogic->mApplication = this;
        }

        //Initialise the resources.
        initQtResources();

        //Load the settings file. If it doesn't exist it is created.
        mSettings = new QSettings( "settings.ini", QSettings::IniFormat );

        mOgreWidget = new EventHandlingOgreWidget( 0, 0 );

        //Logging should be initialised ASAP, and before Ogre::Root is created.
        initialiseLogging();

        //Create the root and load render system plugins. We do that here so that we know
        //what render systems are available by the time we show the settings dialog.
        //Note that the render system is not initialised until the user selects one.
        mRoot = new Ogre::Root();
        loadRenderSystemsFromPlugins();

        mSettingsDialog = new SettingsDialog( mSettings, mOgreWidget );
        mGraphicsSettingsWidget = new GraphicsSettingsWidget;
        mSettingsDialog->addSettingsWidget( "Graphics", mGraphicsSettingsWidget );
    }

    Application::~Application()
    {
        if ( mRoot )
            delete mRoot;
    }

    bool Application::isOpenGLAvailable( void )
    {
        return mOpenGLRenderSystem != 0;
    }

    bool Application::isDirect3D9Available( void )
    {
        return mDirect3D9RenderSystem != 0;
    }

    /*LogManager* Application::logManager(void)
    {
    	return mLogManager;
    }*/

    unsigned int Application::frameCount( void )
    {
        return mFrameCounter;
    }

    /**This function is an implementation detail, and should not really be exposed.
    It return the log which the QtOgre framework uses for its messages, whereas
    users are expected to instead create their own log with createLog(). The reason
    it is exposed is that the Qt debugging system (qtMessageHandler()) also redirects
    to this log, and that cannot be made a member function.*/
    Log* Application::_systemLog( void )
    {
        return mSystemLog;
    }

    QWidget* Application::mainWidget( void )
    {
        return mOgreWidget;
    }

    void Application::applySettings( void )
    {
        //Eventually Application settings might be applied here.
        //Until then, we just pass the settings on the the MainWindow and GameLogic
        if( !mOgreWidget->applySettings( mSettingsDialog->mSettings ) )
        {
            showWarningMessageBox( "Unable to apply desired settings to the window.\nPlease consult the log for details" );
        }

        /*if(mGameLogic != 0)
        {
        	mGameLogic->applySettings(mSettings);
        }*/
    }

    void Application::initialise( void )
    {

        mOgreWidget->show();
        mOgreWidget->resize( 800, 600 );
        centerWidget( mOgreWidget );

        //mLogManagerDockWidget = mMainWindow->addAsDockWidget(mLogManager, "Logs", Qt::AllDockWidgetAreas);
        //mLogManager->setParent(mMainWindow);
        centerWidget( mLogManager, mOgreWidget );

        mLogManager->show();

        //mLogManager->setForceProcessEvents(true);
        initialiseOgre();
        Ogre::NameValuePairList ogreWindowParams;
        ogreWindowParams["FSAA"] = "8";
        mOgreWidget->initialise( &ogreWindowParams );
        mGameLogic->initialise();
        //mLogManager->setForceProcessEvents(false);

        //mLogManager->hide();




        mOgreWidget->setEventHandler( mGameLogic );

        //This is a bit of a hack, necessary because we want to use the settings dialog in two different
        //ways. The first time it is shown (by Application::exec()) no slot is connected - the Accepted
        //event is handled explicitly because the system is not initialised at that point. But now (and
        //when the dialog is shown in future) we are ready for it, so we connect the accepted() signal.
        //We also call accept, to do the initial setup. See also Application::exec().
        connect( mSettingsDialog, SIGNAL( accepted() ), this, SLOT( applySettings() ) );
        mSettingsDialog->accept();

        mFPSDialog = new FPSDialog( mOgreWidget, Qt::FramelessWindowHint );
        mFPSDialog->setWindowOpacity( settings()->value( "System/DefaultWindowOpacity", 1.0 ).toDouble() );
        mFPSDialog->move( mainWidget()->geometry().topLeft() + QPoint( 10, 10 ) );
        mFPSDialog->show();

        mLogManager->move( mainWidget()->geometry().left() + 10, mainWidget()->geometry().top() + mainWidget()->geometry().height() - mLogManager->frameGeometry().height() - 10 );

        mUpdateTimer = new QTimer;
        QObject::connect( mUpdateTimer, SIGNAL( timeout() ), this, SLOT( update() ) );
        //On the test system, a value of one here gives a high frame rate and still allows
        //event prcessing to take place. A value of 0 doubles the frame rate but the mouse
        //becomes jumpy. This property should probably be configurable.
        mUpdateTimer->start( 1 );
    }

    void Application::update( void )
    {
        mGameLogic->update();
        mOgreWidget->update();
        ++mFrameCounter;
    }

    void Application::shutdown( void )
    {
        mUpdateTimer->stop();
        mGameLogic->shutdown();
        this->exit( 0 );
    }

    void Application::initialiseLogging( void )
    {
        //Initialise our logging system
        mLogManager = new LogManager( mOgreWidget );
        mLogManager->resize( 550, 400 );
        mLogManager->setWindowOpacity( settings()->value( "System/DefaultWindowOpacity", 1.0 ).toDouble() );

        //Redirect Qt's logging system to our own
        mSystemLog = mLogManager->createLog( "System" );
        //qInstallMsgHandler(&qtMessageHandler);
        //qDebug("Debug test");
        //qWarning("Warning test");
        //qCritical("Critical test");

        //Redirect Ogre's logging system to our own
        //This has to all be done before creating the Root object.
        mOgreLog = mLogManager->createLog( "Ogre" );
        mLogManager->setVisibleLog( mOgreLog );
        mInternalOgreLogManager = new Ogre::LogManager();
        mInternalOgreLog = mInternalOgreLogManager->createLog( "Ogre.log", false, true, true );
        mInternalOgreLog->addListener( this );
    }

    void Application::loadRenderSystemsFromPlugins( void )
    {
#if defined(Q_WS_WIN)
        try
        {
#if defined(_DEBUG)
            mRoot->loadPlugin( "RenderSystem_GL_d" );
#else
            mRoot->loadPlugin( "RenderSystem_GL" );
#endif
        }
        catch( ... )
        {
            qWarning( "Failed to load OpenGL plugin" );
        }
        try
        {
#if defined(_DEBUG)
            mRoot->loadPlugin( "RenderSystem_Direct3D9_d" );
#else
            mRoot->loadPlugin( "RenderSystem_Direct3D9" );
#endif
        }
        catch( ... )
        {
            qWarning( "Failed to load Direct3D9 plugin" );
        }
#endif
#if defined(Q_WS_X11)
        try
        {
            mRoot->loadPlugin( "/usr/local/lib/OGRE/RenderSystem_GL" );
        }
        catch( ... )
        {
            qWarning( "Failed to load OpenGL plugin" );
        }
#endif

        Ogre::RenderSystemList::const_iterator itr = Ogre::Root::getSingletonPtr()->getAvailableRenderers().begin();
        while ( itr != Ogre::Root::getSingletonPtr()->getAvailableRenderers().end() )
        {
            if ( ( *itr )->getName() == "OpenGL Rendering Subsystem" )
            {
                mOpenGLRenderSystem = *itr;
            }
            if ( ( *itr )->getName() == "Direct3D9 Rendering Subsystem" )
            {
                mDirect3D9RenderSystem = *itr;
            }
            itr++;
        }

        if( !( mOpenGLRenderSystem || mDirect3D9RenderSystem ) )
        {
            qCritical( "No rendering subsystems found" );
        }
    }

    void Application::initialiseOgre( void )
    {
        QString renderSystem = mSettingsDialog->mSettings->value( "Graphics/RenderSystem" ).toString();
        if( renderSystem.compare( "OpenGL Rendering Subsystem" ) == 0 )
        {
            mActiveRenderSystem = mOpenGLRenderSystem;
        }
        if( renderSystem.compare( "Direct3D9 Rendering Subsystem" ) == 0 )
        {
            mActiveRenderSystem = mDirect3D9RenderSystem;
        }

        Ogre::Root::getSingletonPtr()->setRenderSystem( mActiveRenderSystem );

        Ogre::Root::getSingletonPtr()->initialise( false );
    }

    void Application::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage )
    {
        //Convert message to Qt's string type.
        QString messageAsQString = QString::fromStdString( message );

        //Map Ogre's LogMessageLevels to our LogLevels
        switch( lml )
        {
        case Ogre::LML_TRIVIAL:
            mOgreLog->logMessage( messageAsQString, LL_DEBUG );
            break;
        case Ogre::LML_NORMAL:
            mOgreLog->logMessage( messageAsQString, LL_INFO );
            break;
        default: //Should be Ogre::LML_CRITICAL
            mOgreLog->logMessage( messageAsQString, LL_ERROR );
        }
    }

    /*SettingsDialog* Application::settingsDialog(void)
    {
    	return mSettingsDialog;
    }*/

    int Application::exec( bool displaySettingsDialog )
    {
        //If we don't show the setting dialog, or we do show it and it is accepted, then proceed.
        if( ( displaySettingsDialog == false ) || ( qApp->showSettingsDialog() == QDialog::Accepted ) )
        {
            qApp->initialise();
            return QApplication::exec();
        }
        //Otherwise the user cancelled so exit.
        else
        {
            return 0;
        }
    }

    /*Centers a widget inside its parent, or inside the desktop if no
    parent is provided. This requires the widget to be free to move.
    If not (e.g. its in a layout) then the behaviour is undefined*/
    void Application::centerWidget( QWidget* widgetToCenter, QWidget* parent )
    {
        QRect parentGeometry;
        if( parent != 0 )
        {
            parentGeometry = parent->frameGeometry();
        }
        else
        {
            parentGeometry = desktop()->availableGeometry();
        }

        int xOffset = ( parentGeometry.width() - widgetToCenter->frameGeometry().width() ) / 2;
        int yOffset = ( parentGeometry.height() - widgetToCenter->frameGeometry().height() ) / 2;
        widgetToCenter->move( parentGeometry.x() + xOffset, parentGeometry.y() + yOffset );
    }

    void Application::showInfoMessageBox( const QString& text )
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle( "Information" );
        msgBox.setIconPixmap( QPixmap( ":/images/icons/dialog-information.svg" ) );
        msgBox.setStandardButtons( QMessageBox::Ok );
        msgBox.setText( text );
        msgBox.exec();
    }

    void Application::showWarningMessageBox( const QString& text )
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle( "Warning" );
        msgBox.setIconPixmap( QPixmap( ":/images/icons/dialog-warning.svg" ) );
        msgBox.setStandardButtons( QMessageBox::Ok );
        msgBox.setText( text );
        msgBox.exec();
    }

    void Application::showErrorMessageBox( const QString& text )
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle( "Error" );
        msgBox.setIconPixmap( QPixmap( ":/images/icons/dialog-error.svg" ) );
        msgBox.setStandardButtons( QMessageBox::Ok );
        msgBox.setText( text );
        msgBox.exec();
    }

    void Application::hideSettingsDialog( void )
    {
        mSettingsDialog->reject();
    }

    int Application::showSettingsDialog( void )
    {
        return mSettingsDialog->exec();
    }

    Log* Application::createLog( const QString& name )
    {
        return mLogManager->createLog( name );
    }

    void Application::hideLogManager( void )
    {
        mLogManager->setVisible( false );
    }

    void Application::showLogManager( void )
    {
        mLogManager->setVisible( true );
    }

    Ogre::RenderWindow* Application::ogreRenderWindow( void )
    {
        return mOgreWidget->getOgreRenderWindow();
    }

    QSettings* Application::settings( void )
    {
        return mSettings;
    }
}