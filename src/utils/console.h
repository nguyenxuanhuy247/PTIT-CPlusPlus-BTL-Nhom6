#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <iostream>

// Phân biệt hệ điều hành để xử lý màu sắc
#ifdef _WIN32
#include <windows.h> // Thư viện Windows API để xử lý màu console
#else
// Định nghĩa mã màu ANSI cho các hệ điều hành không phải Windows
#define FOREGROUND_RED 1       // Màu đỏ cơ bản
#define FOREGROUND_GREEN 2     // Màu xanh lá cơ bản
#define FOREGROUND_BLUE 4      // Màu xanh dương cơ bản
#define FOREGROUND_INTENSITY 8 // Độ sáng (intensity)
#endif

// Enum định nghĩa các màu sắc cho console
enum class ColorEnum
{
    Default = 7,                                                       // Màu trắng trên Windows, reset màu trên hệ điều hành khác
    Red = FOREGROUND_RED | FOREGROUND_INTENSITY,                       // Màu đỏ sáng
    Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,                   // Màu xanh lá sáng
    Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // Màu vàng (đỏ + xanh lá)
    Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,                     // Màu xanh dương sáng
    Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,  // Màu cyan (xanh lá + xanh dương)
    Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Màu magenta (đỏ + xanh dương)
    White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE        // Màu trắng (tổng hợp 3 màu cơ bản)
};

/********************************************
 * CÁC HÀM XỬ LÝ CONSOLE ĐA NỀN TẢNG
 ********************************************/
/**
 * Reset lại về màu chữ của hệ thống là màu trắng
 */
void resetColor();
/**
 * Thiết lập màu chữ cho console
 * @param color: Màu sắc từ enum ColorEnum
 */
void setConsoleColor(ColorEnum color);

/**
 * In thông báo ra console với màu sắc
 * @param message: Nội dung cần in
 * @param addNewLine: Tự động xuống dòng nếu true
 * @param color: Màu chữ (mặc định là màu trắng/Default)
 */
void print(const std::string &message, bool addNewLine = true, ColorEnum color = ColorEnum::Default);

/**
 * Nhập dữ liệu từ người dùng với prompt có màu
 * @param prompt: Thông báo nhắc nhập
 * @param color: Màu chữ (mặc định là màu trắng/Default)
 * @return Chuỗi do người dùng nhập vào
 */
std::string input(const std::string &prompt, ColorEnum color = ColorEnum::Default);

/**
 * In tiêu đề với định dạng đặc biệt
 * @param text: Nội dung tiêu đề
 * @param color: Màu chữ (mặc định là màu cyan)
 */
void printTitle(const std::string &text, ColorEnum color = ColorEnum::Cyan);

/**
 * Tạm dừng chương trình chờ nhấn Enter
 * @param color: Màu thông báo (mặc định là màu vàng)
 */
void pause(ColorEnum color = ColorEnum::Yellow);

/**
 * Tạo chuỗi ngẫu nhiên gồm 4 chữ số
 * @return Chuỗi 4 chữ số ngẫu nhiên (từ 1000 đến 9999)
 */
std::string random4Digits();

#endif