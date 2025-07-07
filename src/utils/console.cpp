#include <iostream>
#include "../include/ui/console.h"

using namespace std;

void setColor(ConsoleColor color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}

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
    getline(cin >> ws, value);
    return value;
}
