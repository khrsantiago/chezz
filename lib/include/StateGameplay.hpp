#pragma once

#include <IStatePlayable.hpp>

using PieceInstantPtr = std::shared_ptr<PieceInstantiator>;
using json = nlohmann::json;

class StateGameplay : public IStatePlayable
{
protected:
    sf::Texture background_texture; 
    sf::Sprite background_sprite;
    
    std::list<PieceType> inventory;
    PieceType enemy_king; //Este sera la pieza que mantendra el rey enemigo
    unsigned int difficulty;

    bool set_up_black_team();
    void load_instanciators() override;
    void adjust_elements() override;
    void dropped_inst() override;
    void returned_piece() override;
    void end_fight(PlayerType winner) override;
    //void handle_event(const sf::Event& event) override;
    void new_game();
    
public:
    StateGameplay(sf::RenderWindow* _window);
    ~StateGameplay();

    void init() override;                // inicializar aspectos del gamestate
    void terminate() override;           // eliminar memoria reservada dinámicamente o cosas que se tengan que manejar al final de ese estado de juego.
	void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void on_resize() override;
    
};


