#include "PieceInstantiator.hpp"

PieceInstantiator::PieceInstantiator(PieceType _type, bool _team, sf::Vector2f position) : IGameObject(SpriteManager::get_type_texture(_type, _team)), type{_type}, team{_team}
{
    this->set_sprite_position(position);
    originalposition = position;
}

void PieceInstantiator::update(float dt)
{
    
}

PieceType PieceInstantiator::get_type()
{
    return this->type;
}

bool PieceInstantiator::get_team()
{
    return this->team;
}

void PieceInstantiator::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void PieceInstantiator::return_to_origin()
{
    this->set_sprite_position(originalposition);
}

BoardObjectPtr make_board_object(PieceType type, bool team, int startX, int startY)
{
    switch(type)
    {
        case PieceType::Pawn :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Pawn>(team));
        case PieceType::Horse :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Horse>(team));
        case PieceType::Bishop :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Bishop>(team));
        case PieceType::Tower :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Tower>(team));
        case PieceType::Queen :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Queen>(team));
        case PieceType::Trapper :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Trapper>(team));
        case PieceType::Crook :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Crook>(team));
        case PieceType::Archer :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Archer>(team));
        case PieceType::Portal :
            return std::make_shared<InBoardObject>(Position(startX, startY), std::make_shared<Portal>(team));
        default :
            return nullptr;
    }
}

BoardObjectPtr PieceInstantiator::make_piece(int startx, int starty)
{
    return make_board_object(this->type, this->team, startx, starty);
}
