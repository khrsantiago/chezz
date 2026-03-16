#include <FileManager.hpp>

FileManager::FileManager(std::string filename) : filestream(filename)
{}

FileManager::~FileManager()
{
    filestream.close();
}

bool FileManager::append_line(std::string line)
{
    if(!filestream.is_open())
        return false;

    filestream.seekp(0, std::ios::end);
    filestream << line << std::endl;
    filestream.flush();

    return true;
}

bool FileManager::read_line(std::string& line)
{
    if(!filestream.is_open())
        return false;

    if(std::getline(filestream, line))
    {
        if(filestream.eof())
            return false;

        return true;
    }

    return false;
}

bool FileManager::write_line(std::string line)
{
    if(!filestream.is_open())
        return false;

    filestream << line << std::endl;
    filestream.flush();

    return true;
}

bool FileManager::read_char(char& c)
{
    if(!filestream.is_open())
        return false;

    if(filestream.get(c))
        return true;

    return false;
}

bool FileManager::goto_start()
{
    if(!filestream.is_open())
        return false;

    filestream.seekp(0);
    
    return true;
}

bool FileManager::goto_end()
{
    if(!filestream.is_open())
        return false;
    
   filestream.seekp(0, std::ios::end);

    return true;
}

bool FileManager::goto_line(int number)
{
    if(!filestream.is_open())
        return false;

    filestream.seekp(0);

    int i = 0;
    while (i < number)
    {
        std::string _s;
        std::getline(filestream, _s);
        if(filestream.eof())
            return false;
        i++;
    }

    return true;
}

bool FileManager::goto_char_inline(int number)
{
    if(!filestream.is_open())
        return false;

    while (filestream.tellp() > 0)
    {
        if(filestream.peek() == '\n')
        {
            char _c;
            filestream.get(_c);
            break;
        }

        filestream.unget();
    }

    char c;
    int i = 0;
    while (i < number)
    {
        filestream.get(c);
        if(filestream.eof())
            return false;
        i++;
    }

    return true;
}

bool FileManager::goto_char(int number)
{
    if(!filestream.is_open())
        return false;

    //filestream.seekp(0);
    filestream.seekg(0);

    int i = 0;
    while (i < number)
    {
        char _c;
        filestream.get(_c);
        if(filestream.eof())
            return false;
        i++;
    }

    return true;
}
