#pragma once
#include "IGameState.hpp"
#include "Stats.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class StateStats : public IGameState
{
private:
    sf::Font font;
    sf::Text* title_text;
    sf::Text* back_btn_text; 

    std::vector<Stats> loaded_stats;

    float scroll_y;       
    float total_height;   
    float row_height;     

    void draw_row(sf::RenderWindow& window, const Stats& stat, int index, float y_pos);

public:
    StateStats(sf::RenderWindow* _window);
    ~StateStats();

    void init() override;
    void terminate() override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void on_resize() override;
};