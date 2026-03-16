#include"Portal.hpp"


sf::Color Portal::white = sf::Color(230,230,150,255);
sf::Color Portal::black = sf::Color(150,150,230,255);

sf::Color Portal::get_color(bool _team)
{
return (_team)? Portal::white : Portal::black;
}

Portal::Portal(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Portal, team))
{
    height = 84;
    set_team(team);
    set_piece_type(PieceType::Portal);
}

void Portal::update(float dt)
{
    
}

void Portal::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Portal::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current) 
{
    valid_moves.erase(valid_moves.begin(), valid_moves.end());
    for (auto object : pieces)
    {
        if (object->piece->get_team() == team && object->piece->get_piece_type() != PieceType::Portal)
        {
            valid_moves.push_back(object);
        }
    }
    
    return valid_moves;    
}

bool Portal::hurt(PiecePtr attacker)
{
    return true;
}

int Portal::get_material_value() const
{
    return 3;
}

int Portal::get_max_mobility() const
{
    return 0;
}

PiecePtr Portal::clone_piece() const
{
    return std::make_shared<Portal>(*this);
}
