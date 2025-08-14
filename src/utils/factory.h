#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <iostream>

#ifdef _WIN32
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <windows.h>
#else
  // Nếu không phải Windows, đặt các hằng để compile (giá trị chỉ cần tồn tại)
  #ifndef FOREGROUND_RED
  #define FOREGROUND_RED 1
  #endif
  #ifndef FOREGROUND_GREEN
  #define FOREGROUND_GREEN 2
  #endif
  #ifndef FOREGROUND_BLUE
  #define FOREGROUND_BLUE 4
  #endif
  #ifndef FOREGROUND_INTENSITY
  #define FOREGROUND_INTENSITY 8
  #endif
#endif

// === Đồng bộ tên enum với console.cpp ===
enum class ColorEnum {
    Default = 7,
    Red     = FOREGROUND_RED   | FOREGROUND_INTENSITY,
    Green   = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Yellow  = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Blue    = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    Cyan    = FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    Magenta = FOREGROUND_RED   | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    White   = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE
};


using ConsoleColor = ColorEnum;

void resetColor();
void setColor(ColorEnum color);

void print(const std::string& message,
           bool addNewLine = true,
           ColorEnum color = ColorEnum::Default);

std::string input(const std::string& prompt,
                  ColorEnum color = ColorEnum::Default);

void printTitle(const std::string& text,
                ColorEnum color = ColorEnum::Cyan);

void pause(ColorEnum color = ColorEnum::Yellow);

std::string random4Digits();

#endif // FACTORY_H
