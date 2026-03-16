#pragma once
#include <SFML/Graphics.hpp>
struct Button
{
    sf::Sprite btn_sprite;
    sf::Text btn_text;
    Button(const sf::Texture& texture, const sf::Font& font)
        : btn_sprite(texture), btn_text(font)
    {
    }
};
enum class StateType
{
    MainMenu,
    Gameplay,
    Practice,
    Versus,
    Options,
    Credits,
    Gameover,
    Tutorial,
    Shop,
    Stats,
    Return,
    None
};

class IGameState
{
protected:
    sf::RenderWindow* window;
public:

    StateType type;
    StateType go_to;
    virtual void init() = 0;                // inicializar aspectos del gamestate
    virtual void terminate() = 0;           // eliminar memoria reservada dinámicamente o cosas que se tengan que manejar al final de ese estado de juego.
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
    sf::Vector2i get_relative_mouse_position();
    virtual void on_resize() = 0;
    bool is_mouse_over(const sf::Text& text, const sf::Vector2i& mouse_pos);
    bool is_mouse_over(const sf::Sprite& sprite, const sf::Vector2i& mouse_pos);
    void setup_button(Button* button, const std::string& str,float x_pos, float y_pos,const sf::Font& font, const sf::Texture& button_texture);

};
using GameStatePtr = std::shared_ptr<IGameState>;

