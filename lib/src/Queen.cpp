#include"Queen.hpp"

const std::vector<Position> Queen::directions = {Position(1, 0), Position(-1, 0), Position(0, 1), Position(0, -1),
                                                    Position(1, -1), Position(1, 1), Position(-1, 1), Position(-1, -1)};

sf::Color Queen::white = sf::Color(255,130,130,255);
sf::Color Queen::black = sf::Color(155,30,30,255);

sf::Color Queen::get_color(bool _team)
{
return (_team)? Queen::white : Queen::black;
}
                                                    

Queen::Queen(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Queen, team))
{
    height = 96;
    set_team(team);
    set_piece_type(PieceType::Queen);
}

void Queen::update(float dt)
{
    
}

void Queen::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Queen::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current) 
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

bool Queen::hurt(PiecePtr attacker)
{
    return true;
}


int Queen::get_material_value() const
{
    return 9;
}

int Queen::get_max_mobility() const
{
    return 16;
}

PiecePtr Queen::clone_piece() const
{
    return std::make_shared<Queen>(*this);
}
