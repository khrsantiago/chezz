#pragma once

#include <stack>
#include <map>
#include <memory>
#include <functional>
#include <IGameState.hpp>
#include <StateGameplay.hpp>
#include <StateMainMenu.hpp>
#include "StateStats.hpp"
#include <ScreenTransition.hpp>

#include"StateTutorial.hpp"
#include "StateVersus.hpp"
#include "StatePractice.hpp"
class GameStateManager
{
private:
    std::stack<GameStatePtr> states;
    sf::RenderWindow* window;
    ScreenTransition transition;

public:
    GameStateManager(sf::RenderWindow& _window);
    ~GameStateManager();

    void go_back();
    void go_to(GameStatePtr new_state);
    void init();       
	void update(float dt);
	void render(sf::RenderWindow& _window);
    void terminate();
    void on_resize();
};
