#include "InputHandler.h"
#include "Player.h"

GameInput::GameInput(Game* pGame, Player* pPlayer) :
    m_pGame(pGame), m_pPlayer(pPlayer)
{

    
}

GameInput::~GameInput()
{
    
}

void GameInput::update(float deltaTime)
{
    if (m_inputData.hasInputs())
    {
        m_pPlayer->handleInput(m_inputData, deltaTime);
    }
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Escape : m_inputData.m_exit        = true; break;
        case sf::Keyboard::Up     : m_inputData.m_movingUp    = true; break;
        case sf::Keyboard::Down   : m_inputData.m_movingDown  = true; break;
        case sf::Keyboard::Left   : m_inputData.m_movingLeft  = true; break;
        case sf::Keyboard::Right  : m_inputData.m_movingRight = true; break;
        default: break;
    };
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Escape : m_inputData.m_exit        = false; break;
        case sf::Keyboard::Up     : m_inputData.m_movingUp    = false; break;
        case sf::Keyboard::Down   : m_inputData.m_movingDown  = false; break;
        case sf::Keyboard::Left   : m_inputData.m_movingLeft  = false; break;
        case sf::Keyboard::Right  : m_inputData.m_movingRight = false; break;
        default: break;
    };
}
