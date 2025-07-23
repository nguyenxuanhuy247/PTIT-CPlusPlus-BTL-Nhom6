#include <iostream>
#include "../wallet/sysWalletService.h"
#include "../models/User.h"
#include "../data/dataStore.h"
#include "../core/auth/register.h"
using namespace DataStore;
void print(const std::string& message, bool newline = true) {
    std::cout << message;
    if (newline) std::cout << std::endl;
}
std::string input(const std::string& prompt) {
    std::string result;
    std::cout << prompt;
    std::getline(std::cin, result);
    return result;
}

void showManagerMenu(User currentUser) {

    int choice = -1;
    do {
        print("\n===== MANAGER MENU =====", true);
        print("1. Thong ke so nguoi dung", true);
        print("2. Tong diem toan he thong", true);
        print("3. Tong so giao dich", true);
        print("4. Tao tai khoan nguoi dung moi", true);
        print("5. Vi tong", true);
        print("0. Dang xuat", true);

        std::string ch = input("Lua chon: ");
        try { choice = std::stoi(ch); } catch (...) { continue; }

        switch (choice) {
            case 1:
                print("So luong nguoi dung: " + std::to_string(getAllUsers().size()), true);
                break;
            case 2: {
                int total = 0;
                for (auto& [id, w] : getAllWallets()) total += w.getPoints();
                print("Tong diem trong he thong: " + std::to_string(total), true);
                break;
            }
            case 3:
                print("Tong so giao dich: " + std::to_string(getAllTransactions().size()), true);
                break;
            case 4:
                print("=== TAO TAI KHOAN MOI ===", true);
                registerNewUser(true); ;
                break;
            case 5: {
                showSystemWalletView(currentUser);
                break;
            }

            case 0:
                return;
        }
    } while (true);
}
