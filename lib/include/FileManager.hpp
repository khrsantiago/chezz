#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <iostream>

class FileManager
{
private:
    std::fstream filestream;

public:
    FileManager() = delete;
    FileManager(std::string);
    ~FileManager();
    bool append_line(std::string);
    bool read_line(std::string&);
    bool write_line(std::string);
    bool read_char(char&);
    bool goto_start();
    bool goto_end();
    bool goto_line(int);
    bool goto_char_inline(int);
    bool goto_char(int);
};
