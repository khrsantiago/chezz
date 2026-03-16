#include "Bishop.hpp"

const std::vector<Position> Bishop::directions = {Position(1, -1), Position(1, 1), Position(-1, 1), Position(-1, -1)};

sf::Color Bishop::white = sf::Color(150,150,250,255);
sf::Color Bishop::black = sf::Color(50,50,150,255);

sf::Color Bishop::get_color(bool _team)
{
return (_team)? Bishop::white : Bishop::black;
}

Bishop::Bishop(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Bishop, team))
{
    height = 80;
    set_team(team);
    set_piece_type(PieceType::Bishop);
}

void Bishop::update(float dt)
{
    
}

void Bishop::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Bishop::set_valid_moves(const std::list<BoardObjectPtr>& elements, Position current)
{
    valid_moves.clear();
    const int8_t lenght = Board::side_lenght;

    for (auto direction : directions)
    {
        int delta_x = direction.x;
        int delta_y = direction.y;
        for (size_t k = 1; k <= 2; k++)
        {
            uint8_t tile_x = current.x + (k * delta_x);
            uint8_t tile_y = current.y + (k * delta_y);

            if (tile_x >= lenght || tile_y >= lenght) break;

            BoardObjectPtr piece_at_cell = std::make_shared<InBoardObject>(Position(tile_x, tile_y));
            
            for(auto const& element : elements)
            {
                if (element->pos == piece_at_cell->pos)
                {
                    piece_at_cell = element;
                    break;
                }
            }

            if (piece_at_cell->piece)
            {
                if (piece_at_cell->piece->get_team() != this->get_team())
                {
                    valid_moves.push_back(piece_at_cell);
                }
                break;
            }
            else
            {
                piece_at_cell->pos = piece_at_cell->pos;
                valid_moves.push_back(piece_at_cell);
            }
        }
        
    }
    return valid_moves;    
}

bool Bishop::hurt(PiecePtr attacker)
{
    return true;
}

int Bishop::get_material_value() const
{
    return 6;
}

int Bishop::get_max_mobility() const
{
    return 8;
}

PiecePtr Bishop::clone_piece() const
{
    return std::make_shared<Bishop>(*this);
}
