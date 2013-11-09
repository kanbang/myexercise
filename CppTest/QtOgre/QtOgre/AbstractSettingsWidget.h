#ifndef QTOGRE_ABSTRACTSETTINGSWIDGET_H_
#define QTOGRE_ABSTRACTSETTINGSWIDGET_H_

#include <QSettings>
#include <QWidget>

namespace QtOgre
{
	class AbstractSettingsWidget : public QWidget
	{
		Q_OBJECT

	public:
		AbstractSettingsWidget(QWidget *parent = 0);

		void setSettings(QSettings* settings);

		virtual void disableFirstTimeOnlySettings(void) = 0;

	public slots:
		void dialogFinished(int result);
		virtual void readFromSettings(void) = 0;
		virtual void writeToSettings(void) = 0;


	protected:
		QSettings* mSettings;
	};
}

#endif /*QTOGRE_ABSTRACTSETTINGSWIDGET_H_*/