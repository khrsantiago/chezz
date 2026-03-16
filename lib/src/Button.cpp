#include <Button.hpp>

Button::Button(sf::Texture _texture) : IGameObject(_texture)
{
    
}

Button::Button(sf::Vector2f _position, sf::Vector2f _size, sf::Color _color)  : IGameObject(sf::Texture(_size))
{
    sf::RenderTexture rect(_size);
    rect.clear(_color);
}

Button(sf::texture _texture, sf::Vector2f _position) : IGameObject(_texture)
{
    this->set_sprite_position(_position);
}

bool get_state()const
{
    return this->activated;
}

bool toggle()
{
    return this->activated = !this->activated;
}

void update(float dt)
{

}

void render(sf::RenderWindow& window)
{
    window.draw(this->get_sprite());
}
