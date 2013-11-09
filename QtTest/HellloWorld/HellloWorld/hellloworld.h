#ifndef HELLLOWORLD_H
#define HELLLOWORLD_H

#include <QtGui>

class HellloWorld : public QMainWindow
{
	Q_OBJECT

public:
	HellloWorld(QWidget *parent = 0, Qt::WFlags flags = 0);
	~HellloWorld();


public slots:
	void slotBtnClick();

protected:
	virtual void paintEvent(QPaintEvent* e);

	void testDrawPath();
	void testDrawPath2();
	void testDrawLine();
	void testFillRect();
	void testDrawCircle();

	void test22();
private:
	QPushButton* a;
};

#endif // HELLLOWORLD_H
