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

bool issuePointsToWallet(const User adminUser, const std::string &toWalletId, const std::string &targetPhonenumber, int amount)
{
    if (!adminUser.isManager())
    {
        print("❌ Chỉ Người quản lý mới có quyền cấp điểm!", true);
        return false;
    }

    Wallet *toWallet = getWalletById(toWalletId);
    Wallet *sysWallet = getWalletById(SYSTEM_WALLET_ID);
    User *targetUser = getUserByPhone(targetPhonenumber);

    if (!toWallet || !sysWallet)
    {
        print("Không tìm thấy ví đích hoặc ví tổng.", true, ColorEnum::Yellow);
        return false;
    }

    if (!sysWallet->deductPoints(amount))
    {
        print("⚠️ Ví không đủ điểm.", true, ColorEnum::Yellow);
        return false;
    }

    toWallet->addPoints(amount);
    Transaction tx(TransactionType::Transfer, SYSTEM_WALLET_ID, toWalletId, amount);
    recordTransaction(tx);

    print("Đã chuyển " + std::to_string(amount) + " điểm cho người dùng " + targetUser->getDisplayName() + " có số điện thoại " + targetUser->getPhoneNumber() + " thành công", true, ColorEnum::Green);
    return true;
}

void showSystemWalletView(User adminUser)
{
    if (!adminUser.isManager())
    {
        print("❌ Chỉ admin mới có quyền truy cập!", true, ColorEnum::Red);
        return;
    }

    Wallet *sysWallet = getWalletById(SYSTEM_WALLET_ID);
    if (!sysWallet)
    {
        print("⚠️ Ví tổng không tồn tại.", true, ColorEnum::Red);
        return;
    }

    int choice = -1;
    do
    {
        printTitle("MENU VÍ TỔNG");
        print("1. Cấp điểm cho người dùng", true);
        print("2. Lịch sử giao dịch", true);
        print("0. Quay lại", true);

        std::string choiceStr = input("Vui lòng nhập lựa chọn của bạn: ");
        try
        {
            choice = std::stoi(choiceStr);
        }
        catch (...)
        {
            print("Lựa chọn không hợp lệ. Vui lòng kiểm tra lại.", true, ColorEnum::Red);
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            printTitle("CẤP ĐIỂM CHO NGƯỜI DÙNG");
            std::string targetPhonenumber = input("Nhập số điện thoại của người nhận: ");
            const auto &users = DataStore::getAllUsers();

            const User *targetUser = nullptr;
            for (const auto &user : users)
            {
                if (user.getPhoneNumber() == targetPhonenumber)
                {
                    targetUser = &user;
                    break;
                }
            }

            if (!targetUser)
            {
                print("Không tìm thây người dùng.", true, ColorEnum::Red);
                break;
            }

            std::string walletId = targetUser->getWalletId();
            Wallet *targetWallet = getWalletById(walletId);
            if (!targetWallet)
            {
                print("Người dùng chưa có ví.", true, ColorEnum::Red);
                break;
            }

            std::string amountStr = input("Nhập số điểm muốn cấp: ");
            int amount = 0;
            try
            {
                amount = std::stoi(amountStr);
            }
            catch (...)
            {
                print("Số điểm không hợp lệ.", true, ColorEnum::Red);
                break;
            }

            if (!OtpManager::confirmOtpForAction(adminUser.getPhoneNumber()))
            {
                print("Xác thực OTP thất bại. Hủy bỏ quy trình cấp điểm.", true, ColorEnum::Red);
                break;
            }

            issuePointsToWallet(adminUser, walletId, targetPhonenumber, amount);
            break;
        }

        case 2:
        {
            printTitle("LỊCH SỬ GIAO DỊCH CỦA VÍ TỔNG");
            print("Tổng số giao dịch: " + std::to_string(sysWallet->getTransactionIds().size()), true);

            DataStore::loadAllTransactions();
            const auto &transactions = getAllTransactions();
            bool found = false;
            for (const auto &tx : transactions)
            {
                if (tx.getFromWalletId() == "WALLET_SYS")
                {

                    print(tx.toString(), true, tx.getType() == TransactionType::Deposit ? ColorEnum::Green : ColorEnum::Red);
                    found = true;
                }
            }
            if (!found)
            {
                print("Không có giao dịch.", true);
            }
            break;
        }
        case 0:
            return;
        default:
            print("Lựa chọn không hợp lệ. Vui lòng kiểm tra lại.", true, ColorEnum::Red);
            break;
        }
    } while (true);
}