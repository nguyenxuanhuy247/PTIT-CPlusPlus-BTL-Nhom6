#include <iostream>

#include "../models/User.h"
#include "../include/UserFileHelper.h"
#include "../lib/nlohmann/json.hpp"
#include "../../include/security/hash.h"
#include "../include/DataStore.h"
#include "../../utils/console.h"

using namespace std;
using json = nlohmann::json;

User handleLogin()
{
    string username, password;

    printTitle("DANG NHAP HE THONG");
    username = input("Tai khoan: ");
    password = input("Mat khau: ");

    string content = UserFileHelper::readStringFromFile(username + ".json", FileCategory::User);
    if (content.empty())
    {
        print("Dang nhap that bai. Khong tim thay tai khoan.", true);
        return User(username, password, UserRole::Failed, username);
    }

    try
    {
        json j = json::parse(content);
        string storedPassword = j.value("password", "");

        if (PasswordUtils::verifyPassword(password, storedPassword))
        {
            UserRole role = static_cast<UserRole>(j.value("role", 1));
            string displayName = j.value("displayName", "");
            string walletId = j.value("walletId", "");
            string phone = j.value("phoneNumber", "");
            bool isAutoPassword = j.value("isAutoPassword", false);

            User user(username, storedPassword, role, displayName, walletId, phone, isAutoPassword);

            print("Dang nhap thanh cong!", true);

            // Nếu là mật khẩu tự sinh thì yêu cầu đổi
            if (user.getIsAutoPassword())
            {
                print("Ban dang su dung mat khau tu dong. Vui long doi mat khau ngay bay gio!", true);
                string newPass;
                do
                {
                    newPass = input("Nhap mat khau moi: ");
                } while (newPass.empty());

                // Cập nhật mật khẩu mới và gán isAutoPassword = false
                user.setPassword(newPass);
                user.setIsAutoPassword(false);
                DataStore::syncUser(user);

                print("Mat khau da duoc thay doi thanh cong.", true);
            }

            return user;
        }
        else
        {
            print("Sai mat khau.", true);
            return User(username, password, UserRole::Failed, username);
        }
    }
    catch (...)
    {
        print("Loi khi doc file tai khoan.", true);
        return User(username, password, UserRole::Failed, username);
    }
}