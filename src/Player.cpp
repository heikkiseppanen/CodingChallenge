#include "Player.h"
#include "InputHandler.h"
#include "Constants.h"
#include "Coin.h"
#include "Game.h"
#include "Door.h"

#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

Player::Player(Game* pGame) : Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)), m_pGame(pGame)
{
}

void Player::handleInput(InputData inputData, float deltaTime)
{
    if (m_state == State::DEAD)
    {
        return;
    }

    m_acceleration.x -= PlayerAcceleration * inputData.m_movingLeft; 
    m_acceleration.x += PlayerAcceleration * inputData.m_movingRight; 
            
    if (m_state == State::AERIAL)
    {
        m_jumpTimer = std::max(m_jumpTimer - deltaTime, 0.0f);

        if (inputData.m_movingUp == true && m_jumpTimer > 0.0f && m_velocity.y < 0)
        {
            m_velocity.y = -PlayerJumpVelocity;
        }
    }
    else
    {
        if (inputData.m_movingUp == true)
        {
            m_jumpTimer = PlayerJumpTime;
            m_velocity.y = -PlayerJumpVelocity;

            m_state = State::AERIAL;
        }
    }
}

void Player::updatePhysics(float deltaTime)
{
    if (m_state == State::DEAD)
    {
        return;
    }

    if (m_acceleration.x == 0.0f && m_velocity.x != 0.0f)
    {
        m_acceleration.x += m_velocity.x < 0.0f ? PlayerDeacceleration : -PlayerDeacceleration;
    }

    m_acceleration.y = Gravity;

    m_velocity.x += m_acceleration.x * deltaTime;
    m_velocity.x = std::clamp(m_velocity.x, -PlayerMaxVelocityX, PlayerMaxVelocityX);

    m_velocity.y += m_acceleration.y * deltaTime;
    m_velocity.y = std::clamp(m_velocity.y, -PlayerMaxVelocityY, PlayerMaxVelocityY);

    if (std::fabsf(m_velocity.x) <= PlayerMinVelocity)
    {
        m_velocity.x = 0.0f;
    }

    if (std::fabsf(m_velocity.y) <= PlayerMinVelocity)
    {
        m_velocity.y = 0.0f;
    }

    m_acceleration = {};

    auto deltaVelocity = m_velocity * deltaTime;

    move(deltaVelocity.x, 0.0f);

    auto pRectangles = m_pGame->getRectangles();
    for (auto pRectangle : pRectangles)
    {
        if (pRectangle->collidesWith(this))
        {
            move(-deltaVelocity.x, 0.0f);
            m_velocity.x = 0.0f;
        }
    }

    sf::Transformable::move(0.0f, deltaVelocity.y);

    for (auto pRectangle : pRectangles)
    {
        if (pRectangle->collidesWith(this))
        {
            if (deltaVelocity.y >= 0.0f)
            {
                m_state = State::GROUNDED;
            }
            move(0.0f, -deltaVelocity.y);
            m_velocity.y = 0.0f;
        }
    }
    
    auto position = getPosition();

    if (position.y >= ScreenHeight)
    {
        setIsDead(true);
    }
}

void Player::update(float deltaTime)
{
    std::vector<Coin*> Coins = m_pGame->getCoins();
    int i = 0;

    for (auto& temp : Coins)
    {
        if (temp->collidesWith(this))
        {
            temp->setCollected(true);
            ++m_coins;
        }
    }

    if (m_pGame->getDoor()->collidesWith(this))
    {
        m_pGame->getDoor()->setTriggered(true);
    }
}
