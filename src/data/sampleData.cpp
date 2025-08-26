#include "../models/User.h"
#include "../models/Wallet.h"
#include "../core/transaction/transaction.h"
#include "../utils/UserFileHelper.h"
#include "../utils/hash.h"
#include "../utils/console.h"
#include "../models/Config.h"
#include "./DataStore.h"

#include <iostream>

using namespace DataStore;

/**
 * @usage Khởi tạo dữ liệu mẫu cho ứng dụng
 */
void initSampleData()
{
    // Khởi tạo tài khoản admin, admin không có ví
    std::string sysUsername = "admin";
    std::string sysPhoneNumber = "0980000001";
    std::string sysPassword = PasswordUtils::hashPassword("123456");
    User admin(sysUsername, sysPassword, UserRole::Manager, sysUsername, "", sysPhoneNumber);

    // Khởi tạo tài khoản người dùng 1
    std::string username1 = "nguyenvana";
    std::string phoneNumber1 = "0980000002";
    std::string password1 = PasswordUtils::hashPassword("123456");
    Wallet wallet1;
    User user1(username1, password1, UserRole::User, username1, wallet1.getWalletId(), phoneNumber1);

    // Khởi tạo tài khoản người dùng 2
    std::string username2 = "nguyenvanb";
    std::string phoneNumber2 = "0980000003";
    std::string password2 = PasswordUtils::hashPassword("123456");
    Wallet wallet2;
    User user2(username2, password2, UserRole::User, username2, wallet2.getWalletId(), phoneNumber2);

    // Khởi tạo ví tổng với 10,000 điểm
    Wallet systemWallet(SYSTEM_WALLET_ID, 10000);

    // Ghi dữ liệu vào file
    bool isSaveAdmin = UserFileHelper::saveNewUser(admin);
    bool isSaveSysWallet = UserFileHelper::saveNewWallet(systemWallet);
    bool isSaveUser1 = UserFileHelper::saveNewUser(user1);
    bool isSaveWallet1 = UserFileHelper::saveNewWallet(wallet1);
    bool isSaveUser2 = UserFileHelper::saveNewUser(user2);
    bool isSaveWallet2 = UserFileHelper::saveNewWallet(wallet2);

    // Lưu dữ liệu vào DataStore luôn
    DataStore::loadAllWallets();

    if (isSaveUser1 && isSaveWallet1 && isSaveUser2 && isSaveWallet2 && isSaveAdmin && isSaveSysWallet)
    {

        printTitle("DỮ LIỆU MẪU");
        print("Admin: " + sysUsername + " | Password: 123456", true);
        print("User1: " + username1 + " | Password: 123456" + " | SĐT: " + phoneNumber1 + " | Diem: " + std::to_string(wallet1.getPoints()), true);
        print("User2: " + username2 + " | Password: 123456" + " | SĐT: " + phoneNumber2 + " | Diem: " + std::to_string(wallet1.getPoints()), true);
        print("Ví tổng: " + std::to_string(systemWallet.getPoints()), true);
    }
    else
    {
        print("Khởi tạo dữ liệu thiết bại.\n", true, ColorEnum::Red);
    }
}
