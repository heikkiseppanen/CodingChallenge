#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "InputHandler.h"
#include "Player.h"
#include "resources/Resources.h"
#include "Rectangle.h"
#include "Coin.h"
#include "Door.h"

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_pDoor(std::make_unique<Door>(this)),
    m_score(0),
    m_clearedLevels(0)
{
    
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
    
}

bool Game::initialise(sf::Vector2f pitchSize)
{
    m_pClock->restart();
    std::string assetPath = Resources::getAssetPath();

    if (!m_font.loadFromFile(assetPath + "Lavigne.ttf") ||
        !m_mapSheet.loadFromFile(assetPath + "T_MapSheet_D.png") ||
        !m_characterSheet.loadFromFile(assetPath + "T_CharacterSheet_D.png")) 
    {
        std::cerr << "Unable to load asset\n";
        return false;
    }

    //Initialize shapes from TileMap
    resetLevel(MapArray1);
    return true;
}

void Game::resetLevel(const int* tileMap)
{
    m_pClock->restart();
    m_state = State::WAITING;
    m_pCoins.clear();
    m_pRectangles.clear();
    m_pPlayer->setIsDead(false);
    m_pDoor->setTriggered(false);

    const sf::Vector2f tileSize(TileSizeX, TileSizeY);

    for (int y = 0; y < GridSize; y++)
    {
        for (int x = 0; x < GridSize; x++)
        {
            int i = x + y * GridSize;

            const sf::Vector2f worldPos = sf::Vector2f(x * tileSize.x, y * tileSize.y);
            switch (tileMap[i])
            {
                case    eTile::eCoin:
                {
                    m_pCoins.emplace_back(std::make_unique<Coin>(tileSize, worldPos));
                    break;
                }
                case    eTile::eBlock:
                {
                    m_pRectangles.emplace_back(std::make_unique<Rectangle>(tileSize, worldPos));
                    break;
                }
                case    eTile::ePlayerSpawn :
                {
                    m_pPlayer->setPosition(worldPos);
                    break;
                }
                case    eTile::eDoor :
                {
                    m_pDoor->setPosition(worldPos);
                    break;
                }
                default: break;
            }
        }
    }
}

void Game::resetGameState()
{
    m_clearedLevels = 0;
    m_score = 0;
    m_pPlayer->clearCoins();
    resetLevel(MapArray1);
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (m_pClock->getElapsedTime().asSeconds() >= 3.f)
            {
                m_state = State::ACTIVE;
            }
            break;
        }
        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->updatePhysics(deltaTime);
            m_pPlayer->update(deltaTime);

            if (m_pPlayer->isDead())
            {
                resetGameState();
            }
            else if (m_pDoor->isTriggered())
            {
                ++m_clearedLevels;
                if (m_clearedLevels == LevelCount)
                {
                    resetGameState();
                }
                else
                {
                    resetLevel(MapArray2);
                }
                
            }
            break;
        }
    }
    int i = 0;
    while (i < m_pCoins.size())
    {
        if (m_pCoins[i]->getCollected())
        {
            std::swap(m_pCoins[i], m_pCoins.back());
            m_pCoins.pop_back();
            continue;
        }
        i++;
    }

    
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Desperate attempt to move drawing logic from game objects because jesus christ who does that.

    sf::Sprite sampler;

    sf::Vector2f spriteScale { TileSizeX / TileMapSize, TileSizeY / TileMapSize };

    sampler.setScale(spriteScale);

    sf::IntRect sampleArea {0, 0, TileMapSize, TileMapSize};

    // Coins
    sampleArea.left = 0;
    sampleArea.top = 2 * TileMapSize;

    sampler.setTexture(m_mapSheet);
    sampler.setTextureRect(sampleArea);

    for (auto& temp : m_pCoins)
    {
        sampler.setPosition(temp->getPosition());
        target.draw(sampler, states);
    }

    // Ground
    sampleArea.left = 0;
    sampleArea.top = 0;
    sampler.setTextureRect(sampleArea);

    for (auto& temp : m_pRectangles)
    {
        sampler.setPosition(temp->getPosition());
        target.draw(sampler, states);
    }

    // Door 
    sampleArea.left = 0;
    sampleArea.top = 3 * TileMapSize;

    sampler.setPosition(m_pDoor->getPosition());
    sampler.setTextureRect(sampleArea);

    target.draw(sampler, states);

    // Character
    sampleArea.left = 0;
    sampleArea.top = 0;

    auto playerPosition = m_pPlayer->getPosition();
    playerPosition.x -= TileSizeX * PlayerWidthRatio / 2;

    sampler.setTexture(m_characterSheet);
    sampler.setPosition(playerPosition);
    sampler.setTextureRect(sampleArea);

    target.draw(sampler, states);

    //  Draw texts.
    if (m_state == State::WAITING)
    {
        sf::Text startText;

        startText.setFont(m_font);
        startText.setString("Game Start!");
        startText.setStyle(sf::Text::Bold);
        startText.setFillColor(sf::Color::Black);
        startText.setPosition(sf::Vector2f(0.1f * ScreenWidth, 0.1 * ScreenHeight));
        target.draw(startText, states);
    }
    else
    {
        sf::Text coinText;

        coinText.setFont(m_font);
        coinText.setStyle(sf::Text::Bold);
        coinText.setFillColor(sf::Color::Black);
        coinText.setString(std::to_string(m_pPlayer->getCoins()));
        coinText.setPosition(sf::Vector2f(0.1f * ScreenWidth, 0.1 * ScreenHeight));
        target.draw(coinText, states);
    }
}


void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

std::vector<Coin*> Game::getCoins()
{
    std::vector<Coin*> pCoins;

    for (auto& temp : m_pCoins)
    {
        pCoins.push_back(temp.get());
    }
    return pCoins;
}

std::vector<Rectangle*> Game::getRectangles() const
{
    std::vector<Rectangle*> pRectangles;

    for (auto& pRectangle : m_pRectangles)
    {
        pRectangles.push_back(pRectangle.get());
    }
    return (pRectangles);
}

Door* Game::getDoor()
{
    return m_pDoor.get();
}
