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
        print("Không tìm thấy ví của bạn.", true, ColorEnum::Red);
        return;
    }

    int choice = -1;
    do
    {
        printTitle("VÍ ĐIỂM");
        print("Số điểm: " + std::to_string(wallet->getPoints()), true);
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

            printTitle("CHUYỂN ĐIỂM");
            int currentUserWalletPoint = DataStore::getWalletById(user.getWalletId())->getPoints();
            if (currentUserWalletPoint > 0)
            {
                transferPointsUI(user);
            }
            else
            {
                print("Số điểm của bạn không đủ để giao dịch", true, ColorEnum::Red);
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

/**
 * @usage Hàm xử lý việc chuyển điểm giữa hai ví.
 */
void transferPointsUI(User &transferUser)
{
    std::string phone = input("Nhập số điện thoại người nhận: ");
    User *receiverUser = getUserByPhone(phone);

    if (!receiverUser)
    {
        print("Số điện thoại người nhận không đúng.", true, ColorEnum::Red);
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
        print("Số điểm muốn chuyển không hợp lệ.", true, ColorEnum::Red);
        return;
    }

    if (amount <= 0)
    {
        print("Số điểm muốn chuyển phải lớn hơn 0.", true, ColorEnum::Red);
        return;
    }

    if (amount > DataStore::getWalletById(transferUser.getWalletId())->getPoints())
    {
        print("Số điểm trong ví không đủ để chuyển.", true, ColorEnum::Red);
        return;
    }

    if (transferUser.getDisplayName() == receiverUser->getDisplayName())
    {
        print("Không thể tự chuyển cho chính mình.", true, ColorEnum::Red);
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

    if (!OtpManager::confirmOtpForAction(transferUser.getPhoneNumber()))
    {
        print("❌ Xác thực OTP thất bại. Hủy giao dịch.", true, ColorEnum::Red);
        return;
    }

    bool success = transferPointsBetweenWallets(transferUser.getWalletId(), receiverUser->getWalletId(), amount);
    if (!success)
    {
        print("Chuyển điểm thất bại.", true, ColorEnum::Red);
    }
}

bool transferPointsBetweenWallets(const std::string &fromId, const std::string &toId, int amount)
{
    Wallet *from = getWalletById(fromId);
    Wallet *to = getWalletById(toId);
    User *transferUser = getUserByWalletId(fromId);
    User *receiveUser = getUserByWalletId(toId);

    if (toId == SYSTEM_WALLET_ID)
    {
        print("Không thể chuyển điểm vào ví tổng.", true, ColorEnum::Red);
        return false;
    }

    if (!from || !to)
    {
        print("Không tìm thấy ví nguồn hoặc ví đích.", true, ColorEnum::Red);
        return false;
    }

    if (from->getPoints() < amount)
    {
        print("Không điểm không đủ để thực hiện chuyển.", true, ColorEnum::Red);
        return false;
    }

    if (transferUser == receiveUser)
    {
        print("Không thể tự chuyển cho chính mình.", true, ColorEnum::Red);
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

    print("Đã chuyển " + std::to_string(amount) + " điểm cho người dùng " + receiveUser->getDisplayName() + " có số điện thoại " + receiveUser->getPhoneNumber() + " thành công", true, ColorEnum::Green);
    return true;
}

/**
 * @usage Hiển thị lich sử giao dịch của người dùng.
 */
void showTransactionHistory(User &user)
{
    std::string walletId = user.getWalletId();
    DataStore::loadAllTransactions();
    const auto &transactions = getAllTransactions();

    printTitle("LỊCH SỬ GIAO DỊCH");
    bool found = false;
    for (const auto &tx : transactions)
    {
        if (tx.getFromWalletId() == walletId)
        {

            print(tx.toString(walletId), true, ColorEnum::Red);
            found = true;
        }
        else if (tx.getToWalletId() == walletId)
        {

            print(tx.toString(walletId), true, ColorEnum::Green);
            found = true;
        }
    }
    if (!found)
    {
        print("Không có giao dịch.", true);
    }
}
