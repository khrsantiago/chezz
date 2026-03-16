#include "Archer.hpp"

const std::vector<Position> Archer::directions = {Position(0, -1), Position(0, 2),Position(1, -2), Position(2, -1), Position(-1, -2), Position(-2, -1)};

sf::Color Archer::white = sf::Color(200,200,250,255);
sf::Color Archer::black = sf::Color(100,100,150,255);

Archer::Archer(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Archer, team)) 
{
    height = 76;
    set_team(team);
    set_piece_type(PieceType::Archer);
}

void Archer::update(float dt)
{
    
}

void Archer::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Color Archer::get_color(bool _team)
{
    return (_team)? Archer::white : Archer::black;
}


std::vector<BoardObjectPtr> Archer::set_valid_moves(const std::list<BoardObjectPtr>& elements, Position current)
{
    valid_moves.clear();

    std::vector<Position> relative_moves;
    int8_t mirror = (team) ? 1 : -1;

    for (size_t i = 0; i < directions.size(); i++)
    {
        relative_moves.push_back(Position(current.x + directions.at(i).x, current.y + (directions.at(i).y * mirror)));
    }

    BoardObjectPtr retrieve_move = std::make_shared<InBoardObject>(relative_moves.at(1));

    bool free_front = true;
    for (auto element : elements)
    {
        if (relative_moves.at(0) == element->pos ||
            relative_moves.at(0).x < 0 || relative_moves.at(0).x > 5 ||
            relative_moves.at(0).y < 0 || relative_moves.at(0).y > 5) free_front = false;

        if (element->pos == retrieve_move->pos) retrieve_move = element;
    
        for (size_t i = 2; i < 6; i++)
        {
            if (relative_moves.at(i) == element->pos && this->team != element->piece->get_team()) valid_moves.push_back(element);
        }
    }

    if (free_front) valid_moves.push_back(std::make_shared<InBoardObject>(relative_moves.at(0), nullptr));
    if (retrieve_move->piece)
    {
        if ((retrieve_move->piece->get_team() != this->team))
        {
            valid_moves.push_back(retrieve_move);   
        }
    }
    else if (retrieve_move->pos.x >= 0 && retrieve_move->pos.x <= 5 && retrieve_move->pos.y >= 0 && retrieve_move->pos.y <= 5)
    {
        valid_moves.push_back(retrieve_move);   
    }
    
    
    return valid_moves;
}

bool Archer::hurt(PiecePtr attacker)
{
    return true;
}

int Archer::get_material_value() const
{
    return 6;
}

int Archer::get_max_mobility() const
{
    return 2;
}
PiecePtr Archer::clone_piece() const
{
    return std::make_shared<Archer>(*this);
}
