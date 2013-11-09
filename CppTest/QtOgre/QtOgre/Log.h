#ifndef QTOGRE_LOG_H_
#define QTOGRE_LOG_H_

#include "../ui_Log.h"

namespace QtOgre
{
	enum LogLevel
	{ 
		//These values are specifically chosen to be over 1000. This means they can be used as the 'type'
		//of a QTableWidgetItem. This lets us easily identify the message type of any given row.
		//In addition, they are powers of two in order to simplify the logic when filtering messages.
		LL_DEBUG = 1024,
		LL_INFO = 2048,
		LL_WARNING = 4096,
		LL_ERROR = 8192
	};

	class Log : public QWidget, private Ui::Log
	{
		Q_OBJECT

	public:
		Log(QWidget *parent = 0);

		void logMessage(const QString& message, LogLevel logLevel);

		//Messages which get logged do not appear in the log until the event
		//queue get processed to cause a GUI update. This normally happens once
		//per frame, but that may not be enough. It's certainly a problem during
		//initialisation, when many mesages get logged with no updates.
		//Setting this to true causes the log to force event processing every
		//time a message is logged.
		//TODO - should this be a Qt property?
		void setForceProcessEvents(bool forceProcessEvents);

	public slots:
		void on_filterLineEdit_textChanged(const QString& string);
		void on_clearFilterButton_clicked();
		void on_showDebugButton_clicked();
		void on_showInformationButton_clicked();
		void on_showWarningsButton_clicked();
		void on_showErrorsButton_clicked();

	private:
		//Private methods used for filtering and row visiblity
		void computeVisibleMessageTypes(void);
		void filterMessages(void);
		void setRowVisibility(int row);

		//This is used as a bitfield to determine what message types are enabled
		int mVisibleMessageTypes;

		//Icons for the lefthand column
		QIcon debugIcon;
		QIcon infoIcon;
		QIcon warningIcon;
		QIcon errorIcon;

		//Colours used for the log viewer
		QColor backgroundColor;
		QColor debugColor;
		QColor infoColor;
		QColor warningColor;
		QColor errorColor;

		//Controls whether we force the processing of event on log updates
		bool mForceProcessEvents;
	};
}

#endif /*QTOGRE_LOG_H_*/