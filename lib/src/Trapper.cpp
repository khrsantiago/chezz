#include "Trapper.hpp"

const std::vector<Position> Trapper::directions = {Position(0, 2), Position(0, -2), Position(-2, 0), Position(2, 0)};

sf::Color Bomb::white = sf::Color(120,120,20,255);
sf::Color Bomb::black = sf::Color(20,20,0,255);

sf::Color Bomb::get_color(bool _team)
{
return (_team)? Bomb::white : Bomb::black;
}

Bomb::Bomb(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Bomb, team))
{
    set_team(team);
    set_piece_type(PieceType::Bomb);
}

void Bomb::update(float dt)
{
    
}

void Bomb::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Bomb::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current) 
{
   valid_moves.clear();
   return valid_moves;    
}

bool Bomb::hurt(PiecePtr attacker)
{
    attacker->hurt(std::make_shared<Bomb>(this));
    return true;
}

int Bomb::get_material_value() const
{
    return 0;
}

int Bomb::get_max_mobility() const
{
    return 0;
}

PiecePtr Bomb::clone_piece() const
{
    return std::make_shared<Bomb>(*this);
}

sf::Color Trapper::white = sf::Color(170,170,70,255);
sf::Color Trapper::black = sf::Color(70,70,0,255);

sf::Color Trapper::get_color(bool _team)
{
return (_team)? Trapper::white : Trapper::black;
}

Trapper::Trapper(bool team) : IGameObject(SpriteManager::get_type_texture(PieceType::Trapper, team))
{
    height = 80;
    set_team(team);
    set_piece_type(PieceType::Trapper);
}

void Trapper::update(float dt)
{
    
}

void Trapper::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

std::vector<BoardObjectPtr> Trapper::set_valid_moves(const std::list<BoardObjectPtr>& pieces, Position current)
{
    valid_moves.clear();
    const uint8_t lenght = Board::side_lenght;

    for (auto direction : Trapper::directions)
    {
        int new_x = current.x + direction.x;
        int new_y = current.y + direction.y;

        if (new_x < 0 || new_x >= Board::side_lenght || new_y < 0 || new_y >= Board::side_lenght)
        {
            continue;
        }

        auto target = std::make_shared<InBoardObject>(Position(new_x, new_y));
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
            if(target->piece->get_team() != team)
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

bool Trapper::hurt(PiecePtr attacker)
{
    return true;
}

int Trapper::get_material_value() const
{
    return 5;
}

int Trapper::get_max_mobility() const
{
    return 4;
}

PiecePtr Trapper::clone_piece() const
{
    return std::make_shared<Trapper>(*this);
}
