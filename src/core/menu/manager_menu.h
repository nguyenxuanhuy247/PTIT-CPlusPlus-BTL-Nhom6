#ifndef MANAGER_MENU_H
#define MANAGER_MENU_H

#include <string>
#include "../models/User.h"

// Hàm hiển thị menu chính cho người quản lý (manager).
void showManagerMenu(User currentUser);

// Hàm in thông báo ra console, có thể xuống dòng hoặc không.
void print(const std::string& message, bool newline = true);

// Hàm nhập chuỗi từ người dùng.
std::string input(const std::string& prompt);

#endif // MANAGER_MENU_H
