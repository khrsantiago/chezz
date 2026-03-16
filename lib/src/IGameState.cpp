#include "IGameState.hpp"

sf::Vector2i IGameState::get_relative_mouse_position()
{
    return sf::Mouse::getPosition(*window);
}

bool IGameState::is_mouse_over(const sf::Text& text, const sf::Vector2i& mouse_pos)
{
    sf::FloatRect bounds = text.getGlobalBounds();
    return bounds.contains({(float)mouse_pos.x, (float)mouse_pos.y});
}
bool IGameState::is_mouse_over(const sf::Sprite& sprite, const sf::Vector2i& mousePos)
{
    sf::Vector2f mouseF(static_cast<sf::Vector2f>(mousePos));
    return sprite.getGlobalBounds().contains(mouseF);
}
void IGameState::setup_button(Button* button, const std::string& str, float x_pos, float y_pos,const sf::Font& font, const sf::Texture& button_texture)
{
    sf::Vector2u win_size = window->getSize();

    unsigned int char_size = static_cast<unsigned int>(win_size.y * 0.036f);
    
    if (char_size < 20) char_size = 20; 

    button->btn_text.setFont(font);
    button->btn_text.setString(str);
    button->btn_text.setCharacterSize(char_size);
    button->btn_text.setFillColor(sf::Color::White); 
    
    sf::FloatRect text_bounds = button->btn_text.getLocalBounds();
    button->btn_text.setOrigin({
        text_bounds.position.x + text_bounds.size.x / 2.0f, 
        text_bounds.position.y + text_bounds.size.y / 2.0f + 8.0f
    });

    button->btn_sprite.setTexture(button_texture);
    sf::FloatRect sprite_local_bounds = button->btn_sprite.getLocalBounds();

    float target_width = static_cast<float>(win_size.x) * 0.15f;
    float target_height = static_cast<float>(win_size.y) * 0.10f;

    if (target_width < 180.0f) target_width = 180.0f;
    if (target_height < 60.0f) target_height = 60.0f;

    float scale_x = target_width / sprite_local_bounds.size.x;
    float scale_y = target_height / sprite_local_bounds.size.y;

    button->btn_sprite.setScale(sf::Vector2f(scale_x, scale_y)); 

    button->btn_sprite.setOrigin({
        sprite_local_bounds.size.x / 2.0f, 
        sprite_local_bounds.size.y / 2.0f
    });
    //float mid_x = (float)win_size.x / 2.0f;    
    
    button->btn_sprite.setPosition({x_pos, y_pos});
    float vertical_correction = - (target_height * 0.05f); 
    button->btn_text.setPosition({x_pos, y_pos + vertical_correction});
}

