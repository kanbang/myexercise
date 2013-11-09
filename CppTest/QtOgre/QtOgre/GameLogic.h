#ifndef QTOGRE_GAMELOGIC_H_
#define QTOGRE_GAMELOGIC_H_

#include "EventHandler.h"

namespace QtOgre
{
	class Application;

	class GameLogic : public EventHandler
	{
		friend class Application; //So the application can set itself.
	public:
		GameLogic(void);
		
	public:
		virtual void initialise(void);
		virtual void update(void);
		virtual void shutdown(void);

		virtual void onKeyPress(QKeyEvent* event);
		virtual void onKeyRelease(QKeyEvent* event);

		virtual void onMousePress(QMouseEvent* event);
		virtual void onMouseRelease(QMouseEvent* event);
		virtual void onMouseDoubleClick(QMouseEvent* event);
		virtual void onMouseMove(QMouseEvent* event);

		virtual void onWheel(QWheelEvent* event);

	protected:
		Application* mApplication;
	};
}

#endif /*QTOGRE_GAMELOGIC_H_*/