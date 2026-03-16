#include"Pawn.hpp"

sf::Color Pawn::white = sf::Color(200,200,200,255);
sf::Color Pawn::black = sf::Color(100,100,100,255);

sf::Color Pawn::get_color(bool _team)
{
return (_team)? Pawn::white : Pawn::black;
}

Pawn::Pawn(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Pawn, team))
{
    height = 72;
    set_team(team);
    set_piece_type(PieceType::Pawn);
}

void Pawn::update(float dt)
{
    
}

void Pawn::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Pawn::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current) 
{
    valid_moves.clear();
    int8_t mirror = (team) ? -1 : 1;

    BoardObjectPtr front = std::make_shared<InBoardObject>(Position(current.x, current.y + mirror));
    BoardObjectPtr left = std::make_shared<InBoardObject>(Position(current.x + mirror, current.y + mirror));
    BoardObjectPtr right = std::make_shared<InBoardObject>(Position(current.x - mirror, current.y + mirror));

    for (auto object : pieces)
    {
        if(object->pos == front->pos)
        {
            front = object;
        }
        else if (object->pos == left->pos && object->piece->get_team() != this->get_team())
        {
            left = object;
        }
        else if (object->pos == right->pos && object->piece->get_team() != this->get_team())
        {
            right = object;
        }
    }
    
    if (!front->piece && front->pos.x <= 5 && front->pos.y <= 5 && front->pos.x >= 0 && front->pos.y >= 0)
    {
        valid_moves.push_back(front);
    }

    if (left->piece)   valid_moves.push_back(left);

    if (right->piece)  valid_moves.push_back(right);
    
    return valid_moves;    
}

bool Pawn::hurt(PiecePtr attacker)
{
    return true;
}

int Pawn::get_material_value() const
{
    return 1;
}


int Pawn::get_max_mobility() const
{
    return 1;
}

PiecePtr Pawn::clone_piece() const
{
    return std::make_shared<Pawn>(*this);
}
