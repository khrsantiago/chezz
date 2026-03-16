#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>   
#include "../include/json.hpp" 

using json = nlohmann::json;

class Stats
{
public:
    Stats();

    Stats(int score, int t_rounds);

    Stats(std::string name, int score, int t_rounds);
    
    ~Stats();

    std::string get_name() const;
    int get_score() const;
    int get_t_rounds() const;

    static Stats load_by_name(std::string name_to_find, std::string filepath = "stats.json");

    void save_or_update(std::string filepath = "stats.json");

    static std::vector<Stats> load_all(std::string filepath = "stats.json");
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Stats, name, score, total_rounds)

private:
    std::string name;
    int score;
    int total_rounds;
    std::string generate_random_name();
};