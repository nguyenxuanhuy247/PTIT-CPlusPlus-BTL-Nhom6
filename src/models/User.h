/**
 * USER_H là một include guard – dùng để ngăn không cho file header bị include nhiều lần trong quá trình biên dịch.
 */
#ifndef USER_H // Nếu USER_H chưa được định nghĩa
#define USER_H // Định nghĩa USER_H để tránh include nhiều lần

#include <string>
#include "../include/secuirty/hash.h"

// Enum đại diện cho vai trò người dùng
enum class UserRole
{
    Failed, // Không hợp lệ hoặc chưa đăng nhập
    User,   // Người dùng bình thường
    Manager // Người quản lý
};

// Lớp User lưu thông tin người dùng
class User
{
private:
    std::string username;    // Tên đăng nhập
    std::string displayName; // Tên hiển thị
    std::string password;    // Mật khẩu (đã hash)
    UserRole role;           // Vai trò người dùng
    std::string walletId;    // Mã ví điểm
    std::string phoneNumber; // Số điện thoại
    bool isAutoPassword;     // Mật khẩu tạo tự động hay không

public:
    // Constructor đầy đủ với tham số mặc định
    User(const std::string &username, const std::string &password,
         UserRole role = UserRole::Failed,
         const std::string &displayName = "",
         const std::string &walletId = "",
         const std::string &phoneNumber = "",
         bool isAutoPassword = false)
        : username(username), displayName(displayName), password(password),
          role(role), walletId(walletId), phoneNumber(phoneNumber), isAutoPassword(isAutoPassword) {}

    // Constructor mặc định (không có tham số)
    User() : username(""), displayName(""), password(""),
             role(UserRole::Failed), walletId(""), phoneNumber(""), isAutoPassword(false) {}

    // -------- Getter: Trả về giá trị các thuộc tính --------
    std::string getUsername() const { return username; }       // Lấy username
    std::string getDisplayName() const { return displayName; } // Lấy displayName
    std::string getPassword() const { return password; }       // Lấy password
    UserRole getRole() const { return role; }                  // Lấy vai trò
    std::string getWalletId() const { return walletId; }       // Lấy mã ví
    std::string getPhoneNumber() const { return phoneNumber; } // Lấy số điện thoại
    bool getIsAutoPassword() const { return isAutoPassword; }  // Kiểm tra có phải mật khẩu tự động không

    // -------- Setter: Gán giá trị cho các thuộc tính --------
    void setUsername(const std::string &newUsername) { username = newUsername; }                              // Gán username
    void setDisplayName(const std::string &newDisplayName) { displayName = newDisplayName; }                  // Gán displayName
    void setPassword(const std::string &newPassword) { password = PasswordUtils::hashPassword(newPassword); } // Gán password đã mã hóa
    void setRole(UserRole newRole) { role = newRole; }                                                        // Gán vai trò
    void setWalletId(const std::string &newWalletId) { walletId = newWalletId; }                              // Gán mã ví
    void setPhoneNumber(const std::string &newPhone) { phoneNumber = newPhone; }                              // Gán số điện thoại
    void setIsAutoPassword(bool value) { isAutoPassword = value; }                                            // Gán trạng thái mật khẩu tự động

    // -------- Kiểm tra vai trò người dùng --------
    bool isManager() const { return role == UserRole::Manager; } // Có phải manager không
    bool isUser() const { return role == UserRole::User; }       // Có phải user không
    bool isFailed() const { return role == UserRole::Failed; }   // Có phải failed không
};

#endif // USER_H            // Kết thúc chặn include nhiều lần
