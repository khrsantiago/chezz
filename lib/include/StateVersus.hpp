#pragma once

#include <IStatePlayable.hpp>

class StateVersus : public IStatePlayable
{
protected:
    void load_instanciators() override;
    void adjust_elements() override;
    void dropped_inst() override;
    void returned_piece() override;
    void end_fight(PlayerType winner) override;
    
public:
    StateVersus(sf::RenderWindow* _window);
    ~StateVersus();

    void init() override;                // inicializar aspectos del gamestate
    void terminate() override;           // eliminar memoria reservada dinámicamente o cosas que se tengan que manejar al final de ese estado de juego.
	void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void on_resize() override;
};


