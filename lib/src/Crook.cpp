#include "Crook.hpp"

const std::vector<Position> Crook::directions = {Position(0, 1), Position(1, -1), Position(-1, -1)};

sf::Color Crook::white = sf::Color(150,200,230,225);
sf::Color Crook::black = sf::Color(50,100,130,225);

sf::Color Crook::get_color(bool _team)
{
return (_team)? Crook::white : Crook::black;
}

Crook::Crook(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Crook, team))
{
    height = 76;
    set_team(team);
    set_piece_type(PieceType::Crook);
}

void Crook::update(float dt)
{
    
}

void Crook::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Crook::set_valid_moves(const std::list<BoardObjectPtr>& elements, Position current)
{
    valid_moves.clear();

    int8_t mirror = (team) ? 1 : -1;

    Position advance(current.x + directions.at(0).x, current.y + (directions.at(0).y * mirror));
    Position right_diagonal(current.x + directions.at(1).x, current.y + (directions.at(1).y * mirror));
    Position left_diagonal(current.x + directions.at(2).x, current.y + (directions.at(2).y * mirror));

    BoardObjectPtr front_piece = std::make_shared<InBoardObject>(advance);
    BoardObjectPtr diag_right_piece = std::make_shared<InBoardObject>(right_diagonal);
    BoardObjectPtr diag_left_piece = std::make_shared<InBoardObject>(left_diagonal);

    for (auto element : elements)
    {
        if (element->pos == advance)
        {
            front_piece = element;
        }
        else if (element->pos == left_diagonal)
        {
            diag_left_piece = element;
        }
        else if (element->pos == right_diagonal)
        {
            diag_right_piece = element;
        }
    }

    if (!front_piece->piece && advance.x <= 5 && advance.y <= 5 && advance.x >= 0 && advance.y >= 0)
    {
        valid_moves.push_back(std::make_shared<InBoardObject>(advance));
    }

    if (left_diagonal.x <= 5 && left_diagonal.y <= 5 && left_diagonal.x >= 0 && left_diagonal.y >= 0)
    {
        if (diag_left_piece->piece == nullptr || diag_left_piece->piece->get_team() != this->get_team())
        {
            valid_moves.push_back(diag_left_piece);
        }
    }

    if (right_diagonal.x <= 5 && right_diagonal.y <= 5 && right_diagonal.x >= 0 && right_diagonal.y >= 0)
    {
        if (diag_right_piece->piece == nullptr || diag_right_piece->piece->get_team() != this->get_team())
        {
            valid_moves.push_back(diag_right_piece);
        }
    }

    return valid_moves;
}

bool Crook::hurt(PiecePtr attacker)
{
    return true;
}

int Crook::get_material_value() const
{
    return 3;
}

int Crook::get_max_mobility() const
{
    return 3;
}

PiecePtr Crook::clone_piece() const
{
    return std::make_shared<Crook>(*this);
}
