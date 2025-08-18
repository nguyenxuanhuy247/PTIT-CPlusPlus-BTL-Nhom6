#include <iostream>
#include "../wallet/sysWalletService.h"
#include "../models/User.h"
#include "../data/dataStore.h"
#include "../core/auth/register.h"
#include "../../utils/console.h"

using namespace DataStore;

void showManagerMenu(User currentUser)
{

    int choice = -1;
    do
    {
        printTitle("MANAGER MENU");
        print("1. Thống kê số lượng người dùng", true);
        print("2. Tổng điểm toàn hệ thống", true);
        print("3. Tổng số giao dịch", true);
        print("4. Tạo tài khoản người dùng", true);
        print("5. Ví tổng", true);
        print("0. Đăng xuất", true);

        std::string ch = input("Nhập lựa chọn của bạn: ");
        try
        {
            /* @explain : viết tắt của string to int, dùng để chuyển chuỗi (std::string) thành số nguyên (int) */
            choice = std::stoi(ch);
        }
        catch (...)
        {
            continue;
        }

        switch (choice)
        {
        case 1:
            print("Số lượng người dùng: " + std::to_string(getAllUsers().size()), true);
            break;
        case 2:
        {
            int total = 0;
            for (auto &[id, wallet] : getAllWallets())
                total += wallet.getPoints();
            print("Tổng điểm của hệ thống: " + std::to_string(total), true);
            break;
        }
        case 3:
            print("Tổng số giao dịch: " + std::to_string(getAllTransactions().size()), true);
            break;
        case 4:
            printTitle("TẠO TÀI KHOẢN MỚI");
            registerNewUser(true);
            break;
        case 5:
        {
            showSystemWalletView(currentUser);
            break;
        }

        case 0:
            return;
        }
    } while (true);
}
