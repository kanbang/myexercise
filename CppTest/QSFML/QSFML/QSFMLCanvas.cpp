
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "QSFMLCanvas.hpp"
#include <QEvent>

// Platform-specific headers
#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif

QSFMLCanvas::QSFMLCanvas( QWidget* Parent ) :
    QWidget( Parent )
{
    // Resize the widget
    //resize(400,400);

    // Setup some states to allow direct rendering into the widget
    setAttribute( Qt::WA_PaintOnScreen );
    setAttribute( Qt::WA_OpaquePaintEvent );
    setAttribute( Qt::WA_NoSystemBackground );

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy( Qt::StrongFocus );
}

QSFMLCanvas::~QSFMLCanvas()
{
    // Nothing to do...
}

void QSFMLCanvas::onInit()
{
    bool ret = texture.loadFromFile( "sfml.png" );

    // Setup the sprite
    mySprite.setTexture( texture );
    //mySprite.setTextureRect(sf::IntRect(100,100,70,50));
    //mySprite.setColor(sf::Color(255, 255, 255, 200));
    mySprite.setPosition( mySprite.getTextureRect().width / 2, mySprite.getTextureRect().height / 2 );
}

void QSFMLCanvas::onUpdate()
{
    sf::Event event;
    while ( pollEvent( event ) )
    {
        // Stick the sprite to the mouse cursor
        if ( event.type == sf::Event::MouseMoved )
        {
            mySprite.setPosition( mapPixelToCoords( sf::Vector2i( event.mouseMove.x, event.mouseMove.y ) ) );
            //mySprite.setPosition(event.mouseMove.x, event.mouseMove.y);
        }

        // Adjust the size of the default view when the widget is resized
        if ( event.type == sf::Event::Resized )
        {
            sf::View& view = const_cast<sf::View&>( getDefaultView() );
            view.setViewport( sf::FloatRect( 0, 0, event.size.width, event.size.height ) );
        }
    }

    // Rotate the sprite
    mySprite.rotate( 1.0f / 3000 * 100.f );

    // Clear the view
    clear( sf::Color( 0, 128, 0 ) );

    // Draw it
    draw( mySprite );
}

bool QSFMLCanvas::event( QEvent* Event )
{
	//在widget被构造出来之后(构造函数已经执行完成了),在即将显示窗口之前(show)
	//触发polish消息
    if ( Event->type() == QEvent::Polish )
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
#ifdef Q_WS_X11
        XFlush( QX11Info::display() );
#endif

        // Create the SFML window with the widget handle
		//因为RenderWindow::create函数需要传入一个窗口句柄(window handle)
		//如果我们在构造函数中执行这个函数里的代码，由于widget还没有被构造完成
		//此时的窗口时没有句柄的，因此在widget的构造函数里winId()返回的句柄是无效的
		//所以，这个polish消息是比较好的初始化sfml窗口的位置
        sf::RenderWindow::create( winId() );

        onInit();
        myTimer = new QTimer( this );
        // Setup the timer to trigger a refresh at specified framerate
        connect( myTimer, SIGNAL( timeout() ), this, SLOT( update() ) );
        myTimer->start();
    }

    return QWidget::event( Event );
}


////////////////////////////////////////////////////////////
/// Called when the widget needs to be painted ;
/// we use it to display a new frame
////////////////////////////////////////////////////////////
void QSFMLCanvas::paintEvent( QPaintEvent* )
{
    // Let the derived class do its specific stuff
    onUpdate();

    // Display on screen
    display();
}
