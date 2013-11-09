#include "hellloworld.h"
#include <QVector>

HellloWorld::HellloWorld( QWidget* parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
{
    //a = new QPushButton("test", this);
    //connect(a, SIGNAL(clicked()), this, SLOT(slotBtnClick()));

    QPalette palette;
    palette.setColor( this->backgroundRole(), Qt::black );
    this->setPalette( palette );

    test22();
}

HellloWorld::~HellloWorld()
{

}

static void ShowMsg( const QString& msg )
{
    QMessageBox msgBox;
    msgBox.setText( msg );
    msgBox.exec();
}

#define RS_EPLISON 0.000001
static bool IsFloatEqual( double a, double b )
{
    return abs( a - b ) < RS_EPLISON;
}

// 坐标系：屏幕坐标(左上角为原点，向右x轴正方向，向下y轴正方向)
static QRectF BuildValidRect( QPointF v1, QPointF v2 )
{
    if( IsFloatEqual( v1.x(), v2.x() ) )
    {
        v1 += QPointF( -1, 0 );
    }

    if( IsFloatEqual( v1.y(), v2.y() ) )
    {
        v1 += QPointF( 0, 1 );
    }

    return QRectF( v1, v2 ).normalized();
}

void HellloWorld::slotBtnClick()
{
    QPainterPath pp;
    pp.moveTo( 110, 450 );
    pp.lineTo( 290, 270 );

    QRectF rect = BuildValidRect( QPointF( 173, 367 ), QPointF( 96, 274 ) );
    //QRectF rect = BuildValidRect(QPointF(173,274),QPointF(173,367));
    //QRectF rect = BuildValidRect(QPointF(173,367),QPointF(96,274));
    //QRectF rect = BuildValidRect(QPointF(173,367),QPointF(96,274));

    if( rect.isValid() )
    {
        QPainterPath sp;
        sp.addRect( rect );
        bool ret = sp.intersects( pp );
        QPainterPath p = sp.intersected( pp );
        ShowMsg( ret ? "intersects" : "not intersects" );
    }
    else
    {
        ShowMsg( "error" );
    }
}

void HellloWorld::paintEvent( QPaintEvent* e )
{
    //testDrawPath();
    testDrawPath2();
    //testDrawLine();
    //testFillRect();
    //testDrawCircle();
}

void HellloWorld::testDrawPath()
{
    QPainter painter( this );
    QPen pen;
    pen.setCapStyle( Qt::RoundCap );
    pen.setJoinStyle( Qt::RoundJoin );
    pen.setWidth( 0 );
    pen.setColor( Qt::red );
    painter.setPen( pen );
    painter.drawLine( 30, 80, 290, 270 );

    //QRectF rect(290, 270, 80, 80);
    //painter.drawEllipse(290, 270, 80, 80);
    //painter.drawEllipse(rect);
    //painter.drawRect(rect);

    //QPainterPath pp;
    //pp.addEllipse(rect);
    //pp.moveTo(30, 80);
    //pp.lineTo(290, 270);
    //painter.drawRect(pp.boundingRect());
    //painter.drawRect(pp.controlPointRect());

    //QPolygonF polygon = pp.toFillPolygon();
    //painter.drawPolygon(polygon);
    //int c = polygon.count();
}

void HellloWorld::testDrawPath2()
{
    QPainterPathStroker ps;
    //ps.setCapStyle( Qt::RoundCap );
    //ps.setJoinStyle( Qt::RoundJoin );
    ps.setWidth( 50 );

    QPainterPath pp;
    pp.moveTo( 30, 80 );
    pp.lineTo( 290, 270 );
    pp = ps.createStroke( pp );

    QPainter painter( this );
    painter.setPen( Qt::red );
    //painter.setBrush(QBrush(Qt::white, Qt::Dense5Pattern));

    //painter.setPen(pen);
    //painter.drawPath(pp);
    painter.drawPolygon( pp.toFillPolygon() );

    painter.setPen( Qt::black );
    painter.drawLine( QPoint( 30, 80 ), QPoint( 290, 270 ) );
}

void HellloWorld::testDrawCircle()
{
    QPainter painter( this );
    painter.setPen( Qt::black );
    painter.drawEllipse( QPoint( 150, 150 ), 50, 50 );

    QPainterPath pp;
    pp.addEllipse( QPoint( 150, 150 ), 50, 50 );
    painter.setPen( Qt::white );

    QPainterPathStroker ps;
    ps.setCapStyle( Qt::RoundCap );
    ps.setJoinStyle( Qt::RoundJoin );
    ////ps.setWidth( /*pen.getScreenWidth() +*/ 2 ); // 实现线型加粗效果
    /*QPainterPath*/
    pp = ps.createStroke( pp );

    QPolygonF polygon = pp.toFillPolygon();
    painter.drawPolygon( polygon );
    //painter.drawPath(pp);
}

void HellloWorld::testDrawLine()
{
    QPainter painter( this );

    QPen pen;  // creates a default pen
    //pen.setStyle(Qt::DotLine);
    pen.setWidthF( 3 );
    pen.setCapStyle( Qt::RoundCap );
    pen.setJoinStyle( Qt::RoundJoin );

    QVector<qreal> aa = pen.dashPattern();

    QBrush brush( Qt::red );
    brush.setStyle( Qt::Dense4Pattern );
    pen.setBrush( brush );
    painter.setPen( pen );
    painter.setBrush( Qt::NoBrush );

    painter.drawLine( 10, 10, 10, 700 );
    painter.drawLine( 10, 700, 800, 700 );
    painter.drawLine( 800, 700, 800, 10 );
    painter.drawLine( 800, 10, 10, 10 );
    painter.drawEllipse( 200, 300, 150, 100 );

    //pen.setColor(Qt::black);

    //QVector<qreal> dashes;
    //qreal space = 2;
    //qreal dash = 2;
    //dashes << dash << space;
    //pen.setDashPattern(dashes);
    ////pen.setStyle(Qt::DotLine);
    pen.setWidthF( 1 );
    painter.setPen( pen );
    painter.drawLine( 20, 20, 30, 300 );

    pen.setWidthF( 0.8 );
    painter.setPen( pen );
    painter.drawLine( 30, 30, 30, 300 );

    pen.setWidthF( 1.2 );
    painter.setPen( pen );
    painter.drawLine( 40, 40, 30, 300 );

    pen.setWidthF( 1.8 );
    painter.setPen( pen );
    painter.drawLine( 50, 50, 30, 300 );

    //painter.drawEllipse(200, 300, 150, 100);
    //painter.drawText(100, 200, 60, 100,  Qt::AlignCenter | Qt::TextDontClip, "Qt");

    /*
    QRadialGradient gradient(50, 50, 50, 50, 50);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
    pen.setBrush(QBrush(gradient));
    */

}

void HellloWorld::testFillRect()
{
    QRectF rectangle( 40, 20.0, 150, 210 );

    QPainter painter( this );
    painter.setPen( Qt::blue );

    //QBrush brush(Qt::red);
    //painter.setBrush(brush);
    painter.drawRect( rectangle );
}

#include <QDataStream>
#include <QBuffer>

void f1( QByteArray& byteArray )
{
    QBuffer ib( &byteArray );
    ib.open( QIODevice::WriteOnly );
    QDataStream in( &ib );

    bool a = true;
    int i = 10;
    double d = 12.8;
    QString s = "hello world";
    in << a << i << d << s;
}

void f2( QByteArray& byteArray )
{
    QBuffer ob( &byteArray );
    ob.open( QIODevice::ReadOnly );
    QDataStream out( &ob );

    bool aa;
    int ii;
    double dd;
    QString ss;
    out >> aa >> ii >> dd >> ss;
}

void f11( QByteArray& byteArray )
{
    QBuffer ib( &byteArray );
    ib.open( QIODevice::WriteOnly );
    QDataStream in( &ib );

    bool a = true;
    int i = 10;
    double d = 12.8;
    QString s = "hello world";
    double dd = 11.4;
    in << a << i << d << s << dd;
}

void HellloWorld::test22()
{
    QByteArray byteArray/*(1024, '0')*/;

    int s = byteArray.size();
    int c = byteArray.capacity();

    f1( byteArray );

    /*int*/
    s = byteArray.size();
    /*int*/
    c = byteArray.capacity();

    f2( byteArray );

    f11( byteArray );
    /*int*/
    s = byteArray.size();
    /*int*/
    c = byteArray.capacity();
}