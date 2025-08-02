#include <iostream>

#include "../models/User.h"
#include "../utils/UserFileHelper.h"
#include "../lib/nlohmann/json.hpp"
#include "../../utils/hash.h"
#include "../../data/DataStore.h"
#include "../../utils/console.h"
using namespace std;
using json = nlohmann::json;

User handleLogin()
{
    string username, password;

    printTitle("ĐĂNG NHẬP");
    username = input("Tài khoản: ");

    string content = UserFileHelper::readStringFromFile(username + ".json", FileCategory::User);
    if (content.empty())
    {
        print("Thất bại. Không tìm thấy thông tin người dùng trong hệ thống.", true, ColorEnum::Red);
        return User(username, "", UserRole::Failed, username);
    }

    try
    {
        json j = json::parse(content);
        string storedPassword = j.value("password", "");

        while (true)
        {
            password = input("Mật khẩu: ");
            if (PasswordUtils::verifyPassword(password, storedPassword))
            {
                UserRole role = static_cast<UserRole>(j.value("role", 1));
                string displayName = j.value("displayName", "");
                string walletId = j.value("walletId", "");
                string phone = j.value("phoneNumber", "");
                bool isAutoPassword = j.value("isAutoPassword", false);

                User user(username, storedPassword, role, displayName, walletId, phone, isAutoPassword);
                print("Đăng nhập thành công!", true, ColorEnum::Green);

                if (user.getIsAutoPassword())
                {
                    print("Vui lòng thiết lập mật khẩu.", true, ColorEnum::Yellow);
                    string newPass;
                    do
                    {
                        newPass = input("Nhập khẩu mới: ");
                    } while (newPass.empty());

                    user.setPassword(newPass);
                    user.setIsAutoPassword(false);
                    DataStore::syncUser(user);
                    print("Thiết lập mật khẩu thành công.", true, ColorEnum::Green);
                }

                return user;
            }
            else
            {
                print("Sai mật khẩu.", true, ColorEnum::Red);
                string choice;
                do
                {
                    choice = input("Nhấn 1 để nhập lại mật khẩu, hoặc 0 để quay lại: ", ColorEnum::Yellow);
                } while (choice != "1" && choice != "0");

                if (choice == "0")
                {
                    print("Quay lại trang trước.", true);
                    return User(username, "", UserRole::Failed, username);
                }
                // Nếu chọn 1 thì vòng lặp sẽ tiếp tục
            }
        }
    }
    catch (...)
    {
        print("Hệ thống bị lỗi, vui lòng thử lại sau.", true, ColorEnum::Red);
        return User(username, "", UserRole::Failed, username);
    }
}
