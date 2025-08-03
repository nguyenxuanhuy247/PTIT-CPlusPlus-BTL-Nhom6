#include "register.h"
#include "../utils/console.h"
#include "../models/User.h"
#include "../models/Wallet.h"
#include "../utils/userFileHelper.h"
#include "../data/dataStore.h"
#include "../lib/nlohmann/json.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

using json = nlohmann::json;
using namespace DataStore;

bool usernameExists(const std::string &username)
{
    for (const auto &u : allUsers)
    {
        if (u.getUsername() == username)
            return true;
    }
    return false;
}

bool phoneExists(const std::string &phone)
{
    for (const auto &u : allUsers)
    {
        if (u.getPhoneNumber() == phone)
            return true;
    }
    return false;
}

void registerNewUser(bool calledByAdmin)
{
    std::string username;
    std::string phone;
    std::string rawPassword;
    bool isAutoPass = false;

    printTitle("ĐĂNG KÝ");

    do
    {
        username = input("Tài khoản: ");
        if (username.empty())
        {
            print("Tên tài khoản không được để trống.", true, ColorEnum::Yellow);
            continue;
        }
        if (usernameExists(username))
        {
            print("Tên tài khoản đã tồn tại. Vui lòng nhập lại.", true, ColorEnum::Yellow);
        }
        else
            break;
    } while (true);

    do
    {
        phone = input("Số điện thoại: ");
        if (phone.empty())
        {
            print("Số điện thoại không được để trống.", true, ColorEnum::Yellow);
            continue;
        }
        if (phoneExists(phone))
        {
            print("Số điện thoại đã tồn tại. Vui lòng nhập lại.", true);
        }
        else
            break;
    } while (true);

    if (!calledByAdmin)
    {

        do
        {
            rawPassword = input("Mật khẩu: ");
            if (rawPassword.empty())
            {
                print("Mật khẩu không được để trống.", true, ColorEnum::Yellow);
                continue;
            }
            else
                break;
        } while (true);
    }

    if (rawPassword.empty())
    {
        rawPassword = generateRandomPassword();
        isAutoPass = true;
    }

    std::string hashedPassword = PasswordUtils::hashPassword(rawPassword);

    Wallet wallet;
    std::string walletId = wallet.getWalletId();

    User newUser(username, hashedPassword, UserRole::User, username, walletId, phone, isAutoPass);

    bool okUser = UserFileHelper::saveNewUser(newUser);
    bool okWallet = UserFileHelper::saveNewWallet(wallet);

    if (okUser && okWallet)
    {
        allUsers.push_back(newUser);
        allWallets[walletId] = wallet;

        if (calledByAdmin)
        {
            // bạn có thể log lại người tạo là admin nếu cần
            print("Mật khẩu của tài khoản " + username + " là: " + rawPassword, true);
        }
        print("Đăng ký tài khoản thành công.", true, ColorEnum::Green);
    }
    else
    {
        print("Dang ky that bai.", true);
    }
}
