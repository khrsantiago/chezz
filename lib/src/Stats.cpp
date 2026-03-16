#include "Stats.hpp"
#include <iomanip>

Stats::Stats() : name("Unknown"), score(0), total_rounds(0) {}

Stats::Stats(int _score, int _t_rounds)
{
    score = _score;
    total_rounds = _t_rounds;
    name = generate_random_name();
}

Stats::Stats(std::string _name, int _score, int _t_rounds)
{
    name = _name;
    score = _score;
    total_rounds = _t_rounds;
}

Stats::~Stats() {}

std::string Stats::get_name() const { return name; }
int Stats::get_score() const { return score; }
int Stats::get_t_rounds() const { return total_rounds; }

std::string Stats::generate_random_name()
{
    const std::vector<std::string> adjetivos = {
        "Cyber", "Mega", "Super", "Neon", "Turbo", "Dark", "Iron", "Fast", "Epic", "Pixel"
    };
    
    const std::vector<std::string> sustantivos = {
        "Player", "Tetris", "Block", "Ninja", "Warrior", "Ghost", "Master", "King", "Bot", "Rider"
    };

    static bool seeded = false;
    if(!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    int adj_idx = std::rand() % adjetivos.size();
    int sus_idx = std::rand() % sustantivos.size();
    int numero = std::rand() % 1000;

    return adjetivos[adj_idx] + "_" + sustantivos[sus_idx] + "_" + std::to_string(numero);
}

Stats Stats::load_by_name(std::string name_to_find, std::string filepath)
{
    std::ifstream i(filepath);
    json j_array;

    if (i.is_open()) 
    {
        try {
            i >> j_array;
        } catch (json::parse_error& e) 
        {
            j_array = json::array(); 
        }
        i.close();
    }

    if (j_array.is_array()) 
    {
        for (const auto& element : j_array) 
        {
            Stats s = element.template get<Stats>();
            if (s.get_name() == name_to_find) 
            {
                return s;
            }
        }
    }

    std::cout << "Usuario " << name_to_find << " no encontrado." << std::endl;
    return Stats("Unknown", 0, 0);
}

void Stats::save_or_update(std::string filepath)
{
    std::ifstream i(filepath);
    json j_array;

    if (i.is_open()) 
    {
        try {
            if (i.peek() != std::ifstream::traits_type::eof()) 
            {
                i >> j_array;
            } else {
                j_array = json::array();
            }
        } catch (json::parse_error& e) 
        {
            j_array = json::array();
        }
        i.close();
    } 
    else 
    {
        j_array = json::array();
    }

    if (!j_array.is_array()) {
        j_array = json::array();
    }
    
    bool found = false;
    json current_obj = *this; 

    for (auto& element : j_array) 
    {
        if (element["name"] == this->name) 
        {
            element = current_obj;
            found = true;
            break;
        }
    }

    if (!found) 
    {
        j_array.push_back(current_obj);
    }

    std::ofstream o(filepath);
    o << std::setw(4) << j_array << std::endl;
    o.close();
}

std::vector<Stats> Stats::load_all(std::string filepath)
{
    std::vector<Stats> all_stats;
    std::ifstream i(filepath);
    json j_array;

    if (i.is_open())
    {
        try {
            if (i.peek() != std::ifstream::traits_type::eof()) 
                 i >> j_array;
        } catch (...) {} 
        i.close();
    }

    if (j_array.is_array()) 
    {
        for (const auto& element : j_array) 
        {
            all_stats.push_back(element.template get<Stats>());
        }
    }
    return all_stats;
}