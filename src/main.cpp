#include <StateGameplay.hpp>
#include"StateTutorial.hpp"
#include <Board.hpp>
#include <GameStateManager.hpp>
#include <Stats.hpp>
int main()
{
    SpriteManager::init();
    auto window = sf::RenderWindow(sf::VideoMode({1080u, 810u}), "Chess 3.33 v1");
    window.setFramerateLimit(144u);
    
    sf::Vector2u desktopSize = sf::VideoMode::getDesktopMode().size;
    sf::Vector2u windowSize = window.getSize();
    int initial_posX = (static_cast<int>(desktopSize.x) - static_cast<int>(windowSize.x)) / 2;
    int initial_posY = (static_cast<int>(desktopSize.y) - static_cast<int>(windowSize.y)) / 2;
    window.setPosition(sf::Vector2i(initial_posX, initial_posY));

    sf::Clock clock; 
    sf::Time last_time = clock.getElapsedTime();
    sf::Time delta_time = sf::Time::Zero;
    const sf::Time target_time = sf::seconds(1)/60.f;

    const sf::Color Cerulean = sf::Color(130,130,200,255);

    GameStateManager manager(window);
    manager.init();
    SoundManager::init();
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                //Guardar partida rapidamente, tal vez aca
                window.close();
            }
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                
                sf::Vector2f newSize(resized->size);
                bool minimun = false;
                if (newSize.x <= 1080)
                {
                    newSize.x = 1080;
                    minimun = true;
                }
                if (newSize.y <= 810)
                {
                    newSize.y = 810;
                    minimun = true;
                }

                if (minimun)
                {
                    window.setSize(static_cast<sf::Vector2u>(newSize));
                }
                
                sf::FloatRect visibleArea({0.f, 0.f}, newSize);

                window.setView(sf::View(visibleArea));
                manager.on_resize();
            }
        }
        
        auto now = clock.getElapsedTime();
        delta_time += now - last_time;
        if (delta_time >= target_time)
        {
            manager.update(delta_time/target_time);
            
            delta_time = sf::Time::Zero;

            window.clear(Cerulean);
            manager.render(window);
            window.display();
        }
        last_time = now;
    }
}
