#ifndef FACTORY_H
#define FACTORY_H
#define NOMINMAX
#include <windows.h>
#include <string>

enum class ConsoleColor {
    Default = 7, // Trắng
    Red     = FOREGROUND_RED | FOREGROUND_INTENSITY,
    Green   = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Yellow  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Blue    = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    Cyan    = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    White   = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

void print(const std::string& message, bool addNewLine = true, ConsoleColor color = ConsoleColor::Default);
std::string input(const std::string& prompt, ConsoleColor color = ConsoleColor::Default);
void printTitle(const std::string& text, ConsoleColor color = ConsoleColor::Cyan);
void pause(ConsoleColor color = ConsoleColor::Yellow);
std::string random4Digits();

#endif
