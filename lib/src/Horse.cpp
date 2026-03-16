#include"Horse.hpp"

const std::vector<Position> Horse::directions = {Position(0, -2), Position(1, -1), Position(2, 0), Position(1, 1), 
                                                    Position(0, 2), Position(-1, 1), Position(-2, 0), Position(-1, -1)};

sf::Color Horse::white = sf::Color(150,230,150,255);
sf::Color Horse::black = sf::Color(50,130,50,255);
 
sf::Color Horse::get_color(bool _team)
{
return (_team)? Horse::white : Horse::black;
}

Horse::Horse(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Horse, team))
{
    height = 80;
    set_team(team);
    set_piece_type(PieceType::Horse);
}

void Horse::update(float dt)
{
    
}

void Horse::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Horse::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current) 
{
    valid_moves.clear();
    const uint8_t lenght = Board::side_lenght;

    for (auto direction : directions)
    {
        int new_x = current.x + direction.x;
        int new_y = current.y + direction.y;

        if (new_x < 0 || new_x >= Board::side_lenght || new_y < 0 || new_y >= Board::side_lenght)
        {
            continue;
        }

        auto target = std::make_shared<InBoardObject>(Position((int8_t)new_x, (int8_t)new_y));

        for (auto object : pieces)
        {
            if (object->pos == target->pos)
            {
                target = object;
                break;
            }
        }
        
        if (target->piece)
        {
            if(target->piece->get_team() != this->team)
            {
                valid_moves.push_back(target);
            }
        }
        else
        {
            valid_moves.push_back(target);
        }
        
    }
    return valid_moves;    
}

bool Horse::hurt(PiecePtr attacker)
{
    return true;
}


int Horse::get_material_value() const
{
    return 5;
}


int Horse::get_max_mobility() const
{
    return 8;
}

PiecePtr Horse::clone_piece() const
{
    return std::make_shared<Horse>(*this);
}
