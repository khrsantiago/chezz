#include "StateMainMenu.hpp"

StateMainMenu::StateMainMenu(sf::RenderWindow* _window)
    : background_sprite(background_tex), text_title(nullptr), btn_play(nullptr), btn_tutorial(nullptr), btn_exit(nullptr), btn_versus(nullptr), btn_practice(nullptr)
{
    window = _window;
    type = StateType::MainMenu;
    go_to = StateType::None;
    mouse_released = false;
}

StateMainMenu::~StateMainMenu()
{
    terminate();
}

void StateMainMenu::terminate()
{

    delete text_title;
    text_title = nullptr;

    delete btn_stats;
    btn_stats = nullptr;

    delete btn_play;
    btn_play = nullptr;

    delete btn_tutorial;
    btn_tutorial = nullptr;

    delete btn_exit;
    btn_exit = nullptr;

    delete btn_practice;
    btn_practice = nullptr;

    delete btn_versus;
    btn_versus = nullptr;
}

void StateMainMenu::init()
{
    if (!font.openFromFile("assets/fonts/arial.ttf")) 
    {
        std::cerr << "ERROR: No se pudo cargar la fuente (arial.ttf)" << std::endl;

    }
    if(!background_tex.loadFromFile("assets/background_menu.png"))
    {
        std::cerr << "ERROR: No se pudo cargar el fondo" << std::endl;
    }
    auto size = background_tex.getSize();
    background_sprite.setTexture(background_tex);
    background_sprite.setTextureRect({{0, 0}, {static_cast<int>(size.x), static_cast<int>(size.y)}});
    if (!button_texture.loadFromFile("assets/button.png")) 
    {
        sf::Image img;
        img.resize(sf::Vector2u(200, 50), sf::Color::White);
        
        if (!button_texture.loadFromImage(img))
        {
             std::cerr << "FATAL: No se pudo crear textura de respaldo." << std::endl;
        }
        std::cerr << "WARNING: assets/button.png no encontrado, usando textura blanca." << std::endl;
    }
    button_texture.setSmooth(true); 

    text_title = new sf::Text(font, "CHESS 3.33", 60);
    text_title->setOutlineColor(sf::Color::Black);
    text_title->setOutlineThickness(3);
    btn_stats = new Button(button_texture, font);
    btn_play = new Button(button_texture, font);
    btn_tutorial = new Button(button_texture, font);
    btn_exit = new Button(button_texture, font);
    btn_versus = new Button(button_texture,font);
    btn_practice = new Button(button_texture,font);
    text_title->setFillColor(sf::Color(89,220,219));
    text_title->setStyle(sf::Text::Bold);
    
    on_resize();
}

void StateMainMenu::on_resize()
{
    sf::Vector2u win_size = window->getSize();
    float midX = win_size.x / 2.0f;

    sf::FloatRect bounds = text_title->getLocalBounds();
    text_title->setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    text_title->setPosition({midX, win_size.y * 0.2f});

    sf::Vector2u original_size = background_tex.getSize();
    float scale_x = (float)win_size.x / original_size.x;
    float scale_y = (float)win_size.y / original_size.y;
    background_sprite.setScale(sf::Vector2f(scale_x, scale_y));
    background_sprite.setOrigin(sf::Vector2f(0.0f, 0.0f)); 
    background_sprite.setPosition(sf::Vector2f(0.0f, 0.0f));
    auto offset = btn_exit->btn_sprite.getTexture().getSize().x;
    setup_button(btn_play,     "JUGAR", win_size.x / 2.0, win_size.y * 0.40f, font, btn_play->btn_sprite.getTexture());
    setup_button(btn_stats,    "PUNTAJES", win_size.x / 2.0, win_size.y * 0.55f, font, btn_stats->btn_sprite.getTexture());
    setup_button(btn_tutorial, "TUTORIAL", win_size.x / 2.0, win_size.y * 0.70f, font, btn_tutorial->btn_sprite.getTexture());
    setup_button(btn_exit,     "SALIR", win_size.x / 2.0, win_size.y * 0.85f, font, btn_exit->btn_sprite.getTexture());
    setup_button(btn_versus,"PVP",  (win_size.x / 2.0) + 2 * offset, win_size.y * 0.85, font, btn_versus->btn_sprite.getTexture());
    setup_button(btn_practice,"PRACTICE", (win_size.x / 2.0)  - 2 * offset, win_size.y * 0.85, font, btn_practice->btn_sprite.getTexture());

    btn_play->btn_text.setOutlineThickness(2);
    btn_play->btn_text.setOutlineColor(sf::Color::Black);
    btn_stats->btn_text.setOutlineThickness(2);
    btn_stats->btn_text.setOutlineColor(sf::Color::Black);
    btn_tutorial->btn_text.setOutlineThickness(2);
    btn_tutorial->btn_text.setOutlineColor(sf::Color::Black);
    btn_exit->btn_text.setOutlineThickness(2);
    btn_exit->btn_text.setOutlineColor(sf::Color::Black);
    btn_practice->btn_text.setOutlineThickness(2);
    btn_practice->btn_text.setOutlineColor(sf::Color::Black);
    btn_versus->btn_text.setOutlineThickness(2);
    btn_versus->btn_text.setOutlineColor(sf::Color::Black);
}

void StateMainMenu::update(float dt)
{
     time_elapsed += dt;
    sf::Vector2i mouse_pos = get_relative_mouse_position();
   auto update_button_color = [&](Button* btn) {
        if (is_mouse_over(btn->btn_sprite, mouse_pos))
            btn->btn_sprite.setColor(sf::Color(200, 200, 200)); 
        else
            btn->btn_sprite.setColor(sf::Color::White);
    };
    update_button_color(btn_stats);
    update_button_color(btn_play);
    update_button_color(btn_tutorial);
    update_button_color(btn_exit);
    update_button_color(btn_practice);
    update_button_color(btn_versus);

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        mouse_released = true;
    }
    if (time_elapsed < INPUT_COOLDOWN) 
    {
        return; 
    }
    if (mouse_released && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (is_mouse_over(btn_play->btn_sprite, mouse_pos))
        {
            this->go_to = StateType::Gameplay;
        }
        else if (is_mouse_over(btn_stats->btn_sprite, mouse_pos))
        {
            this->go_to = StateType::Stats;
        }
        else if (is_mouse_over(btn_tutorial->btn_sprite, mouse_pos))
        {
            this->go_to = StateType::Tutorial;
        }
        else if (is_mouse_over(btn_practice->btn_sprite, mouse_pos))
        {
            this->go_to = StateType::Practice;
        }
        else if (is_mouse_over(btn_versus->btn_sprite, mouse_pos))
        {
            this->go_to = StateType::Versus;
        }
        else if (is_mouse_over(btn_exit->btn_sprite, mouse_pos) && time_elapsed > INPUT_COOLDOWN)
        {
            window->close(); 
        }
    }
}

void StateMainMenu::render(sf::RenderWindow& target)
{
    target.draw(background_sprite);
    target.draw(*text_title);
    target.draw(btn_play->btn_sprite);
    target.draw(btn_play->btn_text); 
    target.draw(btn_stats->btn_sprite);
    target.draw(btn_stats->btn_text);
    target.draw(btn_tutorial->btn_sprite);
    target.draw(btn_tutorial->btn_text);
    target.draw(btn_exit->btn_sprite);
    target.draw(btn_exit->btn_text);
    target.draw(btn_practice->btn_sprite);
    target.draw(btn_practice->btn_text);
    target.draw(btn_versus->btn_sprite);
    target.draw(btn_versus->btn_text);
}
