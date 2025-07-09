#include "register.h"
#include "factory.h"
#include "../entities/User.h"
#include "../entities/Wallet.h"
#include "../include/UserFileHelper.h"
#include "../include/DataStore.h"
#include "../lib/nlohmann/json.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

using json = nlohmann::json;
using namespace DataStore;

bool usernameExists(const std::string& username) {
    for (const auto& u : allUsers) {
        if (u.getUsername() == username) return true;
    }
    return false;
}

bool phoneExists(const std::string& phone) {
    for (const auto& u : allUsers) {
        if (u.getPhoneNumber() == phone) return true;
    }
    return false;
}

void registerNewUser(bool calledByAdmin)  {
    std::string username;
    std::string phone;
    std::string rawPassword;
    bool isAutoPass;

    do {
        username = input("Tao ten dang nhap: ");
        if (username.empty()) {
            print("Username khong duoc de trong.", true);
            continue;
        }
        if (usernameExists(username)) {
            print("Username da ton tai. Hay nhap lai.", true);
        } else break;
    } while (true);

    do {
        phone = input("Nhap so dien thoai: ");
        if (phone.empty()) {
            print("So dien thoai khong duoc de trong.", true);
            continue;
        }
        if (phoneExists(phone)) {
            print("So dien thoai da ton tai. Hay nhap lai.", true);
        } else break;
    } while (true);

    if (!calledByAdmin)
    {
            rawPassword = input("Nhap mat khau (hoac de trong): ");
    }

    if (rawPassword.empty())
    {
        rawPassword = random4Digits();
        isAutoPass = true;
    }
    
    std::string hashedPassword = PasswordUtils::hashPassword(rawPassword);

    Wallet wallet;
    std::string walletId = wallet.getWalletId();

    User newUser(username, hashedPassword, UserRole::User, username, walletId, phone, isAutoPass);

    bool okUser = UserFileHelper::saveNewUser(newUser);
    bool okWallet = UserFileHelper::saveNewWallet(wallet);

    if (okUser && okWallet) {
        allUsers.push_back(newUser);
        allWallets[walletId] = wallet;

        if (calledByAdmin) {
            // bạn có thể log lại người tạo là admin nếu cần
            print("Tai khoan nay duoc tao boi quan ly.", true);
        }
        print("Dang ky thanh cong! Mat khau cua tai khoan (" + username + ") la: " + rawPassword, true);
        print("Tro ve man hinh truoc.", true);
    } else {
        print("Dang ky that bai.", true);
    }
}
