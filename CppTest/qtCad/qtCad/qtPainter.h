#ifndef QT_PAINTER_H
#define QT_PAINTER_H

#include <QWidget>
#include <QTimer>

class TestWidget:public QWidget
{
	Q_OBJECT
public:
	TestWidget(QWidget *parent=0);
	//~TestWidget();
	void init();
protected:
	virtual void paintEvent(QPaintEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *event);
private:
	QPixmap pix;
	QMatrix matrix;
	int x,y;
	QTimer *myTimer;
};
#endif