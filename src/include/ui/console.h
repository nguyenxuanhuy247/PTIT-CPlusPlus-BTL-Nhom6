#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
// ANSI color codes for non-Windows platforms
#define FOREGROUND_RED     1
#define FOREGROUND_GREEN   2
#define FOREGROUND_BLUE    4
#define FOREGROUND_INTENSITY 8
#endif

enum class ConsoleColor
{
    Default = 7, // White on Windows, reset on others
    Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
    Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

// Function to set console color (platform specific)
void setConsoleColor(ConsoleColor color);

// Print function with color support
void print(const std::string &message, bool addNewLine = true, ConsoleColor color = ConsoleColor::Default);

// Input function with color support
std::string input(const std::string &prompt, ConsoleColor color = ConsoleColor::Default);

// Print title with special formatting
void printTitle(const std::string &text, ConsoleColor color = ConsoleColor::Cyan);

// Pause execution
void pause(ConsoleColor color = ConsoleColor::Yellow);

// Generate random 4-digit string
std::string random4Digits();

#endif