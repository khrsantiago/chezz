#include"Tower.hpp"

const std::vector<Position> Tower::directions = {Position(1, 0), Position(-1, 0), Position(0, 1), Position(0, -1)};

sf::Color Tower::white = sf::Color(100,230,230,255);
sf::Color Tower::black = sf::Color(0,130,130,255);

sf::Color Tower::get_color(bool _team)
{
return (_team)? Tower::white : Tower::black;
}

Tower::Tower(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Tower, team))
{
    height = 76;
    set_team(team);
    set_piece_type(PieceType::Tower);
}

void Tower::update(float dt)
{
    
}

void Tower::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Tower::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current) 
{
    valid_moves.erase(valid_moves.begin(), valid_moves.end());
    const uint8_t lenght = Board::side_lenght;

    for (auto direction : directions)
    {
        int delta_x = direction.x;
        int delta_y = direction.y;
        for (size_t k = 1; k <= 2; k++)
        {
            uint8_t tile_x = current.x + (k * delta_x);
            uint8_t tile_y = current.y + (k * delta_y);

            if (tile_x >= lenght || tile_y >= lenght) break;

            auto target = std::make_shared<InBoardObject>(Position(tile_x, tile_y));
            
            for(auto const& object : pieces)
            {
                if (object->pos == target->pos)
                {
                    target = object;
                    break;
                }
            }

            if (target->piece)
            {
                if (target->piece->get_team() != this->get_team())
                {
                    valid_moves.push_back(target);
                }
                break;
            }
            else
            {
                valid_moves.push_back(target);
            }
        }
        
    }
    return valid_moves;    
}

bool Tower::hurt(PiecePtr attacker)
{
    return true;
}


int Tower::get_material_value() const
{
    return 7;
}

int Tower::get_max_mobility() const
{
    return 8;
}

PiecePtr Tower::clone_piece() const
{
    return std::make_shared<Tower>(*this);
}
