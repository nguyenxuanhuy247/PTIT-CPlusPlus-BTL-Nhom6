#include "../wallet/walletService.h"
#include "../models/Wallet.h"
#include "../transaction/transaction.h"
#include "../models/User.h"
#include "../utils/console.h"
#include "../data/dataStore.h"
#include "../utils/userFileHelper.h"
#include "../otp/otp.h"
#include "../models/Config.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <vector>

using namespace DataStore;

void showWalletMenu(User &user)
{
    Wallet *wallet = getWalletById(user.getWalletId());
    if (!wallet)
    {
        print("Không tìm thấy ví của bạn.", true);
        return;
    }

    int choice = -1;
    do
    {
        printTitle("VÍ ĐIỂM");
        print("Số dư: " + std::to_string(wallet->getPoints()) + " điểm", true);
        print("1. Chuyển điểm", true);
        print("2. Lịch sử giao dịch", true);
        print("0. Quay lại", true);

        std::string choiceStr = input("Nhập lựa chọn của bạn: ");
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

            int currentUserWalletPoint = DataStore::getWalletById(user.getWalletId())->getPoints();
            if (currentUserWalletPoint > 0)
            {
                transferPointsUI(user);
            }
            else
            {
                print("Số điểm hiện tại của bạn là 0. Không thể thực hiện các chức năng ví.", true, ColorEnum::Yellow);
            }
            break;
        }
        case 2:
            showTransactionHistory(user);
            break;
        case 0:
            return;
        default:
            print("Lựa chọn không hợp lệ. Vui lòng kiểm tra lại.", true, ColorEnum::Red);
            break;
        }
    } while (true);
}

void transferPointsUI(User &user)
{
    std::string phone = input("Nhập số điện thoại người nhận: ");
    User *receiverUser = getUserByPhone(phone);

    if (!receiverUser)
    {
        print("Không tìm thấy người nhận với số điện thoại đã nhập.", true, ColorEnum::Yellow);
        return;
    }

    std::string amountStr = input("Nhập số điểm muốn chuyển: ");
    int amount = 0;
    try
    {
        amount = std::stoi(amountStr);
    }
    catch (...)
    {
        print("Số điểm không hợp lệ.", true, ColorEnum::Yellow);
        return;
    }

    if (amount <= 0)
    {
        print("Số điểm phải lớn hơn 0.", true, ColorEnum::Yellow);
        return;
    }

    std::string confirm = input("Bạn có chắc chắn muốn chuyển " + std::to_string(amount) +
                                    " điểm cho " + receiverUser->getUsername() + "? (y/n): ",
                                ColorEnum::Yellow);
    if (confirm != "y" && confirm != "Y")
    {
        print("Hủy giao dịch.", true, ColorEnum::Yellow);
        return;
    }

    if (!OtpManager::confirmOtpForAction(user.getPhoneNumber()))
    {
        print("❌ Xác thực OTP thất bại. Hủy giao dịch.", true, ColorEnum::Red);
        return;
    }

    bool success = transferPointsBetweenWallets(user.getWalletId(), receiverUser->getWalletId(), amount);
    if (!success)
    {
        print("Chuyển điểm thất bại.", true, ColorEnum::Red);
    }
}

bool transferPointsBetweenWallets(const std::string &fromId, const std::string &toId, int amount)
{
    Wallet *from = getWalletById(fromId);
    Wallet *to = getWalletById(toId);
    if (toId == SYSTEM_WALLET_ID)
    {
        std::cerr << "❌ Không thể chuyển điểm vào ví tổng!\n";
        return false;
    }

    if (!from || !to)
    {
        std::cerr << "Không tìm thấy ví nguồn hoặc ví đích.\n";
        return false;
    }

    if (from->getPoints() < amount)
    {
        std::cerr << "Số dư không đủ. Không thể chuyển.\n";
        return false;
    }

    from->deductPoints(amount);
    to->addPoints(amount);

    Transaction tx(TransactionType::Transfer, fromId, toId, amount);
    UserFileHelper::saveTransactionLog(tx); // log vẫn dùng trực tiếp

    from->addTransactionId(tx.getTransactionId());
    to->addTransactionId(tx.getTransactionId());

    DataStore::syncWallet(from->getWalletId()); // ✅ gọi qua DataStore
    DataStore::syncWallet(to->getWalletId());

    std::cout << "Chuyển " << amount << " điểm thành công!\n";
    return true;
}

void showTransactionHistory(User &user)
{
    std::string walletId = user.getWalletId();
    DataStore::loadAllTransactions();
    const auto &transactions = getAllTransactions();

    printTitle("LỊCH SỬ GIAO DỊCH");
    bool found = false;
    for (const auto &tx : transactions)
    {
        if (tx.getFromWalletId() == walletId || tx.getToWalletId() == walletId)
        {
            print(tx.toString(), true, ColorEnum::Blue);
            found = true;
        }
    }
    if (!found)
    {
        print("Không có giao dịch nào liên quan tới ví của bạn.", true);
    }
}
