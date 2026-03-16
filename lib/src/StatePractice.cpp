#include <StatePractice.hpp>

StatePractice::StatePractice(sf::RenderWindow* _window) : IStatePlayable(_window)
{
    type = StateType::Practice;
    go_to = StateType::None;
}

StatePractice::~StatePractice() {}

void StatePractice::init()
{
    actual_phase = PhaseType::Preparing;

    //board.add_piece(std::make_shared<InBoardObject>(Position(3, 5), std::make_shared<Queen>(true, SpriteManager::get_piece_texture("white_queen"))));
    //board.add_piece(std::make_shared<InBoardObject>(Position(1, 4), std::make_shared<Archer>(true, SpriteManager::get_piece_texture("white_archer"))));
    //board.add_piece(std::make_shared<InBoardObject>(Position(5, 5), std::make_shared<Crook>(true, SpriteManager::get_piece_texture("white_crook"))));
    //
    //board.add_piece(std::make_shared<InBoardObject>(Position(0, 1), std::make_shared<Horse>(false, SpriteManager::get_piece_texture("black_horse"))));
    //board.add_piece(std::make_shared<InBoardObject>(Position(2, 1), std::make_shared<Queen>(false, SpriteManager::get_piece_texture("black_queen"))));
    //board.add_piece(std::make_shared<InBoardObject>(Position(1, 1), std::make_shared<Archer>(false, SpriteManager::get_piece_texture("black_archer"))));
    //board.add_piece(std::make_shared<InBoardObject>(Position(1, 0), std::make_shared<Portal>(false, SpriteManager::get_piece_texture("black_portal"))));
    //board.add_piece(std::make_shared<InBoardObject>(Position(5, 1), std::make_shared<Trapper>(false, SpriteManager::get_piece_texture("black_trapper"))));
    
    if (!font.openFromFile("assets/fonts/arial.ttf")) 
    {
         std::cerr << "ERROR: No se pudo cargar fuente en Gameplay" << std::endl;
    }
    if(!tex_exit.loadFromFile("assets/tutorial/exitBtn.png"))
    {
        std::cerr << "Error cargando textura de boton de regreso en Gameplay" << std::endl;
    }

    tex_exit.setSmooth(true);
    btn_back_sprite.setTexture(tex_exit, true);

    if (!start_texture.loadFromFile("assets/startBtn.png"))
    {
        std::cerr << "Error cargando textura de boton de start en Gameplay" << std::endl;
    }
    start_texture.setSmooth(true);
    btnStart = new Button(start_texture, font);
    
    bot.set_current_board(board.get_elements());
    bot.initial_game_eval();
    this->adjust_elements();
}

void StatePractice::terminate()
{
    delete btnStart;
    btnStart = nullptr;
}

void StatePractice::update(float dt)
{
    transition.update(dt);

    if (transition.is_staying())
    {
        sf::Vector2u win_size = window->getSize();
        switch (actual_winner)
        {
        case PlayerType::P1:
            result.setString("You Win");
            result.setFillColor(sf::Color::Cyan);
            center_sftext(result, win_size, -200);
            break;
        case PlayerType::P2:
            result.setString("You Lose");
            result.setFillColor(sf::Color::Red);
            center_sftext(result, win_size, -200);
            break;
        default:
            result.setString("What Happend??");
            result.setFillColor(sf::Color::Cyan);
            break;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            auto possible_winner = check_winner();
            auto possible_stalemate = check_stalemate();        
            if (possible_winner != PlayerType::None)
            {
                this->end_fight(possible_winner);
            }
            else if (possible_stalemate != PlayerType::None)
            {
                this->end_fight(possible_stalemate);
            }

            transition.leave(10);
            actual_winner = PlayerType::None;
        }

        
    }

    if (!transition.is_out())
        return;

    if (player_turn)
    {
        drag();
    }
    else
    {
        sf::sleep(sf::seconds(1.f));
        auto bot_play = bot.find_best_play(4);

        if (bot_play.moving_piece)
        {
            Position old_position = bot_play.moving_piece->pos;

            board.move_piece(bot_play.moving_piece, bot_play.destination);
            board.set_piece_sprite(bot_play.moving_piece);

            board.update_bombs(bot_play.moving_piece, old_position);
        }

        player_turn = true;
    }
    sf::Vector2i mousePos = get_relative_mouse_position();

    if (is_mouse_over(btn_back_sprite, mousePos)) btn_back_sprite.setColor(sf::Color(200, 200, 200));
    else btn_back_sprite.setColor(sf::Color::White);

    if (is_mouse_over(btn_back_sprite, mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        this->go_to = StateType::Return;
        return;
    }

    if (btnStart) 
    {
        if (is_mouse_over(btnStart->btn_sprite, mousePos)) 
        {
            if(start_texture.loadFromFile("assets/startBtnR.png"))
            {
                btnStart->btn_sprite.setTexture(start_texture);
            }
            else 
            {
                std::cerr << "ERROR: No se pudo cargar boton rojo" << std::endl;
            }
        }
        else 
        {
            if(start_texture.loadFromFile("assets/startBtn.png"))
            {
                btnStart->btn_sprite.setTexture(start_texture);
            }
            else 
            {
                std::cerr << "ERROR: No se pudo cargar boton estandar" << std::endl;
            }
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (btnStart && is_mouse_over(btnStart->btn_sprite, mousePos) && board.is_white_king_in_board() && board.is_black_king_in_board())
        {
            this->start_fight();

            return;
        }
    }
    board.update(dt);
}

void StatePractice::render(sf::RenderWindow& window)
{
    board.render(window);
    if (selected_piece) board.render_move_highlights(window, selected_piece->piece->get_valid_moves());

    board.render_pieces(window);

    for (auto inst : instantiators)
    {
        inst->render(window);
    }

    window.draw(btn_back_sprite);

    if (btnStart && actual_phase == PhaseType::Preparing) 
    {
        window.draw(btnStart->btn_sprite);
        window.draw(btnStart->btn_text);
    }

    transition.render(window);

    if (transition.is_staying())
    {
        window.draw(result);
    }
}

void StatePractice::on_resize() 
{
    adjust_elements();
}

void StatePractice::adjust_elements()
{
    float halfboard_lenght = Board::side_lenght * Board::cell_lenght / 2;
    auto pos = sf::Vector2<float>((float)(window->getSize().x/2 - halfboard_lenght),
                                  (float)(window->getSize().y/2 - halfboard_lenght));
    board.set_sprite_position(pos);
    board.on_resize();

    

    sf::Vector2u winSizeU = window->getSize();
    sf::Vector2f winSize((float)winSizeU.x, (float)winSizeU.y);
    float target_btn_height = winSize.y * 0.08f;
    
    sf::Vector2u btn_texture_size = tex_exit.getSize();

    if (btn_texture_size.y > 0) 
    {
        float scale = (target_btn_height / btn_texture_size.y);
        btn_back_sprite.setScale(sf::Vector2f(scale, scale));
    }

    float margin = 20.0f;
    sf::FloatRect bounds = btn_back_sprite.getLocalBounds(); 
    btn_back_sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
    
    float pos_x = margin + bounds.size.x * btn_back_sprite.getScale().x / 2.0f;
    float pos_y = margin + bounds.size.y * btn_back_sprite.getScale().y / 2.0f;

    btn_back_sprite.setPosition(sf::Vector2f(pos_x, pos_y));
    setup_button(btnStart,"START",winSize.x - pos_x * 1.8, winSize.y - pos_y,font,start_texture);
    btnStart->btn_text.setOutlineThickness(2);
    btnStart->btn_text.setOutlineColor(sf::Color::Black);
    this->load_instanciators();
}

void StatePractice::dropped_inst()
{
    if (!selected_inst) return;

    load_instanciators();
}

void StatePractice::returned_piece()
{
    if (!selected_piece) return;

    board.remove_piece(selected_piece);
    
    load_instanciators();
}

void StatePractice::end_fight(PlayerType winner)
{
    actual_phase = PhaseType::Preparing;
    
    board.clear();
    round++;
    player_turn = true;

    this->adjust_elements();
}

void StatePractice::load_instanciators()
{
    float width = (float)window->getSize().x;

    instantiators.clear();

    auto updated_ymargin = ymargin + 50; 

    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Pawn, true, sf::Vector2f(xmargin, updated_ymargin)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Horse, true, sf::Vector2f(xmargin, updated_ymargin + yoffset)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Bishop, true, sf::Vector2f(xmargin, updated_ymargin + 2*yoffset)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Tower, true, sf::Vector2f(xmargin, updated_ymargin + 3*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Queen, true, sf::Vector2f(xmargin, updated_ymargin + 4*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Trapper, true, sf::Vector2f(xmargin + xoffset, updated_ymargin + yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Crook, true, sf::Vector2f(xmargin + xoffset, updated_ymargin + 2*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Archer, true, sf::Vector2f(xmargin + xoffset, updated_ymargin + 3*yoffset)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Portal, true, sf::Vector2f(xmargin + xoffset, updated_ymargin + 4*yoffset)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Pawn, false, sf::Vector2f(width - (xmargin + 100), updated_ymargin)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Horse, false, sf::Vector2f(width - (xmargin + 100), updated_ymargin + yoffset)));
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Bishop, false, sf::Vector2f(width - (xmargin + 100), updated_ymargin + 2*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Tower, false, sf::Vector2f(width - (xmargin + 100), updated_ymargin + 3*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Queen, false, sf::Vector2f(width - (xmargin + 100), updated_ymargin + 4*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Trapper, false, sf::Vector2f(width - (xmargin + 100) - xoffset, updated_ymargin + yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Crook, false, sf::Vector2f(width - (xmargin + 100) - xoffset, updated_ymargin + 2*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Archer, false, sf::Vector2f(width - (xmargin + 100) - xoffset, updated_ymargin + 3*yoffset))); 
    instantiators.push_back(std::make_shared<PieceInstantiator>(PieceType::Portal, false, sf::Vector2f(width - (xmargin + 100) - xoffset, updated_ymargin + 4*yoffset)));
    
}
