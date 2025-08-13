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
        printTitle("USER MENU");
        print("Xin chào " + currentUser.getDisplayName() + "!", true);
        print("1. Xem thông tin cá nhân", true);
        print("2. Thay đổi tên hiển thị", true);
        print("3. Đổi mật khẩu", true);
        print("4. Ví điểm", true);
        print("0. Đăng xuất", true);

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
            printTitle("THÔNG TIN CÁ NHÂN");
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
                print("Xác thực OTP thất bại.", true, ColorEnum::Red);
                break;
            }
            currentUser.setPassword(newPass);
            DataStore::syncUser(currentUser); //  dong bo user sau doi mat khau
            print("Cập nhật mật khẩu thành công.", true, ColorEnum::Green);
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
