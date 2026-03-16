#include <IPiece.hpp>

int InBoardObject::get_points_value()
{
    int points = this->piece->get_material_value() * 100;
    if (this->king) points += points * 0.3f;
    return points;
}


Position::Position(int8_t _x, int8_t _y) : x(_x), y(_y) {}

bool Position::operator==(const Position& right)
{
    return (x == right.x) && (y == right.y);
}

Position Position::operator+(Position it)
{
    return Position(x + it.x, y + it.y);
}

IPiece::IPiece() : height(0)
{}

void IPiece::set_piece_type(PieceType type)
{
    this->type = type;
}

void IPiece::set_team(bool team)
{
    this->team = team;
}

bool IPiece::get_team() const
{
    return this->team;
}

PieceType IPiece::get_piece_type() const
{
    return this->type;
}

bool IPiece::is_valid(Position pos)
{
    bool is_valid = false;
    for (auto move : valid_moves)
    {
        if (move->pos == pos)
        {
            is_valid = true;
            break;
        }
    }
    return is_valid;   
}

std::vector<BoardObjectPtr> IPiece::get_valid_moves()
{
    return valid_moves;
}

int IPiece::get_height() const
{
    return height;
}
