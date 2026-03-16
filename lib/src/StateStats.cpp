#include "StateStats.hpp"
#include <algorithm> 
#include <iostream>

StateStats::StateStats(sf::RenderWindow* _window) : title_text(nullptr), back_btn_text(nullptr)
{
    window = _window;
    type = StateType::Stats;
    go_to = StateType::None;
    scroll_y = 0.0f;
    row_height = 60.0f; 
}

StateStats::~StateStats() {}

void StateStats::init()
{
    if (!font.openFromFile("assets/fonts/arial.ttf")) 
    {
        std::cerr << "Error cargando fuente en Stats" << std::endl;
    }
    title_text = new sf::Text(font, "SCORES", 40); 
    title_text->setFillColor(sf::Color::Yellow);
    
    back_btn_text = new sf::Text(font, "<- VOLVER", 25);
    back_btn_text->setPosition(sf::Vector2f(20, 20));

    loaded_stats = Stats::load_all("stats.json");

    std::sort(loaded_stats.begin(), loaded_stats.end(), 
        [](const Stats& a, const Stats& b) {
            return a.get_score() < b.get_score();
        });

    total_height = loaded_stats.size() * row_height;
    
    on_resize();
}

void StateStats::terminate() 
{
    loaded_stats.clear();
    delete back_btn_text; 
    back_btn_text = nullptr;
    
    delete title_text; 
    title_text = nullptr;
}

void StateStats::update(float dt)
{
    sf::Vector2i mouse_pos = get_relative_mouse_position();

    if (is_mouse_over(*back_btn_text, mouse_pos)) 
    {
        back_btn_text->setFillColor(sf::Color::Green);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
        {
            go_to = StateType::Return;
        }
    } 
    else 
    {
        back_btn_text->setFillColor(sf::Color::White);
    }
   
    float scroll_speed = 500.0f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
    {
        scroll_y -= scroll_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) 
    {
        scroll_y += scroll_speed;
    }

    float max_scroll = std::max(0.0f, total_height - (float)window->getSize().y + 150.0f);
    
    if (scroll_y < 0) scroll_y = 0;
    if (scroll_y > max_scroll) scroll_y = max_scroll;
}

void StateStats::draw_row(sf::RenderWindow& window, const Stats& stat, int index, float y_pos)
{
    if (y_pos < -row_height || y_pos > window.getSize().y) return;

    sf::Text text_rank(font, std::to_string(index + 1) + ".", 20);
    sf::Text text_name(font,stat.get_name(), 20);
    sf::Text text_score(font, "Score: " + std::to_string(stat.get_score()), 20);
    
    float x_margin = window.getSize().x * 0.15f;
    float col_2 = window.getSize().x * 0.25f;
    float col_3 = window.getSize().x * 0.70f;

    text_rank.setPosition(sf::Vector2f(x_margin, y_pos));
    text_name.setPosition(sf::Vector2f(col_2, y_pos));
    text_score.setPosition(sf::Vector2f(col_3, y_pos));

    if (index == 0) 
    {
        text_name.setFillColor(sf::Color::Yellow);
        text_score.setFillColor(sf::Color::Yellow);
    }

    window.draw(text_rank);
    window.draw(text_name);
    window.draw(text_score);

    sf::RectangleShape line(sf::Vector2f(window.getSize().x * 0.8f, 1));
    line.setPosition(sf::Vector2f(x_margin, y_pos + 35));
    line.setFillColor(sf::Color(100, 100, 100));
    window.draw(line);
}

void StateStats::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(20, 20, 30));

    float start_y_offset = 120.0f; 

    for (size_t i = 0; i < loaded_stats.size(); ++i) 
    {
        float y = start_y_offset + (i * row_height) - scroll_y;
        draw_row(window, loaded_stats[i], i, y);
    }

    sf::RectangleShape header_bg(sf::Vector2f((float)window.getSize().x, 100.f));
    header_bg.setFillColor(sf::Color(20, 20, 30));
    window.draw(header_bg);

    window.draw(*title_text);
    window.draw(*back_btn_text);
}

void StateStats::on_resize()
{
    sf::FloatRect bounds = title_text->getLocalBounds();
    title_text->setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));
    title_text->setPosition(sf::Vector2f(window->getSize().x / 2.0f, 50.0f));
}