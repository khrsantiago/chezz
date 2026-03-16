#include <SpriteManager.hpp>

std::map<std::string, sf::Texture> SpriteManager::texture_map;

void SpriteManager::load_texture_piece(std::string type, std::string file)
{
    sf::Texture texture;
    if (!texture.loadFromFile(file))
    {
        throw(std::runtime_error("Texture not found: " + file));
    }
        texture_map[type] = texture;
}

const sf::Texture& SpriteManager::get_piece_texture(std::string type)
{
    return texture_map.at(type);
}

const sf::Texture& SpriteManager::get_type_texture(PieceType type, bool team)
{
    std::string route = "assets/";

    switch (type)
    {
    case PieceType::Archer:
        return (team) ? get_piece_texture("white_archer") : get_piece_texture("black_archer") ;
        break;
    case PieceType::Queen:
        return (team) ? get_piece_texture("white_queen") : get_piece_texture("black_queen") ;
        break;
    case PieceType::Bishop:
        return (team) ? get_piece_texture("white_bishop") : get_piece_texture("black_bishop") ;
        break;
    case PieceType::Bomb:
        return (team) ? get_piece_texture("white_trap") : get_piece_texture("black_trap") ;
        break;
    case PieceType::Crook:
        return (team) ? get_piece_texture("white_crook") : get_piece_texture("black_crook") ;
        break;
    case PieceType::Portal:
        return (team) ? get_piece_texture("white_portal") : get_piece_texture("black_portal") ;
        break;
    case PieceType::Tower:
        return (team) ? get_piece_texture("white_rook") : get_piece_texture("black_rook") ;
        break;
    case PieceType::Trapper:
        return (team) ? get_piece_texture("white_trapper") : get_piece_texture("black_trapper") ;
        break;
    case PieceType::Horse:
        return (team) ? get_piece_texture("white_horse") : get_piece_texture("black_horse") ;
        break;
    case PieceType::Pawn:
    default:
        return (team) ? get_piece_texture("white_pawn") : get_piece_texture("black_pawn") ;
        break;
    }
}

void SpriteManager::init()
{
    std::string route = "assets/";

    SpriteManager::load_texture_piece("white_pawn", route + "WhitePawn.png");
    SpriteManager::load_texture_piece("white_rook", route + "WhiteRook.png");
    SpriteManager::load_texture_piece("white_bishop", route + "WhiteBishop.png");
    SpriteManager::load_texture_piece("white_horse", route + "WhiteHorse.png");
    SpriteManager::load_texture_piece("white_queen", route + "WhiteQueen.png");
    SpriteManager::load_texture_piece("white_portal", route + "WhitePortal.png");
    SpriteManager::load_texture_piece("white_crook", route + "WhiteCrook.png");
    SpriteManager::load_texture_piece("white_archer", route + "WhiteArcher.png");
    SpriteManager::load_texture_piece("white_trapper", route + "WhiteTrapper.png");
    SpriteManager::load_texture_piece("white_trap", route + "WhiteTrap.png");
    SpriteManager::load_texture_piece("black_pawn", route + "BlackPawn.png");
    SpriteManager::load_texture_piece("black_rook", route + "BlackRook.png");
    SpriteManager::load_texture_piece("black_bishop", route + "BlackBishop.png");
    SpriteManager::load_texture_piece("black_horse", route + "BlackHorse.png");
    SpriteManager::load_texture_piece("black_queen", route + "BlackQueen.png");
    SpriteManager::load_texture_piece("black_portal", route + "BlackPortal.png");
    SpriteManager::load_texture_piece("black_crook", route + "BlackCrook.png");
    SpriteManager::load_texture_piece("black_archer", route + "BlackArcher.png");
    SpriteManager::load_texture_piece("black_trapper", route + "BlackTrapper.png");
    SpriteManager::load_texture_piece("black_trap", route + "BlackTrap.png");
    SpriteManager::load_texture_piece("crown", route + "Crown.png");
}
