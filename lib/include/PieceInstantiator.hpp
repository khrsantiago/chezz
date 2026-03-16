#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <IGameObject.hpp>
#include <IPiece.hpp>
#include <Board.hpp>
#include "Archer.hpp"
#include "Crook.hpp"
#include "Horse.hpp"
#include "Pawn.hpp"
#include "Portal.hpp"
#include "Queen.hpp"
#include "Tower.hpp"
#include "Trapper.hpp"
#include "Bishop.hpp"
#include <SpriteManager.hpp>


BoardObjectPtr make_board_object(PieceType type, bool team, int startX, int startY);

class PieceInstantiator : public IGameObject
{
public:
    PieceInstantiator(PieceType _type, bool _team, sf::Vector2f position);
    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;
    void return_to_origin();
    BoardObjectPtr make_piece(int startX, int startY);
    PieceType get_type();
    bool get_team();
    
private:
    PieceType type;
    bool team;
    sf::Vector2f originalposition;
};
