/**
 * USER_H là một include guard – dùng để ngăn không cho file header bị include nhiều lần trong quá trình biên dịch.
 */
#ifndef USER_H // Nếu USER_H chưa được định nghĩa
#define USER_H // Định nghĩa USER_H để tránh include nhiều lần

#include <string>
#include "../utils/hash.h"

// Enum đại diện cho vai trò người dùng
enum class UserRole
{
    Guest,  // Người dùng chưa đăng nhập
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
    // Constructor mặc định (không có tham số)
    User() : username(""), displayName(""), password(""),
             role(UserRole::Guest), walletId(""), phoneNumber(""), isAutoPassword(false) {}

    // Constructor đầy đủ với tham số mặc định
    User(const std::string &username,
         const std::string &password,
         UserRole role = UserRole::Guest,
         const std::string &displayName = "",
         const std::string &walletId = "",
         const std::string &phoneNumber = "",
         bool isAutoPassword = false)
        : username(username), displayName(displayName), password(password),
          role(role), walletId(walletId), phoneNumber(phoneNumber), isAutoPassword(isAutoPassword) {}

    // -------- Getter: Trả về giá trị các thuộc tính --------
    // const ở cuối chỉ để đảm bảo rằng hàm này không sửa đổi thuộc tính nào của User
    std::string getUsername() const
    {
        return username;
    }

    std::string getDisplayName() const
    {
        return displayName;
    }

    std::string getPassword() const
    {
        return password;
    }

    UserRole getRole() const
    {
        return role;
    }

    std::string getWalletId() const
    {
        return walletId;
    }

    std::string getPhoneNumber() const
    {
        return phoneNumber;
    }

    bool getIsAutoPassword() const
    {
        return isAutoPassword;
    }

    // -------- Setter: Gán giá trị cho các thuộc tính --------
    void setUsername(const std::string &newUsername)
    {
        username = newUsername;
    }
    void setDisplayName(const std::string &newDisplayName)
    {
        displayName = newDisplayName;
    }
    void setPassword(const std::string &newPassword)
    {
        password = PasswordUtils::hashPassword(newPassword);
    }
    void setRole(UserRole newRole)
    {
        role = newRole;
    }
    void setWalletId(const std::string &newWalletId)
    {
        walletId = newWalletId;
    }
    void setPhoneNumber(const std::string &newPhone)
    {
        phoneNumber = newPhone;
    }
    void setIsAutoPassword(bool value)
    {
        isAutoPassword = value;
    }

    // -------- Kiểm tra vai trò người dùng --------
    bool isManager() const
    {
        return role == UserRole::Manager;
    }
    bool isUser() const
    {
        return role == UserRole::User;
    }
    bool isGuest() const
    {
        return role == UserRole::Guest;
    }
};

#endif // USER_H - Kết thúc chặn include nhiều lần
