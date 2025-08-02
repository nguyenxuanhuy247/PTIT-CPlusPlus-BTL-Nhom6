#include <iostream>
#include <string>
#include <limits>
#include "./console.h"

using namespace std;

// Phần định nghĩa màu sắc cho từng hệ điều hành
#ifdef _WIN32
// Thiết lập màu chữ trên Windows
void setColor(ColorEnum color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);           // Lấy handle của console
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color)); // Áp dụng màu
}
#else
// Thiết lập màu chữ trên macOS/Linux dùng ANSI escape codes
void setColor(ColorEnum color)
{
    switch (color)
    {
    case ColorEnum::Red:
        cout << "\033[1;31m"; // Màu đỏ đậm
        break;
    case ColorEnum::Green:
        cout << "\033[1;32m"; // Màu xanh lá đậm
        break;
    case ColorEnum::Yellow:
        cout << "\033[1;33m"; // Màu vàng đậm
        break;
    case ColorEnum::Blue:
        cout << "\033[1;34m"; // Màu xanh dương đậm
        break;
    case ColorEnum::Cyan:
        cout << "\033[1;36m"; // Màu cyan đậm
        break;
    case ColorEnum::Magenta:
        cout << "\033[1;35m"; // Màu magenta đậm
        break;
    case ColorEnum::White:
        cout << "\033[1;37m"; // Màu trắng đậm
        break;
    default:               // Màu mặc định
        cout << "\033[0m"; // Reset về màu ban đầu
        break;
    }
}
#endif

// Reset màu về mặc định
void resetColor()
{
    setColor(ColorEnum::Default);
}

// In thông báo ra console với màu sắc
void print(const string &message, bool addNewLine, ColorEnum color)
{
    setColor(color); // Thiết lập màu
    cout << message; // In nội dung
    if (addNewLine)
    {
        cout << endl; // Xuống dòng nếu cần
    }
    resetColor(); // Reset lại màu
}

// In tiêu đề với định dạng đặc biệt
void printTitle(const string &text, ColorEnum color)
{
    setColor(color);
    cout << "\n===> " << text << " <===\n";
    resetColor();
}

// Nhập dữ liệu từ người dùng với prompt có màu
string input(const string &prompt, ColorEnum color)
{
    string value;
    setColor(color);
    cout << prompt; // Hiển thị prompt với màu
    resetColor();

    // Xử lý nhập liệu đa nền tảng
    getline(cin, value); // Đọc dòng đầu tiên

    // Nếu dòng đầu rỗng (do trước đó có dùng cin >>)
    if (value.empty())
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bỏ qua ký tự thừa
        getline(cin, value);                                 // Đọc lại
    }
    return value;
}

// Tạm dừng chương trình chờ nhấn Enter
void pause(ColorEnum color)
{
    setColor(color);
    cout << "Press Enter to continue...";
    resetColor();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Đợi đến khi nhấn Enter
}

// Tạo chuỗi ngẫu nhiên 4 chữ số
string random4Digits()
{
    srand(static_cast<unsigned int>(time(nullptr))); // Khởi tạo seed ngẫu nhiên
    const int num = rand() % 9000 + 1000;            // Số ngẫu nhiên từ 1000 đến 9999
    return to_string(num);                           // Chuyển thành chuỗi
}