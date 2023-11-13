#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Game.h"
#include <memory>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Platformer");
    window.setKeyRepeatEnabled(false);

    std::cout.sync_with_stdio(false);
    
    std::unique_ptr<Game> pGame = std::make_unique<Game>();
    if (!pGame->initialise(window.getView().getSize()))
    {
        std::cerr << "Game Failed to initialise\n";
        return 1;
    }
    
    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    pGame->onKeyPressed(event.key.code);
                    break;
                }
                case sf::Event::KeyReleased:
                {
                    pGame->onKeyReleased(event.key.code);
                    break;
                }
                default: break;
            }
        }
        
        sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();

        pGame->update(elapsedTime.asSeconds());
        
        // clear the window with black color
        window.clear(sf::Color::White);
        
        window.draw(*pGame.get());
        
        // end the current frame
        window.display();
    }
    
    return 0;
}
