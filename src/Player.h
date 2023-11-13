#pragma once

#include "Rectangle.h"


struct InputData;

class Game;

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}
    
    void handleInput(InputData inputData, float deltaTime);
    void updatePhysics(float deltaTime);
    void update(float deltaTime);

    int getCoins() const { return m_coins; };
    void clearCoins() { m_coins = 0; };

    bool isDead() const { return m_state == State::DEAD; }
    void setIsDead(bool isDead) { m_state = isDead ? State::DEAD : State::AERIAL; }

private:

    enum class State
    {
        GROUNDED,
        AERIAL,
        DEAD,
    };

    sf::Vector2f    m_acceleration = {};
    sf::Vector2f    m_velocity = {};

    State   m_state = State::AERIAL;
    float   m_jumpTimer = 0.0f;
    int     m_coins = 0;

    Game*   m_pGame;
};
