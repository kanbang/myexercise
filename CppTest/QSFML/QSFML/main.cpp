
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "QSFMLCanvas.hpp"
#include <QApplication>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
class MyWidget : public QWidget
{
public:
    MyWidget( QWidget* parent = 0 ) : QWidget( parent )
    {
        QVBoxLayout* Layout = new QVBoxLayout;
        Layout->addWidget( new QSFMLCanvas() );
        setLayout( Layout );
    }
};

int main( int argc, char** argv )
{
    QApplication App( argc, argv );

	MyWidget mw;
	mw.setWindowTitle( "Qt SFML" );
	mw.resize( 400, 400 );
	mw.show();

    return App.exec();
}
