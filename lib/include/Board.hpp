#pragma once

#include <SFML/Graphics.hpp>
#include <SoundManager.hpp>
#include <IPiece.hpp>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "Trapper.hpp"

using BoardObjectPtr = std::shared_ptr<InBoardObject>;
using BoardL = std::list<BoardObjectPtr>;

class Board : public IGameObject
{
private:
    //static const sf::Color black{70,50,50,255};
    //static const sf::Color white{220,200,200,255}; 
    BoardL elements;
    bool white_king_in_board;
    bool black_king_in_board;

public:

    static constexpr uint8_t side_lenght = 6;
    static constexpr uint8_t cell_lenght = 100;
    Board(sf::Texture texture);
    Board(sf::Texture texture, std::list<BoardObjectPtr> _elements);
    Board(const Board& _board);

    size_t size();
    BoardL get_elements();
    BoardObjectPtr get_position(short x, short y);
    void remove_piece(BoardObjectPtr piece);
    BoardObjectPtr clicked_piece(sf::Vector2i mouse_position);
    Position get_square_by_coords(sf::Vector2i mouse_position);
    bool drop_piece(BoardObjectPtr element, long& score);
    bool move_piece(BoardObjectPtr element, Position destination);
    void update_bombs(BoardObjectPtr moved_piece, Position old_position);
    void update(float dt) override;
    void update_avaiable_moves();
    void render(sf::RenderWindow& window) override;
    void render_move_highlights(sf::RenderWindow& window, const std::vector<BoardObjectPtr>& valid_moves);
    void render_instantiator_highlights(sf::RenderWindow& window);
    void render_instantiator_highlights(sf::RenderWindow& window, bool team);
    void render_pieces(sf::RenderWindow& window);
    void add_piece(BoardObjectPtr piece);
    void set_piece_sprite(BoardObjectPtr element);
    bool is_black_king_in_board() const;
    bool is_white_king_in_board() const;
    void render_crown(sf::RenderWindow&, sf::Vector2f, float);
    void on_resize();
    void clear();
    bool empty();
};

