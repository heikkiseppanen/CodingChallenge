
#include "Coin.h"
#include <SFML/Graphics.hpp>
#include "Rectangle.h"

Coin::Coin(sf::Vector2f size, sf::Vector2f position)
    : Rectangle(size, position)
{
}

void Coin::setCollected(bool isCollected)
{
    m_isCollected = isCollected;
}

bool Coin::getCollected()
{
    return m_isCollected;
}
