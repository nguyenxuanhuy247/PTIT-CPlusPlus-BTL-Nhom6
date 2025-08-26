// user_menu.cpp - cập nhật dùng DataStore::syncUser thay vì gửi UserFileHelper trực tiếp

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
        printTitle("MENU NGƯỜI DÙNG");
        print("Xin chào " + currentUser.getDisplayName() + "!", true);
        print("1. Xem thông tin cá nhân", true);
        print("2. Thay đổi tên hiển thị", true);
        print("3. Đổi mật khẩu", true);
        print("4. Ví điểm", true);
        print("0. Đăng xuất", true);

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
            printTitle("THÔNG TIN CÁ NHÂN");
            print("Tài khoản: " + currentUser.getUsername(), true);
            print("Tên hiển thị: " + currentUser.getDisplayName(), true);
            print("Số điện thoại: " + currentUser.getPhoneNumber(), true);
            break;
        }
        case 2:
        {
            printTitle("THAY ĐỔI THÔNG TIN CÁ NHÂN");
            std::string newName = input("Nhập tên hiển thị mới: ");
            currentUser.setDisplayName(newName);
            DataStore::syncUser(currentUser); // đồng bộ user sau khi đổi tên
            print("Cập nhật tên hiển thị mới thành công!", true, ColorEnum::Green);
            break;
        }
        case 3:
        {
            printTitle("ĐỔI MẬT KHẨU");
            std::string newPassword = input("Nhập mật khẩu mới: ");
            int passwordAttempts = 0;
            while (!PasswordUtils::isValidPassword(newPassword) && passwordAttempts < 3)
            {
                print("Mật khẩu phải chứa ít nhất 6 kí tự", true, ColorEnum::Red);
                newPassword = input("Nhập mật khẩu mới: ");
                passwordAttempts++;
            }
            if (!PasswordUtils::isValidPassword(newPassword))
            {
                print("Nhập sai quá 3 lần. Đổi mật khẩu thất bại.", true, ColorEnum::Red);
                break;
            }

            int retryCount = 0;
            bool passwordMatched = false;
            while (retryCount < 3)
            {
                std::string confirmPassword = input("Xác nhận mật khẩu mới: ");
                if (newPassword == confirmPassword)
                {
                    passwordMatched = true;
                    break;
                }
                else
                {
                    print("Nhập sai mật khẩu, vui lòng nhập lại.", true, ColorEnum::Red);
                    retryCount++;
                }
            }
            if (!passwordMatched)
            {
                print("Nhập sai quá 3 lần. Đổi mật khẩu thất bại.", true, ColorEnum::Red);
                break;
            }
            if (!OtpManager::confirmOtpForAction(currentUser.getPhoneNumber()))
            {
                print("Xác thực OTP thất bại.", true, ColorEnum::Red);
                break;
            }
            currentUser.setPassword(newPassword);
            DataStore::syncUser(currentUser); //  đồng bộ user sau khi đổi mật khẩu
            print("Cập nhật mật khẩu thành công!", true, ColorEnum::Green);
            break;
        }
        case 4:
        {
            showWalletMenu(currentUser);
            break;
        }
        case 0:
        {
            DataStore::syncUser(currentUser); //  sao lưu user trước khi đăng xuất
            print("Đăng xuất thành công!", true, ColorEnum::Green);
            return;
        }
        default:
        {
            print("Lựa chọn không hợp lệ. Vui lòng kiểm tra lại.", true, ColorEnum::Red);
            break;
        }
        }
    } while (true);
}
