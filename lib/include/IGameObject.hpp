#pragma once
#include <SFML/Graphics.hpp>

class IGameObject
{
public:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    IGameObject(sf::Texture texture);
    IGameObject();
    sf::Sprite get_sprite() const;
    void set_sprite_position(const sf::Vector2f& position);
    bool is_touching_mouse(const sf::Vector2i& mouse_position);
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};


