#ifndef USERFILEHELPER_H
#define USERFILEHELPER_H

#include <string>
#include <vector>
#include "../models/User.h"
#include "../models/Wallet.h"
#include "../core/transaction/transaction.h"

#include <ctime>
#include <filesystem> // thêm thư viện cho thao tác file

enum class FileCategory
{
    User,
    Wallet,
    TransactionLog
};

// UserFileHelper giúp đọc/ghi, lưu trữ, cập nhật và backup dữ liệu ra file, chia theo từng loại dữ liệu trong hệ thống.
class UserFileHelper
{
public:
    // Lưu thông tin người dùng mới vào file
    static bool saveNewUser(const User &user);

    // Lưu thông tin ví mới vào file
    static bool saveNewWallet(const Wallet &wallet);

    // Lưu log giao dịch mới vào file
    static bool saveTransactionLog(const Transaction &tx);

    // Đọc toàn bộ nội dung từ file (theo tên và loại file)
    static std::string readStringFromFile(const std::string &fileName, FileCategory category);

    // Ghi nội dung chuỗi vào file (theo tên và loại file)
    static bool writeStringToFile(const std::string &fileName, const std::string &content, FileCategory category);

    // Liệt kê danh sách các file thuộc một loại (category) nhất định
    static std::vector<std::string> listFilesInCategory(FileCategory category);

    // Lưu lại thông tin người dùng đã được cập nhật
    static bool saveUpdatedUser(const User &user);

    // Lưu lại thông tin ví đã được cập nhật
    static bool saveUpdatedWallet(const Wallet &wallet);

private:
    // Xây dựng đường dẫn file dựa trên tên file và loại file
    static std::string buildPath(const std::string &fileName, FileCategory category);

    // Nếu file cũ tồn tại thì tạo bản backup trước khi ghi mới
    static void backupOldFileIfExists(const std::string &path);

    // Lấy ngày hiện tại dưới dạng chuỗi (yyyy-MM-dd)
    static std::string getCurrentDate();

    // Lấy ngày giờ hiện tại dưới dạng chuỗi (yyyy-MM-dd HH:mm:ss)
    static std::string getCurrentDateTime(); // ✅ chỉ khai báo
};

#endif // USERFILEHELPER_H
