#ifndef QSFMLCANVAS_HPP
#define QSFMLCANVAS_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>

class QEvent;

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
public :
    QSFMLCanvas(QWidget* Parent = NULL);
    virtual ~QSFMLCanvas();

public:
    virtual void onInit();
    virtual void onUpdate();
	virtual QPaintEngine* paintEngine() const { return 0; }

protected:
    virtual bool event(QEvent* Event);
    virtual void paintEvent(QPaintEvent*);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    QTimer* myTimer; ///< Timer used to update the view
	sf::Texture texture;
	sf::Sprite mySprite; ///< A sprite to display the image
};


#endif // QSFMLCANVAS_HPP
