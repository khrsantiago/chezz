#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <IPiece.hpp>
#include <Board.hpp>

class Portal : public IPiece
{
    public:
    Portal(bool team);

    virtual ~Portal() = default;
    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;
    virtual std::vector<BoardObjectPtr> set_valid_moves(const std::list<BoardObjectPtr>& elements, Position current) override;
    virtual bool hurt(PiecePtr attacker) override;
    virtual int get_material_value() const override;
    virtual int get_max_mobility() const override;
    virtual PiecePtr clone_piece() const override;
    static sf::Color get_color(bool);
    static sf::Color white;
    static sf::Color black;

private:
};
