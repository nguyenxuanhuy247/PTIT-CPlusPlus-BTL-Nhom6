#include "../models/User.h"
#include "../models/Wallet.h"
#include "../core/transaction/transaction.h"
#include "../utils/UserFileHelper.h"
#include "../utils/hash.h"
#include "../utils/factory.h"
#include "../models/Config.h"
#include "../utils/DataStore.h"

#include <iostream>

using namespace DataStore;

void insertSeedData() {
    // === Admin không có ví ===
    std::string username1 = "admin";
    std::string phone1 = "099";
    std::string password1 = PasswordUtils::hashPassword("123");
    User user1(username1, password1, UserRole::Manager, username1, "", phone1);

    // === Người dùng thường có ví ===
    std::string username2 = "daolinh1";
    std::string phone2 = "097";
    std::string password2 = PasswordUtils::hashPassword("123");
    Wallet wallet2;
    User user2(username2, password2, UserRole::User, username2, wallet2.getWalletId(), phone2);

    std::string username3 = "daolinh";
    std::string phone3 = "098";
    std::string password3 = PasswordUtils::hashPassword("123");
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
    Wallet* sys = getWalletById(SYSTEM_WALLET_ID);
    Wallet* w2 = getWalletById(wallet2.getWalletId());
    Wallet* w3 = getWalletById(wallet3.getWalletId());

        // === Thông báo kết quả ===
    if (ok1 && ok2 && ok3 && ok4 && ok5 && ok6) {
        print("==> Seed data thanh cong!", true);
        print("Admin: " + username1 + " | Pass: 123 | Role: Manager (khong co vi)", true);
        print("User2: " + username2 + " | Pass: 123 | Diem: " + std::to_string(w2->getPoints()), true);
        print("User3: " + username3 + " | Pass: 123 | Diem: " + std::to_string(w3->getPoints()), true);
        print("Vi tong: " + SYSTEM_WALLET_ID + " | Diem con lai: " + std::to_string(sys->getPoints()), true);
    } else {
        std::cerr << "\n\n==> Seed data that bai.\n";
    }

    if (sys && w2 && w3) {
        if (sys->deductPoints(2000)) {
            w2->addPoints(2000);
            recordTransaction(Transaction(TransactionType::Deposit, SYSTEM_WALLET_ID, w2->getWalletId(), 2000));
            print("Vi tong cap 2000 diem cho " + username2 + " [" + w2->getWalletId() + "]", true);
        } else {
            print("Vi tong khong du diem de cap cho " + username2, true);
        }

        if (sys->deductPoints(1000)) {
            w3->addPoints(1000);
            recordTransaction(Transaction(TransactionType::Deposit, SYSTEM_WALLET_ID, w3->getWalletId(), 1000));
            print("Vi tong cap 1000 diem cho " + username3 + " [" + w3->getWalletId() + "]", true);
        } else {
            print("Vi tong khong du diem de cap cho " + username3, true);
        }

        // Ghi đè ví sau cập nhật điểm
        UserFileHelper::saveUpdatedWallet(*sys);
        UserFileHelper::saveUpdatedWallet(*w2);
        UserFileHelper::saveUpdatedWallet(*w3);

        print("User2: " + username2 + " | Pass: 123 | Diem: " + std::to_string(w2->getPoints()), true);
        print("User3: " + username3 + " | Pass: 123 | Diem: " + std::to_string(w3->getPoints()), true);
    }


}
