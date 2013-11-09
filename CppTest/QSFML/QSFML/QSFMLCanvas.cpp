
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
	//��widget���������֮��(���캯���Ѿ�ִ�������),�ڼ�����ʾ����֮ǰ(show)
	//����polish��Ϣ
    if ( Event->type() == QEvent::Polish )
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
#ifdef Q_WS_X11
        XFlush( QX11Info::display() );
#endif

        // Create the SFML window with the widget handle
		//��ΪRenderWindow::create������Ҫ����һ�����ھ��(window handle)
		//��������ڹ��캯����ִ�����������Ĵ��룬����widget��û�б��������
		//��ʱ�Ĵ���ʱû�о���ģ������widget�Ĺ��캯����winId()���صľ������Ч��
		//���ԣ����polish��Ϣ�ǱȽϺõĳ�ʼ��sfml���ڵ�λ��
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
