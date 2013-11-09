#ifndef QTOGRE_APPLICATION_H_
#define QTOGRE_APPLICATION_H_

#include <OgreLog.h>
#include <OgrePrerequisites.h>

#include <QApplication>
#include <QSettings>

//This redefines qApp, causing it to return an Application pointer instead of a QApplication one.
//This is useful, for example, to access the logging system. This is done in the same way that
//Qt does it to get a QApplication rather than a QCoreApplication, so it should be legitimate.
#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

namespace QtOgre
{
	class EventHandlingOgreWidget;
	class FPSDialog;
	class GameLogic;
	class GraphicsSettingsWidget;
	class Log;
	class LogManager;
	class SettingsDialog;

	class Application : public QApplication, public Ogre::LogListener
	{
		Q_OBJECT

	public:
		///Creates an instance of the Application class.
		Application(int & argc, char ** argv, GameLogic* gameLogic = 0);
		//Destroys an instance of the Application class
		~Application();

		//Getters
		///Returns the number of frames rendered since the application started.
		unsigned int frameCount(void);
		///Gets a pointer to the Ogre RenderWindow to which you can add viewports.
		Ogre::RenderWindow* ogreRenderWindow(void);
		///Gets a pointer to the applications main window.
		QWidget* mainWidget(void);
		///Getsa pointer to the application settings
		QSettings* settings(void);
		///Return the log used by the QtOgre framework.
		Log* _systemLog(void);

		//Testers
		///Determine whether the OpenGL render system is available.
		bool isOpenGLAvailable(void);
		///Determine whether the Direct3D9 render system is available.
		bool isDirect3D9Available(void);
		
		//Other
		///Creates a new log with a given name.
		Log* createLog(const QString& name);
		///Hides the LogManager window.
		void hideLogManager(void);
		///Hides the settings dialog.
		void hideSettingsDialog(void);
		///Shows the LogManager window.
		void showLogManager(void);
		///Shows the settings dialog.
		int showSettingsDialog(void);

		//Static functions
		///Start the main event loop.
		//Use 'displaySettingsDialog' because there is already a function called 'showSettingsDialog'.
		static int exec(bool displaySettingsDialog = true); 

		///Utility function to center a widget.
		static void centerWidget(QWidget* widgetToCenter, QWidget* parent = 0);

		///Shows a message box with an 'Info' icon and 'Information' in the title bar.
		static void showInfoMessageBox(const QString& text);
		///Shows a message box with a 'Warning' icon and 'Warning' in the title bar.
		static void showWarningMessageBox(const QString& text);
		///Shows a message box with an 'Error' icon and 'Error' in the title bar.
		static void showErrorMessageBox(const QString& text);

	public slots:
		void applySettings(void);
		void initialise(void);
		void shutdown(void);
		void update(void);		

	private:
		//Private functions
		void initialiseLogging(void);
		void initialiseOgre(void);
		void loadRenderSystemsFromPlugins(void);
		///Implemented from Ogre::LogListener to redirect logging 
		void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName,bool& skipThisMessage);

		//Widgets
		FPSDialog* mFPSDialog;
		GraphicsSettingsWidget* mGraphicsSettingsWidget;
		EventHandlingOgreWidget* mOgreWidget;
		SettingsDialog* mSettingsDialog;

		//Logging
		Ogre::Log* mInternalOgreLog;
		Ogre::LogManager* mInternalOgreLogManager;
		LogManager* mLogManager;
		Log* mOgreLog;
		Log* mSystemLog;

		//Ogre Stuff
		Ogre::RenderSystem* mActiveRenderSystem;
		Ogre::RenderSystem* mOpenGLRenderSystem;
		Ogre::RenderSystem* mDirect3D9RenderSystem;
		Ogre::Root* mRoot;

		//Misc
		unsigned int mFrameCounter;
		GameLogic* mGameLogic;
		QTimer* mUpdateTimer;
		QSettings* mSettings;
	};
}

#endif /*QTOGRE_APPLICATION_H_*/ 