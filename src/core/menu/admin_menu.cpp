#include <iostream>
#include <iomanip>

#include "../wallet/sysWalletService.h"
#include "../models/User.h"
#include "../data/dataStore.h"
#include "../core/auth/register.h"
#include "../../utils/console.h"

using namespace DataStore;

void showManagerMenu(User currentUser)
{

    int yourChoice = -1;
    do
    {
        printTitle("MENU QUẢN TRỊ VIÊN");
        print("1. Thống kê số lượng người dùng", true);
        print("2. Thống kê điểm trong toàn hệ thống", true);
        print("3. Thống kê số giao dịch", true);
        print("4. Tạo tài khoản người dùng", true);
        print("5. Ví tổng", true);
        print("0. Đăng xuất", true);

        std::string ch = input("Nhập lựa chọn của bạn: ");
        try
        {
            /* @explain : viết tắt của string to int, dùng để chuyển chuỗi (std::string) thành số nguyên (int) */
            yourChoice = std::stoi(ch);
        }
        catch (...)
        {
            continue;
        }
        int stt = 1;
        switch (yourChoice)
        {
        case 1:
        {
            printTitle("THỐNG KÊ NGƯỜI DÙNG");
            print("Số lượng người dùng: " + std::to_string(getAllUsers().size()), true);
            print("STT\t|\tTên hiển thị", true, ColorEnum::Blue);
            for (const auto &user : getAllUsers())
            {
                print(std::to_string(stt) + "\t|\t" + user.getDisplayName(), true);
                ++stt;
            }
            break;
        }
        case 2:
        {
            printTitle("THỐNG KÊ ĐIỂM TRONG TOÀN HỆ THỐNG");
            int total = 0;
            for (auto &[id, wallet] : getAllWallets())
                total += wallet.getPoints();
            print("Tổng điểm của hệ thống: " + std::to_string(total), true);

            // In tiêu đề bảng
            std::cout << std::left << std::setw(5) << "STT"
                      << " | " << std::setw(20) << "Tên hiển thị       "
                      << " | " << std::setw(10) << "Số điểm" << std::endl;

            int stt = 1;
            for (auto &[id, wallet] : getAllWallets())
            {
                User *user = getUserByWalletId(wallet.getWalletId());
                if (!user)
                    continue;
                std::string name = user ? user->getDisplayName() : "(Trống)";

                std::cout << std::left << std::setw(5) << stt
                          << " | " << std::setw(20) << name
                          << " | " << std::setw(10) << wallet.getPoints()
                          << std::endl;
                ++stt;
            }
            break;
        }
        case 3:
        {
            printTitle("THỐNG KÊ GIAO DỊCH");
            print("Tổng số giao dịch: " + std::to_string(getAllTransactions().size()), true);
            break;
        }
        case 4:
        {
            printTitle("TẠO TÀI KHOẢN MỚI");
            registerNewUser(true, false);
            break;
        }
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
