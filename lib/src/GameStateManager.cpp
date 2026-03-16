#include <GameStateManager.hpp>

GameStateManager::GameStateManager(sf::RenderWindow& _window) : window(&_window)
{

}
GameStateManager::~GameStateManager()
{

}

void GameStateManager::go_back()
{    
    states.top()->terminate();
    states.pop();
    this->on_resize();
    transition.leave(30);
}

void GameStateManager::go_to(GameStatePtr new_state)
{
    states.push(new_state);
    new_state->init();
    transition.leave(30);
}

void GameStateManager::init()
{
    if (states.empty())
    {
        states.push(std::make_shared<StateMainMenu>(window));
        states.top()->init();
    }
}   

void GameStateManager::update(float dt)
{
    if (states.empty()) return;
    StateType next_state = states.top()->go_to;
    
    
    if (next_state != StateType::None && transition.is_out())
    {
        transition.enter(30);
    }

    if (next_state != StateType::None && transition.is_staying())
    {
        StateType next_state = states.top()->go_to;
        states.top()->go_to = StateType::None;
        switch(next_state)
        {
            case StateType::MainMenu:
                {
                    states.pop();
                    states.push(std::make_shared<StateMainMenu>(this->window));
                    states.top()->init();
                }       
                break;
            case StateType::Gameplay:
                {
                    go_to(std::make_shared<StateGameplay>(this->window));
                }    
                break;
            case StateType::Stats:
            {
                go_to(std::make_shared<StateStats>(this->window)); 
            }
            break;
            case StateType::Tutorial:
                {
                    go_to(std::make_shared<StateTutorial>(this->window)); 
                }
            break;
            case StateType::Practice:
                {
                    go_to(std::make_shared<StatePractice>(this->window)); 
                }
            break;
            case StateType::Versus:
                {
                    go_to(std::make_shared<StateVersus>(this->window)); 
                }
            break;
            case StateType::Return:
                {
                    go_back();
                }
                break;
        
            default:
                break;
        }
    }
    
    if (!states.empty() && transition.is_out()) 
    {
        states.top()->update(dt);
    }

    transition.update(dt);
}

void GameStateManager::render(sf::RenderWindow& _window)
{
    states.top()->render(_window);
    transition.render(_window);
}

void GameStateManager::terminate()
{
    
}

void GameStateManager::on_resize()
{
    states.top()->on_resize();
}

