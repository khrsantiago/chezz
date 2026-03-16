#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

enum class PieceType
{
    Pawn ,
    Horse,
    Bishop,
    Tower,
    Queen,
    Trapper,
    Crook,
    Archer,
    Portal,
    Bomb
};

class SpriteManager
{
private:
    static std::map<std::string,sf::Texture> texture_map;
    
public:
    static void load_texture_piece(std::string type, std::string file);
    static const sf::Texture& get_piece_texture(std::string type);
    static const sf::Texture& get_type_texture(PieceType type, bool team);
    static void init();

    virtual void abstract() = 0;
};
