#pragma once
#include <IGameState.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include"Button.hpp"

class StateMainMenu : public IGameState
{
private:
    float time_elapsed = 0.0f; 
    const float INPUT_COOLDOWN = 0.5f;   
   
    sf::Font font;
    sf::Texture button_texture;
    sf::Text* text_title = nullptr;
    
    Button* btn_practice = nullptr;
    Button* btn_versus = nullptr;
    Button* btn_play = nullptr;
    Button* btn_tutorial = nullptr;
    Button* btn_exit = nullptr;
    Button* btn_stats = nullptr;
    // por si tengo que borrara
    bool mouse_released = false;


public:
    StateMainMenu(sf::RenderWindow* _window); 
    ~StateMainMenu();
    sf::Texture background_tex;
    sf::Sprite background_sprite;
    void init() override;
    void terminate() override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void on_resize() override;
};