#include "register.h"
#include "../utils/console.h"
#include "../models/User.h"
#include "../models/Wallet.h"
#include "../utils/userFileHelper.h"
#include "../data/dataStore.h"
#include "../lib/nlohmann/json.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

using json = nlohmann::json;
using namespace DataStore;

/**
 * @brief Kiểm tra xem tên người dùng đã tồn tại trong hệ thống hay chưa.
 *
 * Hàm này duyệt qua danh sách tất cả người dùng và so sánh tên người dùng truyền vào
 * với tên người dùng của từng đối tượng User. Nếu tìm thấy tên người dùng trùng khớp,
 * hàm trả về true, ngược lại trả về false.
 *
 * @param username Tên người dùng cần kiểm tra.
 * @return true nếu tên người dùng đã tồn tại, false nếu chưa tồn tại.
 */
bool usernameExists(const std::string &username)
{
    for (const User &user : DataStore::allUsers)
    {
        if (user.getUsername() == username)
            return true;
    }
    return false;
}

bool phoneExists(const std::string &phone)
{
    for (const User &user : DataStore::allUsers)
    {
        if (user.getPhoneNumber() == phone)
            return true;
    }
    return false;
}

void registerNewUser(bool isRegisterByAdmin)
{
    std::string username;
    std::string phone;
    std::string rawPassword;
    bool isAutoPass = false;

    printTitle("ĐĂNG KÝ");

    int usernameAttempts = 0;
    bool usernameInputSuccess = false;
    do
    {
        username = input("Tài khoản: ");
        if (username.empty())
        {
            print("Tên tài khoản không được để trống.", true, ColorEnum::Yellow);
            usernameAttempts++;
        }
        else if (usernameExists(username))
        {
            print("Tên tài khoản đã tồn tại. Vui lòng nhập lại.", true, ColorEnum::Yellow);
            usernameAttempts++;
        }
        else
        {
            usernameInputSuccess = true;
            break;
        }

        if (usernameAttempts >= 3)
        {
            print("\nNhập sai 3 lần.", true, ColorEnum::Red);
            std::string choice = "";
            do
            {
                choice = input("Nhấn 1 để nhập lại Tài khoản, hoặc 0 để quay lại: ");
            } while (choice != "1" && choice != "0");

            if (choice == "0")
            {
                print("Quay lại trang trước.", true);
                return;
            }
            usernameAttempts = 0;
        }
    } while (true);

    int phoneAttempts = 0;
    bool phoneInputSuccess = false;
    do
    {
        phone = input("Số điện thoại: ");
        if (phone.empty())
        {
            print("Số điện thoại không được để trống.", true, ColorEnum::Yellow);
            phoneAttempts++;
        }
        // Chỉ chấp nhận số điện thoại bắt đầu bằng 0 và có độ dài 10 ký tự
        else if (phone.size() != 10 || phone[0] != '0')
        {
            print("Định dạng số điện thoại không đúng. Vui lòng nhập lại", true, ColorEnum::Yellow);
            phoneAttempts++;
        }
        else if (phoneExists(phone))
        {
            print("Số điện thoại đã tồn tại. Vui lòng nhập lại.", true, ColorEnum::Yellow);
            phoneAttempts++;
        }
        else
        {
            phoneInputSuccess = true;
            break;
        }

        if (phoneAttempts >= 3)
        {
            print("\nNhập sai 3 lần.", true, ColorEnum::Red);
            std::string choice = "";
            do
            {
                choice = input("Nhấn 1 để nhập lại Số điện thoại, hoặc 0 để quay lại: ");
            } while (choice != "1" && choice != "0");

            if (choice == "0")
            {
                print("Quay lại trang trước.", true);
                return;
            }
            phoneAttempts = 0;
        }
    } while (true);

    if (!isRegisterByAdmin)
    {
        int passwordAttempts = 0;
        bool passwordInputSuccess = false;
        do
        {
            rawPassword = input("Mật khẩu: ");
            if (rawPassword.empty())
            {
                print("Mật khẩu không được để trống.", true, ColorEnum::Yellow);
                passwordAttempts++;
            }
            else
            {
                passwordInputSuccess = true;
                break;
            }

            if (passwordAttempts >= 3)
            {
                print("\nNhập sai 3 lần.", true, ColorEnum::Red);
                std::string choice = "";
                do
                {
                    choice = input("Nhấn 1 để nhập lại Mật khẩu, hoặc 0 để quay lại: ");
                } while (choice != "1" && choice != "0");

                if (choice == "0")
                {
                    print("Quay lại trang trước.", true);
                    return;
                }
                passwordAttempts = 0;
            }
        } while (true);
    }

    if (rawPassword.empty())
    {
        rawPassword = generateRandomPassword();
        isAutoPass = true;
    }

    std::string hashedPassword = PasswordUtils::hashPassword(rawPassword);

    Wallet wallet;
    std::string walletId = wallet.getWalletId();

    User newUser(username, hashedPassword, UserRole::User, username, walletId, phone, isAutoPass);

    bool isSaveUserSuccess = UserFileHelper::saveNewUser(newUser);
    bool isWalletUserSuccess = UserFileHelper::saveNewWallet(wallet);

    if (isSaveUserSuccess && isWalletUserSuccess)
    {
        allUsers.push_back(newUser);
        allWallets[walletId] = wallet;

        print("Đăng ký tài khoản thành công.", true, ColorEnum::Green);
        if (isRegisterByAdmin)
        {
            // bạn có thể log lại người tạo là admin nếu cần
            print("Mật khẩu của tài khoản " + username + " là: " + rawPassword, true);
        }
    }
    else
    {
        print("Đăng ký tài khoản thất bai.", true, ColorEnum::Red);
    }
}
