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
        printTitle(" MANAGER MENU");
        print("1. Thong ke so nguoi dung", true);
        print("2. Tong diem toan he thong", true);
        print("3. Tong so giao dich", true);
        print("4. Tao tai khoan nguoi dung moi", true);
        print("5. Vi tong", true);
        print("0. Dang xuat", true);

        std::string ch = input("Lua chon: ");
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
            print("So luong nguoi dung: " + std::to_string(getAllUsers().size()), true);
            break;
        case 2:
        {
            int total = 0;
            for (auto &[id, w] : getAllWallets())
                total += w.getPoints();
            print("Tong diem trong he thong: " + std::to_string(total), true);
            break;
        }
        case 3:
            print("Tong so giao dich: " + std::to_string(getAllTransactions().size()), true);
            break;
        case 4:
            print("=== TAO TAI KHOAN MOI ===", true);
            registerNewUser(true);
            ;
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
