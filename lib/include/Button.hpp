/*#include <IGameObject.hpp>

class Button : public IGameObject
{
public:
    Button() = delete;
    Button(sf::Texture);
    Button(sf::Texture, sf::Vector2f);
    Button(sf::Vector2f, sf::Vector2f, sf::Color);
    bool get_state()const;
    bool toggle();
    virtual void update(float) override;
    virtual void render(sf::RenderWindow&) override;

private:
    bool activated;
};

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
*/
