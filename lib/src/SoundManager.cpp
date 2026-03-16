#include "SoundManager.hpp"

std::map<std::string, sf::SoundBuffer> SoundManager::sound_buffers;
std::vector<std::unique_ptr<sf::Sound>> SoundManager::active_sounds;

void SoundManager::init()
{
    

    std::string route = "assets/sounds/";
    sound_buffers["victory"] = load_file(route + "victory.ogg");
    sound_buffers["defeat"] = load_file(route + "defeat.ogg");
    sound_buffers["move_piece"] = load_file(route + "move_piece.ogg");
}

sf::SoundBuffer SoundManager::load_file(std::string route)
{
    sf::SoundBuffer new_buffer;
    if (!new_buffer.loadFromFile(route))
    {
        std::cerr << "Error loading file: " << route << std::endl;
    }
    return new_buffer;
}

void SoundManager::play(SoundType type)
{
    clean_finished_sounds();
    
    std::string sound_name;
    switch (type)
    {
    case SoundType::Victory:
        sound_name = "victory";
        break;
    case SoundType::Defeat:
        sound_name = "defeat";
        break;
    case SoundType::MovePiece:
    default:
        sound_name = "move_piece";
        break;
    }
    
    auto sound = std::make_unique<sf::Sound>(sound_buffers.at(sound_name));
    sound->play();
    active_sounds.push_back(std::move(sound));
}

void SoundManager::clean_finished_sounds()
{
    active_sounds.erase(
        std::remove_if(active_sounds.begin(), active_sounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) {
                return sound->getStatus() == sf::SoundSource::Status::Stopped;
            }),
        active_sounds.end()
    );
}