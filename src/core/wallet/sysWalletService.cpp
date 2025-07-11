#include "../models/User.h"
#include "../models/Wallet.h"
#include "../transaction/transaction.h"
#include "../utils/console.h"
#include "../utils/userFileHelper.h" // Đã sửa chữ hoa/thường
#include "../otp/otp.h"
#include "../data/dataStore.h" // Đã sửa chữ hoa/thường
#include "../models/Config.h"
#include <iostream>

using namespace DataStore;

bool issuePointsToWallet(const User adminUser, const std::string &toWalletId, int amount)
{
    if (!adminUser.isManager())
    {
        print("❌ Chi Manager moi co quyen cap diem!", true);
        return false;
    }

    Wallet *toWallet = getWalletById(toWalletId);
    Wallet *sysWallet = getWalletById(SYSTEM_WALLET_ID);

    if (!toWallet || !sysWallet)
    {
        print("Khong tim thay vi dich hoac vi tong.", true);
        return false;
    }

    if (!sysWallet->deductPoints(amount))
    {
        print("⚠️ Vi tong khong du diem.", true);
        return false;
    }

    toWallet->addPoints(amount);
    Transaction tx(TransactionType::Deposit, SYSTEM_WALLET_ID, toWalletId, amount);
    recordTransaction(tx);

    print("✅ Da cap " + std::to_string(amount) + " diem cho vi " + toWalletId, true);
    return true;
}

void showSystemWalletView(User adminUser)
{
    if (!adminUser.isManager())
    {
        print("❌ Chi Manager moi duoc truy cap chuc nang nay!", true);
        return;
    }

    Wallet *sysWallet = getWalletById(SYSTEM_WALLET_ID);
    if (!sysWallet)
    {
        print("⚠️ Vi tong khong ton tai.", true);
        return;
    }

    int choice = -1;
    do
    {
        print("\n===== MENU VI TONG =====", true);
        print("1. Cap diem cho nguoi dung", true);
        print("2. Xem tong so giao dich cua vi tong", true);
        print("0. Quay lai", true);

        std::string choiceStr = input("Lua chon: ");
        try
        {
            choice = std::stoi(choiceStr);
        }
        catch (...)
        {
            print("Lua chon khong hop le.", true);
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            std::string targetUsername = input("Nhap username nguoi nhan: ");
            const auto &users = DataStore::getAllUsers();

            const User *targetUser = nullptr;
            for (const auto &user : users)
            {
                if (user.getUsername() == targetUsername)
                {
                    targetUser = &user;
                    break;
                }
            }

            if (!targetUser)
            {
                print("Khong tim thay nguoi dung.", true);
                input("Nhan Enter de tiep tuc..."); // Thay pause() bằng input()
                break;
            }

            std::string walletId = targetUser->getWalletId();
            Wallet *targetWallet = getWalletById(walletId);
            if (!targetWallet)
            {
                print("Nguoi dung chua co vi.", true);
                input("Nhan Enter de tiep tuc..."); // Thay pause() bằng input()
                break;
            }

            std::string amountStr = input("Nhap so diem muon cap: ");
            int amount = 0;
            try
            {
                amount = std::stoi(amountStr);
            }
            catch (...)
            {
                print("So diem khong hop le.", true);
                break;
            }

            if (!OtpManager::confirmOtpForAction(adminUser.getPhoneNumber()))
            {
                print("Xac thuc OTP that bai. Huy cap diem.", true);
                break;
            }

            issuePointsToWallet(adminUser, walletId, amount);
            input("Nhan Enter de tiep tuc..."); // Thay pause() bằng input()
            break;
        }

        case 2:
        {
            print("Tong so giao dich cua vi tong: " + std::to_string(sysWallet->getTransactionIds().size()), true);
            input("Nhan Enter de tiep tuc..."); // Thay pause() bằng input()
            break;
        }
        case 0:
            return;
        default:
            print("Lua chon khong hop le.", true);
            break;
        }
    } while (true);
}