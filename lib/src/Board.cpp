#include <Board.hpp>

Board::Board(sf::Texture texture) : IGameObject(texture), white_king_in_board(false), black_king_in_board(false)
{
    sprite.setScale(sf::Vector2f(1.0,1.0));
}

Board::Board(sf::Texture texture, std::list<BoardObjectPtr> _elements) : IGameObject(texture), elements(_elements), white_king_in_board(false), black_king_in_board(false)
{
    sprite.setScale(sf::Vector2f(1.0,1.0));
}

Board::Board(const Board& _board) : IGameObject(_board.sprite.getTexture()), elements(_board.elements), white_king_in_board(false), black_king_in_board(false)
{
    sprite.setScale(sf::Vector2f(1.0,1.0));
}

void Board::update(float dt)
{
    for (auto element : elements)
    {
        element->piece->update(dt);
    }
}

void Board::clear()
{
    elements.clear();
    white_king_in_board = false;
    black_king_in_board = false;
}

bool Board::empty()
{
    return elements.empty();
}

void Board::update_avaiable_moves()
{
    for (auto element : elements)
    {
        element->piece->set_valid_moves(elements, element->pos);
    }
}

void Board::render(sf::RenderWindow& window)
{
    sf::RectangleShape cell({(float)(Board::cell_lenght),(float)(Board::cell_lenght)});
    auto origin = this->sprite.getPosition();
    
    for(int i = 0; i < Board::side_lenght * Board::side_lenght; i++)
    {
        cell.setFillColor( ((i + i/Board::side_lenght) % 2 == 0) ? sf::Color::White : sf::Color::Black);        
        auto pos = sf::Vector2<float>({(float)(i % Board::side_lenght * Board::cell_lenght),(float)(i / Board::side_lenght * Board::cell_lenght)});
        cell.setPosition(origin + pos);
        window.draw(cell);
    }
}

void Board::render_move_highlights(sf::RenderWindow& window, const std::vector<BoardObjectPtr>& valid_moves)
{
    sf::RectangleShape cell({(float)(Board::cell_lenght),(float)(Board::cell_lenght)});
    auto origin = this->sprite.getPosition();

    for (auto move : valid_moves)
    {
        cell.setFillColor( (move->piece) ? sf::Color{255, 0, 127, 128} : sf::Color{0, 127, 255, 128} );
        auto pos = sf::Vector2<float>({(float)(move->pos.x * Board::cell_lenght), (float)(move->pos.y * Board::cell_lenght)});
        cell.setPosition(origin + pos);
        window.draw(cell);
    }
}

void Board::render_instantiator_highlights(sf::RenderWindow& window)
{
    render_instantiator_highlights(window, true);
    render_instantiator_highlights(window, false);
}

void Board::render_instantiator_highlights(sf::RenderWindow& window, bool team)
{
    sf::RectangleShape cell({(float)(Board::cell_lenght),(float)(Board::cell_lenght)});
    cell.setFillColor(sf::Color{0, 127, 255, 128});
    auto origin = this->sprite.getPosition();

    uint8_t start = ((team) ? Board::side_lenght - 1 : 1);

    for (int i = start; i >= start - 1; i--)
    {
        for (size_t j = 0; j < 6; j++)
        {
            if (!this->get_position(j, i))
            {
                auto pos = sf::Vector2<float>({(float)(j * Board::cell_lenght), (float)(i * Board::cell_lenght)});
                cell.setPosition(origin + pos);
                window.draw(cell);
            }
        }
    }
}


void Board::render_pieces(sf::RenderWindow& window)
{
    sf::Vector2f wk_pos;
    int wk_height = 0;
    sf::Vector2f bk_pos;
    int bk_height = 0;

    for (auto element : elements)
    {
        element->piece->render(window);
        if (element->king)
        {
            if (element->piece->get_team())
            {
                wk_pos = element->piece->get_sprite().getPosition();
                wk_height = element->piece->get_height();
            }
            else
            {
                bk_pos = element->piece->get_sprite().getPosition();
                bk_height = element->piece->get_height();
            }
        }
    }

    if (white_king_in_board)
    {
        render_crown(window, wk_pos, wk_height);
    }

    if (black_king_in_board)
    {
        render_crown(window, bk_pos, bk_height);
    }
}

size_t Board::size()
{
    return elements.size();
}

BoardL Board::get_elements()
{
    return elements;
}

BoardObjectPtr Board::get_position(short x, short y)
{
    if ((x >= Board::side_lenght) || (y >= Board::side_lenght)) return nullptr;

    for (auto element : elements)
    {
        if (element->pos.x == x && element->pos.y == y) return element;
    }
    
    return nullptr;
}

void Board::remove_piece(BoardObjectPtr object)
{
    elements.erase(std::find(elements.begin(), elements.end(), object));
    
    if (object->king)
    {   
        bool found_substitute = false;
        for (auto element : elements)
        {
            if (element->piece->get_team() == object->piece->get_team()) 
            {
                element->king = true;
                found_substitute = true;
                break;
            }
        }
        if (!found_substitute)
        {
            if (object->piece->get_team()) white_king_in_board = false;
            else black_king_in_board = false;
        }
    }
}

BoardObjectPtr Board::clicked_piece(sf::Vector2i mouse_position)
{
    if (this->is_touching_mouse(mouse_position))
    {
        for (auto element : elements)
        {
            if (element->piece->is_touching_mouse(mouse_position))
            {
                return element;
            }   
        }
    }

    return nullptr;
}

void Board::set_piece_sprite(BoardObjectPtr element)
{
    auto objetive_position = element->pos;
    auto board_position = sprite.getPosition();
    auto offset = sf::Vector2f({(float)(objetive_position.x * Board::cell_lenght), (float)(objetive_position.y * Board::cell_lenght)});
    element->piece->set_sprite_position(board_position + offset);
}

Position Board::get_square_by_coords(sf::Vector2i mouse_position)
{
    sf::Vector2f relative_position = static_cast<sf::Vector2f>(mouse_position)- sprite.getPosition();
    return Position(relative_position.x / Board::cell_lenght, relative_position.y / Board::cell_lenght);
}

//Retorna si la pieza se movio en el tablero
bool Board::drop_piece(BoardObjectPtr element, long& score)
{
    bool it_moves = false;
    auto old_pos = element->pos;
    BoardObjectPtr attacked_piece;
    if (sprite.getGlobalBounds().contains(element->piece->get_sprite().getPosition()))
    {
        sf::FloatRect piece_center = element->piece->get_sprite().getLocalBounds();
        float center_x = piece_center.size.x / 2.0;
        float center_y = piece_center.size.y / 2.0;
        sf::Vector2f offset(center_x,center_y);
        Position position_on_board = get_square_by_coords(static_cast<sf::Vector2i>(element->piece->get_sprite().getPosition() + offset));
        attacked_piece = this->get_position(position_on_board.x, position_on_board.y);
        it_moves = this->move_piece(element, position_on_board);
    }

    if (it_moves) 
    {
        update_bombs(element, old_pos);
        if (attacked_piece) score += attacked_piece->get_points_value();
    }

    
    set_piece_sprite(element);
    return it_moves;
}

void Board::add_piece(BoardObjectPtr board_object)
{
    auto possible_conflictor = std::find_if(elements.begin(), elements.end(), [board_object] (const BoardObjectPtr& actual) { return board_object->pos == actual->pos; });
    if (possible_conflictor != elements.end()) elements.erase(possible_conflictor);
    

    if (board_object->piece->get_piece_type() != PieceType::Bomb)
    {
        if (board_object->piece->get_team())
        {//player or white team
            if (!white_king_in_board)
            {
                board_object->king = true;
                white_king_in_board = true;
            }    
        }    
        else
        {//bot or black team
            if (!black_king_in_board)
            {            
                board_object->king = true;
                black_king_in_board = true;
            }
        }
    }

    elements.push_back(board_object);
    set_piece_sprite(board_object);
}

bool Board::move_piece(BoardObjectPtr element, Position destination)
{
    bool it_moves = false;
    auto valid_moves = element->piece->get_valid_moves();
    for (auto move : valid_moves)
    {
        if (move->pos == destination)
        {
            it_moves = true;
            SoundManager::play(SoundType::MovePiece);
            if (move->piece)
            {
                if (element->piece->get_piece_type() == PieceType::Portal && element->piece->get_team() == move->piece->get_team())
                {                    
                    move->pos = element->pos;
                    set_piece_sprite(move);
                }
                else if (move->piece->hurt(element->piece))
                {
                    auto it = std::find(elements.begin(), elements.end(), (move->piece->get_piece_type() == PieceType::Bomb) ? element : move);
                    if (it != elements.end())
                    {
                        elements.erase(it);

                        if (move->king)
                        {
                            if (move->piece->get_team())
                            {// player/white team
                                white_king_in_board = false;
                            }
                            else
                            {// bot/black team
                                black_king_in_board = false;
                            }
                        }
                    }
                }
            }
            element->pos = destination;
            break;
        }
    }

    return it_moves;
}

void Board::update_bombs(BoardObjectPtr moved_piece, Position old_position)
{
    if (moved_piece->piece->get_piece_type() == PieceType::Trapper)
        {
            add_piece(std::make_shared<InBoardObject>(old_position, std::make_shared<Bomb>(moved_piece->piece->get_team())));
        }
        
        bool team_of_bombs_to_remove = !moved_piece->piece->get_team(); 
        elements.erase(std::remove_if(elements.begin(), elements.end(),
            [team_of_bombs_to_remove](BoardObjectPtr element) {
                return element->piece->get_piece_type() == PieceType::Bomb && element->piece->get_team() == team_of_bombs_to_remove;}), elements.end());
}

bool Board::is_black_king_in_board() const
{
    return black_king_in_board;
}

bool Board::is_white_king_in_board() const
{
    return white_king_in_board;
}

void Board::render_crown(sf::RenderWindow& window, sf::Vector2f position, float height)
{
    sf::Sprite crown(SpriteManager::get_piece_texture("crown"));
    crown.setOrigin(sf::Vector2f(-10,16));
    crown.setPosition(sf::Vector2f(position.x, position.y + (96 - height)));
    window.draw(crown);
}

void Board::on_resize()
{
    for (auto piece : elements)
    {
        set_piece_sprite(piece);
    }
}



