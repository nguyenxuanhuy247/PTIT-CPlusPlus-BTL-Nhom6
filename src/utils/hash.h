#ifndef HASH_H
#define HASH_H

#include <string>

/**
 * Khai báo namespace PasswordUtils để gom nhóm các hàm liên quan đến xử lý mật khẩu.
 * namespace - dùng để nhóm các hàm, biến, lớp lại với nhau để tránh trùng tên với các phần khác trong chương trình.
 */
namespace PasswordUtils
{
    // Hàm hashPassword: Nhận vào chuỗi mật khẩu thuần (raw password) và trả về chuỗi đã được mã hóa (hashed password)
    std::string hashPassword(const std::string &raw);

    // Hàm verifyPassword:
    // So sánh mật khẩu người dùng nhập vào (raw) với mật khẩu đã lưu (hashed).
    // Trả về true nếu khớp, ngược lại là false.
    bool verifyPassword(const std::string &raw, const std::string &hashed);

    bool isValidPassword(const std::string &raw);

}

// Kết thúc phần bảo vệ định danh tránh include lặp
#endif
