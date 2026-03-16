#include <IGameObject.hpp>
const float CELL_LENGHT = 100.0f;
IGameObject::IGameObject(sf::Texture r_texture) : texture(r_texture), sprite((this->texture))
{
    sf::Vector2u texture_size = this->texture.getSize();
    float scale_x = CELL_LENGHT / static_cast<float>(texture_size.x);
    float scale_y = CELL_LENGHT / static_cast<float>(texture_size.y);

    this->sprite.setScale(sf::Vector2f(scale_x, scale_y));
}

sf::Sprite IGameObject::get_sprite() const
{
    return sprite;
}

void IGameObject::set_sprite_position(const sf::Vector2f& position)
{
    sprite.setPosition(position);
}

bool IGameObject::is_touching_mouse(const sf::Vector2i& mouse_position)
{
    return sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position));
}
