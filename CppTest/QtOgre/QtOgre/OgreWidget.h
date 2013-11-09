#ifndef QTOGRE_OGREWIDGET_H_
#define QTOGRE_OGREWIDGET_H_

#include <OgreCommon.h>
#include <OgrePrerequisites.h>

#include <QSettings>
#include <QWidget>

namespace QtOgre
{
	class InputEventHandler;

	class OgreWidget : public QWidget
	{
		Q_OBJECT

	public:
		OgreWidget(QWidget* parent=0, Qt::WindowFlags f=0);
		~OgreWidget();

		Ogre::RenderWindow* getOgreRenderWindow() const;

		//Other
		bool applySettings(QSettings* settings);
		void initialise(const Ogre::NameValuePairList *miscParams = 0);
		// 必须有这行代码,否则图形会闪烁
		QPaintEngine * paintEngine () const
		{ 
			return  0 ;
		}

	protected:
		void paintEvent(QPaintEvent* evt);
		void resizeEvent(QResizeEvent* evt);

	public:
		Ogre::RenderWindow* m_pOgreRenderWindow;

	private:
		bool mIsInitialised;
	};
}

#endif /*QTOGRE_OGREWIDGET_H_*/ 