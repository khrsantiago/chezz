#pragma once
#include "IGameState.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
class StateTutorial : public IGameState
{
private:
    sf::Texture background_tex;
    sf::Sprite background_sprite;
    sf::RenderWindow* window;
    std::vector<sf::Texture> slides_tutorial;
    sf::Sprite current_sprite;
    int current_indx;

    sf::Texture texNext, texPrev, texExit;
    sf::Sprite btn_next;
    sf::Sprite btn_prev;
    sf::Sprite btn_exit;

    std::vector<sf::CircleShape> progress_dots;

    float input_cooldown;

    void setup_layout(); 
    void update_dots();  

public:
    StateTutorial(sf::RenderWindow* Window);
    ~StateTutorial();

    void init() override;                
    void terminate() override;           
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    
    bool is_clicked(sf::Sprite& sprite, sf::Vector2i mousePos);
    void on_resize() override;
};