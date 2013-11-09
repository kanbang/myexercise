#include "qtPainter.h"
#include <QPainter>
#include <QMouseEvent>

TestWidget::TestWidget(QWidget *parent):QWidget(parent)
{
	setMouseTracking(true);
	setMinimumSize(100, 100);

	QPalette palette;
	//palette.setColor(QPalette::Background, QColor(192,253,123));
	palette.setColor(backgroundRole(), QColor(192,253,123));
	setAutoFillBackground(true);
	setPalette(palette);

	init();

	myTimer = new QTimer( this );
	connect( myTimer, SIGNAL( timeout() ), this, SLOT( update() ) );
	myTimer->start();
}


//TestWidget::~TestWidget()
//{
//
//}

void TestWidget::paintEvent(QPaintEvent *e)
{
	static qreal d = 5;
	//matrix.rotate(0.5);
	//pix=pix.transformed(matrix,Qt::FastTransformation);

	QPainter painter(this);
	//painter.setBackground(Qt::black);

	painter.fillRect(0,0, this->width(), this->height(), Qt::transparent);
	painter.drawLine(0,0,50,50);

	painter.save();
	painter.translate(x,y);
	painter.rotate(d);
	d += 5;
	painter.drawPixmap(0,0,pix);
	painter.restore();
}

void TestWidget::init()
{
	pix.load("background.png");
	pix.scaled(pix.width()*200,pix.height()*200,Qt::KeepAspectRatio);
}		

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
	x = event->x();
	y = event->y();
	/*pix = pix.grabWidget(this,x,y,30,30);
	width = pix.width();
	height = pix.height();
	pix = pix.scaled(width * 5,height * 5,Qt::KeepAspectRatio);*/
	//update();
	
	
}