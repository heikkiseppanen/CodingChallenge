#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Rectangle.h"

class Coin : public Rectangle
{
public:
    Coin(sf::Vector2f size, sf::Vector2f position);
    virtual ~Coin() {}

    void setCollected(bool isCollected);
    bool getCollected();

private:

    float   m_radius;
    
    bool    m_isCollected = false;
};
