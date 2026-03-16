#include <IStatePlayable.hpp>
#include <Stats.hpp>
IStatePlayable::IStatePlayable(sf::RenderWindow* _window) : btn_back_sprite(tex_exit), btnStart(nullptr), board(sf::Texture(sf::Vector2u((unsigned int)(Board::side_lenght * Board::cell_lenght),
                                                                              (unsigned int)(Board::side_lenght * Board::cell_lenght)))), player_turn(true), bot(BoardL(), GameEvaluator()), round_display(font, "Round 1"), score_display(font, "0 Score"), result(font)
{
    window = _window;
    actual_phase = PhaseType::Preparing;
    actual_winner = PlayerType::None;

    result.setCharacterSize(100);

    amount_of_white_instances = 0;
    amount_of_black_instances = 0;
}

IStatePlayable::~IStatePlayable() { }

void IStatePlayable::drag()
{
    auto mouse_position = this->get_relative_mouse_position();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {        
        if (selected_piece)
        {   
            selected_piece->piece->set_sprite_position({(float)mouse_position.x - fix_offset(selected_piece->piece->get_sprite(), 'x'), (float)mouse_position.y - fix_offset(selected_piece->piece->get_sprite(),'y')});
            selected_inst = nullptr;
        }
        else if (selected_inst)
        {
            selected_inst->set_sprite_position({(float)mouse_position.x - fix_offset(selected_inst->get_sprite(), 'x'), (float)mouse_position.y - fix_offset(selected_inst->get_sprite(), 'y')});
            selected_piece = nullptr;
        }
        else
        {
            selected_piece = board.clicked_piece(mouse_position);

            if (selected_piece)
            {
                if (selected_piece->piece && (actual_phase == PhaseType::Fighting && selected_piece->piece->get_team() != player_turn)) selected_piece = nullptr;                
            }
            else if (actual_phase == PhaseType::Preparing)
            {
                selected_inst = clicked_instantiator(mouse_position);
            }
        }
    }
    else if (selected_piece)
    {   
        if (actual_phase == PhaseType::Fighting) 
        {
            if (board.drop_piece(selected_piece, score))
            {
                this->end_turn();
            }
        }
        else if (actual_phase == PhaseType::Preparing)
        {
            if (selected_piece->piece->get_team())   amount_of_white_instances--;
            else                                     amount_of_black_instances--;
            returned_piece();
        }

        bot.set_current_board(board.get_elements());
        selected_piece = nullptr;
    }
    else if (selected_inst)
    {
        auto amount = (selected_inst->get_team() ? amount_of_white_instances : amount_of_black_instances);
        if(board.is_touching_mouse(mouse_position) && amount < 5)
        {
            auto pos = board.get_square_by_coords(mouse_position);
            auto new_piece = selected_inst->make_piece(pos.x, pos.y);
            if (this->type == StateType::Practice || (new_piece->piece->get_team() && pos.y >= 4) || (!new_piece->piece->get_team() && pos.y <= 1))
            {
                if (!board.get_position(pos.x, pos.y))
                {
                    board.add_piece(new_piece);
                    if (new_piece->piece->get_team())   amount_of_white_instances++;
                    else                                amount_of_black_instances++;
                    dropped_inst();
                }
            }
            else
            {
                //Reproducir sonido de error tal vez, notificar que no es valido instanciar asi
            }
        }
        
        selected_inst->return_to_origin();
        selected_inst = nullptr;
    }
}

float IStatePlayable::fix_offset(const sf::Sprite& _sprite, char t)
{
    sf::FloatRect boundaries = _sprite.getGlobalBounds();
    if(t == 'x') return boundaries.size.x / 2.0;

    else return boundaries.size.y / 2.0;
}

PieceInstantPtr IStatePlayable::clicked_instantiator(sf::Vector2i mouse_position)
{
    sf::Vector2f pos = static_cast<sf::Vector2f>(mouse_position);
    
    for (auto instantiator : instantiators)
    {
        if (instantiator->get_sprite().getGlobalBounds().contains(pos))
        {
            return instantiator;
        }
    }

    return nullptr;
}

void IStatePlayable::end_turn()
{
    auto elements = board.get_elements();
    for (auto element : elements)
    {
        if (element->piece->get_piece_type() == PieceType::Pawn && ((element->piece->get_team() && element->pos.y == 0) || (!element->piece->get_team() && element->pos.y == 5)))
        {
            board.add_piece(std::make_shared<InBoardObject>(element->pos, std::make_shared<Queen>(element->piece->get_team()), element->king));
        }
    }

    bot.initial_game_eval();
    board.update_avaiable_moves();

    //std::cout << score << std::endl;

    score_display.setString(std::to_string(score) + " Score");

    sf::FloatRect textRect = score_display.getLocalBounds();
    score_display.setOrigin(sf::Vector2f(textRect.size.x, 0));
    textRect = round_display.getLocalBounds();
    round_display.setOrigin(sf::Vector2f(textRect.size.x, 0));


    player_turn = !player_turn;
    //Aqui se implementa el arbitro mejoe
    auto possible_winner = check_winner();
    if (possible_winner != PlayerType::None)
    {
        actual_winner = possible_winner;

        if (actual_winner == PlayerType::P2 && type == StateType::Gameplay)
        {
            Stats stats(score, round);
            stats.save_or_update();
        }
        
        if (actual_winner == PlayerType::P1 || this->type == StateType::Versus) SoundManager::play(SoundType::Victory);
        else if (this->type == StateType::Gameplay || this->type == StateType::Practice) SoundManager::play(SoundType::Defeat);
        
        round_display.setString("Round " + std::to_string(round));
        transition.enter(10);
        return;
    }

    possible_winner = check_stalemate();
    if (possible_winner != PlayerType::None)
    {
        actual_winner = possible_winner;

        if (actual_winner == PlayerType::P2 && type == StateType::Gameplay)
        {
            Stats stats(score, round);
            stats.save_or_update();
        }

        if (actual_winner == PlayerType::P1 || this->type == StateType::Versus) SoundManager::play(SoundType::Victory);
        else if (this->type == StateType::Gameplay || this->type == StateType::Practice) SoundManager::play(SoundType::Defeat);

        round_display.setString("Round " + std::to_string(round));
        transition.enter(10);
        return;
    }
    
    // std::cout << round << std::endl;
}

PlayerType IStatePlayable::check_winner()
{
    auto elements = board.get_elements();
    BoardObjectPtr P1_King = nullptr;
    BoardObjectPtr P2_King = nullptr;

    for (auto element : elements)
    {
        if (element->king)
        {
            if (element->piece->get_team()) P1_King = element;
            else P2_King = element;
        }
    }

    if (!P1_King) 
    {
        return PlayerType::P2;
    }
    if (!P2_King) 
    {
        return PlayerType::P1;
    }
    return PlayerType::None;
}

//Retorna el player que aun se puede mover, en caso de que uno no pueda
PlayerType IStatePlayable::check_stalemate()
{
    auto elements = board.get_elements();

    //Verifica si no puedes jugar
    if (player_turn && bot.generate_all_plays(elements, true).empty()) return PlayerType::P2;
    if (!player_turn && bot.generate_all_plays(elements, false).empty()) return PlayerType::P1;

    bool p1_occupies_white = false;
    bool p1_occupies_black = false;
    bool p2_occupies_white = false;
    bool p2_occupies_black = false;

    bool p1_has_non_portal = false;
    bool p2_has_non_portal = false;

    bool has_freemove_piece = false;

    for (auto element : elements)
    {
        auto type = element->piece->get_piece_type();
        bool team = element->piece->get_team();
        
        if (type != PieceType::Bishop && type != PieceType::Horse && type != PieceType::Trapper && type != PieceType::Portal) 
        {
            has_freemove_piece = true;
        }

        if (type != PieceType::Portal)
        {
            if (team) p1_has_non_portal = true;
            else p2_has_non_portal = true;
        }

        int sum = element->pos.x + element->pos.y; 
        
        if (sum % 2 != 0) // Casilla Blanca
        {
            if (team) p1_occupies_white = true;
            else      p2_occupies_white = true;
        }
        else // Casilla Negra
        {
            if (team) p1_occupies_black = true;
            else      p2_occupies_black = true;
        }
    }

    // Si solo tienes portales pierdes
    if (!p1_has_non_portal) return PlayerType::P2;
    if (!p2_has_non_portal) return PlayerType::P1;

    if (has_freemove_piece) return PlayerType::None;

    bool meet_on_white = (p1_occupies_white && p2_occupies_white);
    
    bool meet_on_black = (p1_occupies_black && p2_occupies_black);

    if (meet_on_white || meet_on_black)
    {
        return PlayerType::None; // Hay interacción posible, el juego continúa.
    }

    return PlayerType::P2; 
}

void IStatePlayable::start_fight()
{
    this->actual_winner = PlayerType::None;
    this->actual_phase = PhaseType::Fighting;
    instantiators.clear();
    amount_of_black_instances = 0;
    amount_of_white_instances = 0;
    board.update_avaiable_moves();
}
