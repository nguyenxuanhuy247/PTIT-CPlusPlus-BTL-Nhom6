// user_menu.cpp - cap nhat dung DataStore::syncUser thay vi gui UserFileHelper truc tiep

#include "../models/User.h"
#include "../wallet/walletService.h"
#include "../data/DataStore.h"
#include "../otp/otp.h"
#include "../utils/hash.h"
#include "../utils/console.h"

void showUserMenu(User &currentUser)
{
    int choice = -1;

    do
    {
        print("\n===== USER MENU =====", true);
        print("1. Xem thong tin ca nhan", true);
        print("2. Thay doi ten hien thi", true);
        print("3. Thay doi mat khau", true);
        print("4. Vi diem", true);
        print("0. Dang xuat", true);

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
            print("\n=== THONG TIN CA NHAN ===", true);
            print("Ten dang nhap: " + currentUser.getUsername(), true);
            print("Ten hien thi: " + currentUser.getDisplayName(), true);
            break;
        case 2:
        {
            std::string newName = input("Nhap ten hien thi moi: ");
            currentUser.setDisplayName(newName);
            DataStore::syncUser(currentUser); // dong bo user sau thay doi ten
            print("Da cap nhat ten hien thi!", true);
            break;
        }
        case 3:
        {
            std::string newPass = input("Nhap mat khau moi: ");
            if (!OtpManager::confirmOtpForAction(currentUser.getPhoneNumber()))
            {
                print("Xac thuc OTP that bai.", true, ColorEnum::Red);
                break;
            }
            currentUser.setPassword(newPass);
            DataStore::syncUser(currentUser); //  dong bo user sau doi mat khau
            print("Da cap nhat mat khau!", true, ColorEnum::Green);
            break;
        }
        case 4:
            showWalletMenu(currentUser);
            break;
        case 0:
            DataStore::syncUser(currentUser); //  sao luu user truoc khi dang xuat
            print("Dang xuat thanh cong!", true);
            return;
        default:
            print("Lua chon khong hop le.", true);
            break;
        }

    } while (true);
}
