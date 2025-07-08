#include <iostream>
#include <string>
#include <limits>
#include "../include/ui/console.h"

using namespace std;

#ifdef _WIN32
void setColor(ConsoleColor color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}
#else
void setColor(ConsoleColor color)
{
    // ANSI escape codes for colors
    switch (color)
    {
        case ConsoleColor::Red:
            cout << "\033[1;31m";
            break;
        case ConsoleColor::Green:
            cout << "\033[1;32m";
            break;
        case ConsoleColor::Yellow:
            cout << "\033[1;33m";
            break;
        case ConsoleColor::Blue:
            cout << "\033[1;34m";
            break;
        case ConsoleColor::Cyan:
            cout << "\033[1;36m";
            break;
        case ConsoleColor::Magenta:
            cout << "\033[1;35m";
            break;
        case ConsoleColor::White:
            cout << "\033[1;37m";
            break;
        default: // Default
            cout << "\033[0m";
            break;
    }
}
#endif

void resetColor()
{
    setColor(ConsoleColor::Default);
}

void print(const string &message, bool addNewLine, ConsoleColor color)
{
    setColor(color);
    cout << message;
    if (addNewLine)
    {
        cout << endl;
    }
    resetColor();
}

void printTitle(const string &text, ConsoleColor color)
{
    setColor(color);
    cout << "\n=== " << text << " ===\n";
    resetColor();
}

string input(const string &prompt, ConsoleColor color)
{
    string value;
    setColor(color);
    cout << prompt;
    resetColor();

    // Xử lý đa nền tảng cho việc nhập liệu
    getline(cin, value);
    if (value.empty()) {
        // Nếu getline không lấy được dữ liệu (do trước đó có cin >> something)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, value);
    }
    return value;
}

void pause(ConsoleColor color)
{
    setColor(color);
    cout << "Press Enter to continue...";
    resetColor();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string random4Digits()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int num = rand() % 9000 + 1000; // Random number between 1000 and 9999
    return to_string(num);
}