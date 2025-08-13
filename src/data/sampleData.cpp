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
    std::string username1 = "admin";
    std::string phone1 = "0980000001";
    std::string password1 = PasswordUtils::hashPassword("123456");
    User user1(username1, password1, UserRole::Manager, username1, "", phone1);

    // === Tạo ví tổng với 10000 điểm ===
    Wallet systemWallet(SYSTEM_WALLET_ID, 10000);

    // === Ghi vào file ===
    bool isSaveAdmin = UserFileHelper::saveNewUser(user1);
    bool isSaveSysWallet = UserFileHelper::saveNewWallet(systemWallet);

    // ✅ Load lại vào RAM
    DataStore::loadAllWallets();

    // === Thông báo kết quả ===
    if (!(isSaveAdmin && isSaveSysWallet))
    {
        print("Khởi tạo dữ liệu thiết bại.\n", true, ColorEnum::Red);
    }
}
