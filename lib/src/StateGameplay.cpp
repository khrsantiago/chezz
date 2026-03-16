#include <StateGameplay.hpp>

StateGameplay::StateGameplay(sf::RenderWindow* _window) : background_sprite(background_texture), IStatePlayable(_window)
{
    type = StateType::Gameplay;
    go_to = StateType::None;
}

StateGameplay::~StateGameplay() {}

void StateGameplay::init()
{
    srand(time(NULL));


    if(!background_texture.loadFromFile("assets/game_background.png"))
    {
        std::cerr << "ERROR: No se pudo cargar fondo en Gameplay" << std::endl;
    }
    auto size = background_texture.getSize();
    background_sprite.setTexture(background_texture);
    background_sprite.setTextureRect({{0, 0}, {static_cast<int>(size.x), static_cast<int>(size.y)}});
    
    if (!tex_exit.loadFromFile("assets/tutorial/exitBtn.png"))
    {
        std::cerr << "Error cargando textura de boton de regreso en Gameplay" << std::endl;
    }

    tex_exit.setSmooth(true);
    btn_back_sprite.setTexture(tex_exit, true);
    //Aqui se intenta cargar una partida anterior que se haya guardado, sino se carga este cacho
    new_game();
    
    
    if (!font.openFromFile("assets/fonts/arial.ttf")) 
    {
        std::cerr << "ERROR: No se pudo cargar fuente en Gameplay" << std::endl;
    }

    round_display.setFont(font);
    score_display.setFont(font);

    if(!start_texture.loadFromFile("assets/startBtn.png"))
    {
        std::cerr << "Error cargando textura de boton de start en Gameplay" << std::endl;
    }
    start_texture.setSmooth(true);
    btnStart = new Button(start_texture, font);
    adjust_elements();
    bot.set_current_board(board.get_elements());
    bot.initial_game_eval();
}

void StateGameplay::terminate()
{
    delete btnStart;
    btnStart = nullptr;
}

void StateGameplay::update(float dt)
{
    transition.update(dt);

    if (transition.is_staying())
    {
        sf::Vector2u win_size = window->getSize();
        switch (actual_winner)
        {
        case PlayerType::P1:
            result.setString("You win this round");
            result.setFillColor(sf::Color::Cyan);
            center_sftext(result, win_size, -200);
            center_sftext(score_display, win_size, 0);
            center_sftext(round_display, win_size, 100);
            break;
        case PlayerType::P2:
            result.setString("You Lose");
            result.setFillColor(sf::Color::Red);
            center_sftext(result, win_size, -200);
            center_sftext(score_display, win_size, 0);
            center_sftext(round_display, win_size, 100);
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

    if (actual_phase != PhaseType::Fighting || player_turn)
    {
        drag();
        if (selected_piece && !selected_piece->piece->get_team()) selected_piece = nullptr;
    }
    else
    {
        sf::sleep(sf::seconds(1.f));
        auto bot_play = bot.find_best_play(difficulty);

        if (bot_play.moving_piece)
        {
            Position old_position(bot_play.moving_piece->pos);

            board.move_piece(bot_play.moving_piece, bot_play.destination);
            board.set_piece_sprite(bot_play.moving_piece);

            board.update_bombs(bot_play.moving_piece, old_position);
            this->end_turn();
        }

        player_turn = true;
    }
    sf::Vector2i mouse_pos = get_relative_mouse_position();

    if (is_mouse_over(btn_back_sprite, mouse_pos)) btn_back_sprite.setColor(sf::Color(200, 200, 200));
    else btn_back_sprite.setColor(sf::Color::White);

    if (is_mouse_over(btn_back_sprite, mouse_pos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        this->go_to = StateType::Return;
        return;
    }

    if (btnStart) 
    {
        if (is_mouse_over(btnStart->btn_sprite, mouse_pos)) 
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
        if (btnStart && is_mouse_over(btnStart->btn_sprite, mouse_pos) && board.is_white_king_in_board())
        {
            this->start_fight();

            return;
        }
    }
    board.update(dt);
}


void StateGameplay::render(sf::RenderWindow& window)
{
    window.draw(background_sprite);

    window.draw(btn_back_sprite);

    if (actual_phase == PhaseType::Preparing) 
    {
        window.draw(btnStart->btn_sprite);
        window.draw(btnStart->btn_text);
    }

    board.render(window);
    if (actual_phase == PhaseType::Fighting && selected_piece) board.render_move_highlights(window, selected_piece->piece->get_valid_moves());
    if (actual_phase == PhaseType::Preparing && selected_inst) board.render_instantiator_highlights(window, true);
    board.render_pieces(window);

    for (auto inst : instantiators)
    {
        inst->render(window);
    }

    window.draw(score_display);
    window.draw(round_display);
    
    transition.render(window);
    if (transition.is_staying())
    {
        window.draw(result);
        window.draw(score_display);
        window.draw(round_display);
    }
}

void StateGameplay::on_resize() 
{
    adjust_elements();
}

void StateGameplay::adjust_elements()
{
    sf::Vector2u win_size = window->getSize();
    sf::Vector2u original_size = background_texture.getSize();
    float scale_x = (float)win_size.x / original_size.x;
    float scale_y = (float)win_size.y / original_size.y;

    background_sprite.setScale(sf::Vector2f(scale_x, scale_y));
    background_sprite.setOrigin(sf::Vector2f(0.0f, 0.0f)); 
    background_sprite.setPosition(sf::Vector2f(0.0f, 0.0f));

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
        float scale = target_btn_height / btn_texture_size.y;
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


    pos_x = win_size.x - (Board::cell_lenght / 2);
    
    score_display.setPosition({pos_x, Board::cell_lenght/2});
    round_display.setPosition({pos_x, Board::cell_lenght});

    sf::FloatRect textRect = score_display.getLocalBounds();
    score_display.setOrigin(sf::Vector2f(textRect.size.x, 0));
    textRect = round_display.getLocalBounds();
    round_display.setOrigin(sf::Vector2f(textRect.size.x, 0));

    this->load_instanciators();
    
}

void StateGameplay::new_game()
{
    this->actual_phase = PhaseType::Preparing;
    if (inventory.empty())
    {
        for (size_t i = 0; i < 5; i++)
        {
            inventory.push_front(PieceType::Pawn);
        }
    }
    difficulty = 2;
    round = 1;
    score = 0;
    enemy_king = static_cast<PieceType>(rand() % 9);
    this->set_up_black_team();
}

bool StateGameplay::set_up_black_team()
{
    FileManager presets("kingpresets.json");

    std::string index_line;
    if(!presets.read_line(index_line))
    {
        std::cerr << "Error: Can't read index line in kingpresets.json" << std::endl;
        return false;
    }

    json index_values;
    try {
        index_values = json::parse(index_line);
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON index: " << e.what() << std::endl;
        return false;
    }

    std::string piece_name;
    switch(enemy_king)
    {
        case PieceType::Pawn: piece_name = "Pawn"; break;
        case PieceType::Horse: piece_name = "Horse"; break;
        case PieceType::Bishop: piece_name = "Bishop"; break;
        case PieceType::Tower: piece_name = "Rook"; break;
        case PieceType::Queen: piece_name = "Queen"; break;
        case PieceType::Trapper: piece_name = "Trapper"; break;
        case PieceType::Crook: piece_name = "Crook"; break;
        case PieceType::Archer: piece_name = "Archer"; break;
        case PieceType::Portal: piece_name = "Portal"; break;
    }

    std::string start_key = piece_name + "Start";
    std::string amount_key = piece_name + "Amount";
    
    if (!index_values.contains(start_key) || !index_values.contains(amount_key)) {
        std::cerr << "Error: No keys in kingpresets.json for: " << piece_name << std::endl;
        std::cerr << "Available keys: ";
        for (auto& [key, value] : index_values.items()) {
            std::cerr << key << " ";
        }
        std::cerr << std::endl;
        return false;
    }

    int start_index = index_values[start_key];
    int amount = index_values[amount_key];

    if (amount <= 0) {
        std::cerr << "Error: Negative or 0 amount " << piece_name << std::endl;
        return false;
    }

    int chosen_preset_index = start_index + (rand() % amount);
    
    std::string piece_array_string;
    if(!presets.goto_line(chosen_preset_index) || !presets.read_line(piece_array_string))
    {
        std::cerr << "Error: Can't read line " << chosen_preset_index << " in kingpresets.json" << std::endl;
        return false;
    }

    json pieces;
    try {
        pieces = json::parse(piece_array_string);
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing piece array " << e.what() << std::endl;
        std::cerr << "Read line: " << piece_array_string << std::endl;
        return false;
    }

    for (auto p : pieces)
    {
        auto piece_type = p.at("piece");
        int column = p.at("x");
        int row = p.at("y");

        board.add_piece(make_board_object(piece_type, false, column, row));
    }

    //if (btn_start) btn_start->setPosition(static_cast<sf::Vector2f>(window->getSize()) - sf::Vector2f(100, 100));

    return true;
}

void StateGameplay::dropped_inst()
{
    if (!selected_inst) return;

    inventory.erase(std::find(inventory.begin(), inventory.end(), selected_inst->get_type()));
    load_instanciators();
}

void StateGameplay::returned_piece()
{
    if (!selected_piece) return;
    if (selected_piece->piece->get_team() != true) return;

    inventory.push_front(selected_piece->piece->get_piece_type());
    board.remove_piece(selected_piece);
    
    load_instanciators();
}

void StateGameplay::end_fight(PlayerType winner)
{
    actual_phase = PhaseType::Preparing;

    if (winner == PlayerType::P1)
    {
        auto elements = board.get_elements();
        for (auto element : elements)
        {
            if (element->piece->get_team()) inventory.push_front(element->piece->get_piece_type());
        }
        round++;
        if (difficulty <= 5 && round % 5 == 0) difficulty++;

        if (round % 3 == 1)
        {
            if (inventory.size() <= 10) inventory.push_front(enemy_king);
            PieceType new_enemy_king;
            do
            {
                int index = rand() % 9;
                new_enemy_king = static_cast<PieceType>(index);
            } while (new_enemy_king == enemy_king);
            enemy_king = new_enemy_king;
        }

        //Mostrar pantalla de victoria, tal vez
        player_turn = true;

        board.clear();
        set_up_black_team();
        this->adjust_elements();
    }
    else if (winner == PlayerType::P2)
    {   
        //Mostrar pantalla de derrota, pedir registrar tu nombre para el statsy luego esto
        board.clear();

        go_to = StateType::Return;
    }
}

void StateGameplay::load_instanciators()
{
    if (actual_phase == PhaseType::Preparing)
    {
        int Ymultiplier = 1;

        bool even_piece = false;

        instantiators.clear();

        for (auto piecetype : inventory)
        {
            instantiators.push_back(std::make_shared<PieceInstantiator>(piecetype, true, sf::Vector2f(xmargin + ((even_piece) ? xoffset : 0), ymargin + (Ymultiplier*yoffset))));
            
            if (even_piece) Ymultiplier++;
            even_piece = !even_piece;
        }
    }
}
