#include <ScreenTransition.hpp>

void center_sftext(sf::Text& text, sf::Vector2u win_size, int Yoffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    text.setPosition({static_cast<float>(win_size.x) / 2, (static_cast<float>(win_size.y) / 2) + Yoffset});
}

ScreenTransition::ScreenTransition() : target_frames(0), elapsed_frames(0), phase(TransitionPhase::Out)
{}

bool ScreenTransition::is_transitioning()
{
    if (phase == TransitionPhase::Enter || phase == TransitionPhase::Leave)
        return true;

    return false;
}

bool ScreenTransition::is_staying()
{
    if (phase == TransitionPhase::Stay)
        return true;

    return false;
}

bool ScreenTransition::is_out()
{
    if (phase == TransitionPhase::Out)
        return true;

    return false;
}

void ScreenTransition::enter(float frames)
{
    elapsed_frames = 0;
    target_frames = frames;
    phase = TransitionPhase::Enter;
}

void ScreenTransition::leave(float frames)
{
    elapsed_frames = 0;
    target_frames = frames;
    phase = TransitionPhase::Leave;
}

void ScreenTransition::update(float dt)
{
    if(elapsed_frames < target_frames)
    {
        elapsed_frames = (elapsed_frames + dt < target_frames)? elapsed_frames + dt : target_frames;
    }
    else
    {
        if(phase == TransitionPhase::Enter)
            phase = TransitionPhase::Stay;
        
        if(phase == TransitionPhase::Leave)
            phase = TransitionPhase::Out;
    }
}

void ScreenTransition::render(sf::RenderWindow& window)
{
    float percent = 0;

    if (is_staying())
    {
        percent = 1;
    }
    else if (is_transitioning())
    {
        percent = elapsed_frames / target_frames;
        percent = (phase == TransitionPhase::Leave)? (1 - percent) : percent; 
    }

    sf::Color col(30,30,30,(int)(255.f * percent));

    sf::RenderTexture rendertexture(window.getSize());
    rendertexture.clear(col);
    rendertexture.display();
    window.draw(sf::Sprite(rendertexture.getTexture()));
}
