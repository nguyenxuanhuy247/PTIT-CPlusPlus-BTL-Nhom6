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

void initSampleData()
{
    // Khởi tạo tài khoản admin, admin không có ví
    std::string username1 = "admin";
    std::string phone1 = "0980000001";
    std::string password1 = PasswordUtils::hashPassword("123456");
    User user1(username1, password1, UserRole::Manager, username1, "", phone1);

    // Khởi tạo tài khoản người dùng 1
    std::string username2 = "user123";
    std::string phone2 = "0980000002";
    std::string password2 = PasswordUtils::hashPassword("123456");
    Wallet wallet2;
    User user2(username2, password2, UserRole::User, username2, wallet2.getWalletId(), phone2);

    // Khởi tạo tài khoản người dùng 2
    std::string username3 = "user456";
    std::string phone3 = "0980000003";
    std::string password3 = PasswordUtils::hashPassword("123456");
    Wallet wallet3;
    User user3(username3, password3, UserRole::User, username3, wallet3.getWalletId(), phone3);

    // === Tạo ví tổng với 10000 điểm ===
    Wallet systemWallet(SYSTEM_WALLET_ID, 10000);

    // === Ghi vào file ===
    bool ok1 = UserFileHelper::saveNewUser(user1);
    bool ok2 = UserFileHelper::saveNewUser(user2);
    bool ok3 = UserFileHelper::saveNewWallet(wallet2);
    bool ok4 = UserFileHelper::saveNewUser(user3);
    bool ok5 = UserFileHelper::saveNewWallet(wallet3);
    bool ok6 = UserFileHelper::saveNewWallet(systemWallet);

    // ✅ Load lại vào RAM
    DataStore::loadAllWallets();

    // === Giao dịch: ví tổng cấp điểm cho user ===
    Wallet *sys = getWalletById(SYSTEM_WALLET_ID);
    Wallet *w2 = getWalletById(wallet2.getWalletId());
    Wallet *w3 = getWalletById(wallet3.getWalletId());

    // === Thông báo kết quả ===
    if (ok1 && ok2 && ok3 && ok4 && ok5 && ok6)
    {
        print("Khởi tạo dữ liệu thành công!", false, ColorEnum::Green);
        // printTitle("DỮ LIỆU MẪU");
        // print("1. Quản trị viên (admin):", true);
        // print("- Tài khoản: " + username1, true);
        // print("- Mật khẩu: 123456", true);
        // print("- Admin (khong co vi)", true);
        // print("2. Người dùng 1 (user1):", true);
        // print("- Tài khoản: " + username2, true);
        // print("- Mật khẩu: 123456", true);
        // print("Điểm: " + std::to_string(w2->getPoints()), true);
        // print("3. Người dùng 2 (user2):", true);
        // print("Tài khoản: " + username3, true);
        // print("Mật khẩu: 123456", true);
        // print("Điểm: " + std::to_string(w3->getPoints()), true);

        // print("Ví tổng: " + SYSTEM_WALLET_ID + " | Điểm còn lại: " + std::to_string(sys->getPoints()), true, ColorEnum::Yellow);
    }
    else
    {
        print("Khởi tạo dữ liệu thiết bại.\n", true, ColorEnum::Red);
    }

    if (sys && w2 && w3)
    {
        if (sys->deductPoints(2000))
        {
            w2->addPoints(2000);
            recordTransaction(Transaction(TransactionType::Deposit, SYSTEM_WALLET_ID, w2->getWalletId(), 2000));
            // print("Vi tong cap 2000 diem cho " + username2 + " [" + w2->getWalletId() + "]", true);
        }
        else
        {
            // print("Vi tong khong du diem de cap cho " + username2, true);
        }

        if (sys->deductPoints(1000))
        {
            w3->addPoints(1000);
            recordTransaction(Transaction(TransactionType::Deposit, SYSTEM_WALLET_ID, w3->getWalletId(), 1000));
            // print("Vi tong cap 1000 diem cho " + username3 + " [" + w3->getWalletId() + "]", true);
        }
        else
        {
            // print("Vi tong khong du diem de cap cho " + username3, true);
        }

        // Ghi đè ví sau cập nhật điểm
        UserFileHelper::saveUpdatedWallet(*sys);
        UserFileHelper::saveUpdatedWallet(*w2);
        UserFileHelper::saveUpdatedWallet(*w3);

        // print("User2: " + username2 + " | Pass: 123 | Diem: " + std::to_string(w2->getPoints()), true);
        // print("User3: " + username3 + " | Pass: 123 | Diem: " + std::to_string(w3->getPoints()), true);
    }
}
