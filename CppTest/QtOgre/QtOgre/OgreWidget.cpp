#include "OgreWidget.h"

#include "Application.h"
#include "EventHandler.h"
#include "Log.h"

#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreStringConverter.h>

#if defined(Q_WS_X11)
#include <QX11Info>
#endif

namespace QtOgre
{
    OgreWidget::OgreWidget( QWidget* parent, Qt::WindowFlags f )
        : QWidget( parent, f | Qt::MSWindowsOwnDC )
        , m_pOgreRenderWindow( 0 )
        , mIsInitialised( false )
    {
        QPalette palette = this->palette(); //FIXME - Doesn't compile unless we use 'this'?! Because we are in the constructor?
        palette.setColor( QPalette::Active, QPalette::WindowText, Qt::black );
        palette.setColor( QPalette::Active, QPalette::Window, Qt::black );
        setPalette( palette );
        //setUpdatesEnabled(false);
    }

    OgreWidget::~OgreWidget()
    {
    }

    void OgreWidget::initialise( const Ogre::NameValuePairList* miscParams )
    {
        //These attributes are the same as those use in a QGLWidget
        setAttribute( Qt::WA_PaintOnScreen );
        setAttribute( Qt::WA_NoBackground );
        //setAttribute(Qt::WA_NoSystemBackground);

        //Parameters to pass to Ogre::Root::createRenderWindow()
        Ogre::NameValuePairList params;

        //If the user passed in any parameters then be sure to copy them into our own parameter set.
        //NOTE: Many of the parameters the user can supply (left, top, border, etc) will be ignored
        //as they are overridden by Qt. Some are still useful (such as FSAA).
        if( miscParams != 0 )
        {
            params.insert( miscParams->begin(), miscParams->end() );
        }

        //The external windows handle parameters are platform-specific
        Ogre::String externalWindowHandleParams;

        //Accept input focus
        //setFocusPolicy(Qt::StrongFocus);
        setFocusPolicy( Qt::ClickFocus );

#if defined(Q_WS_WIN)
        //positive integer for W32 (HWND handle) - According to Ogre Docs
        externalWindowHandleParams = Ogre::StringConverter::toString( ( unsigned int )( winId() ) );
#endif

#if defined(Q_WS_X11)
        //poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*) for GLX - According to Ogre Docs
        QX11Info info = x11Info();
        externalWindowHandleParams  = Ogre::StringConverter::toString( ( unsigned long )( info.display() ) );
        externalWindowHandleParams += ":";
        externalWindowHandleParams += Ogre::StringConverter::toString( ( unsigned int )( info.screen() ) );
        externalWindowHandleParams += ":";
        externalWindowHandleParams += Ogre::StringConverter::toString( ( unsigned long )( winId() ) );
        //externalWindowHandleParams += ":";
        //externalWindowHandleParams += Ogre::StringConverter::toString((unsigned long)(info.visual()));
#endif

        //Add the external window handle parameters to the existing params set.
#if defined(Q_WS_WIN)
        params["externalWindowHandle"] = externalWindowHandleParams;
#endif

#if defined(Q_WS_X11)
        params["parentWindowHandle"] = externalWindowHandleParams;
#endif

        //Finally create our window.
        m_pOgreRenderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow( "OgreWindow", width(), height(), false, &params );

        mIsInitialised = true;

        //setAttribute( Qt::WA_OpaquePaintEvent );
    }

    Ogre::RenderWindow* OgreWidget::getOgreRenderWindow() const
    {
        return m_pOgreRenderWindow;
    }

    void OgreWidget::paintEvent( QPaintEvent* evt )
    {
        if( mIsInitialised )
        {
            //Ogre::Root::getSingleton()._fireFrameStarted();
            //m_pOgreRenderWindow->update();
            ////Ogre::Root::getSingleton()._fireFrameRenderingQueued();
            //Ogre::Root::getSingleton()._fireFrameEnded();
            Ogre::Root::getSingleton().renderOneFrame();
        }
    }

    void OgreWidget::resizeEvent( QResizeEvent* evt )
    {
        if( m_pOgreRenderWindow )
        {
            m_pOgreRenderWindow->resize( width(), height() );
            m_pOgreRenderWindow->windowMovedOrResized();

            for( int ct = 0; ct < m_pOgreRenderWindow->getNumViewports(); ++ct )
            {
                Ogre::Viewport* pViewport = m_pOgreRenderWindow->getViewport( ct );
                Ogre::Camera* pCamera = pViewport->getCamera();
                pCamera->setAspectRatio( static_cast<Ogre::Real>( pViewport->getActualWidth() ) / static_cast<Ogre::Real>( pViewport->getActualHeight() ) );
            }
        }
    }

    bool OgreWidget::applySettings( QSettings* settings )
    {
        bool applied = false;
        QString resolution;

        QStringList windowModes = settings->value( "Graphics/WindowModes" ).toStringList();
        int selectedWindowMode = settings->value( "Graphics/SelectedWindowMode", 0 ).toInt();

        if( windowModes.size() > selectedWindowMode ) //Make sure it's a valid index.
        {
            resolution = windowModes.at( selectedWindowMode );
            if( resolution.compare( "FullScreen", Qt::CaseInsensitive ) == 0 )
            {
                showFullScreen();
                applied = true;
            }
            else
            {
                QStringList splitResolution = resolution.split( "x", QString::SkipEmptyParts, Qt::CaseInsensitive );

                if( splitResolution.size() == 2 )
                {
                    bool widthValid = false;
                    int width = splitResolution.at( 0 ).toInt( &widthValid );
                    widthValid &= ( width > 100 ) && ( width < 10000 );

                    bool heightValid = false;
                    int height = splitResolution.at( 1 ).toInt( &heightValid );
                    heightValid &= ( width > 100 ) && ( width < 10000 );

                    if( widthValid && heightValid )
                    {
                        resize( width, height );
                        Application::centerWidget( this );
                        applied = true;
                    }
                }
            }
        }

        if( applied )
        {
            qApp->_systemLog()->logMessage( "Set main window resolution to \"" + resolution + "\"", LL_INFO );
        }
        else
        {
            qApp->_systemLog()->logMessage( "Failed to set main window resolution. The list of resolutions was:", LL_WARNING );
            foreach ( QString res, windowModes )
            {
                qApp->_systemLog()->logMessage( "    \"" + res + "\"", LL_WARNING );
            }
            qApp->_systemLog()->logMessage( "The requested index was: \"" + QString::number( selectedWindowMode ) + "\"", LL_WARNING );
        }
        return applied;
    }
}
