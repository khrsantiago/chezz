#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

enum class SoundType
{
    MovePiece,
    Victory,
    Defeat
};

class SoundManager
{
private:
    static std::vector<std::unique_ptr<sf::Sound>> active_sounds;
    static std::map<std::string, sf::SoundBuffer> sound_buffers;
    static sf::SoundBuffer load_file(std::string route);
    static void clean_finished_sounds();
public:
    static void init();
    static void play(SoundType type);
};

