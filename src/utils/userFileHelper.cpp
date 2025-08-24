#include "./userFileHelper.h" // Đã sửa chữ hoa/thường
#include "../models/User.h"
#include "../models/Wallet.h"
#include "../core/transaction/transaction.h"
#include "../models/Config.h"
#include "../lib/nlohmann/json.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;
using json = nlohmann::json;

bool UserFileHelper::writeStringToFile(const std::string &fileName, const std::string &content, FileCategory category)
{
    std::string path = buildPath(fileName, category);

    std::ofstream outFile(path);
    if (outFile.is_open())
    {
        outFile << content;
        outFile.close();
        return true;
    }
    else
    {
        /* @explain : Không thể mở file ghi đè, không in ra câu thông báo này */
        // std::cerr << "Khong the mo file de ghi: " << path << "\n";
        return false;
    }
}

// Hàm readStringFromFile dùng để đọc toàn bộ nội dung của một file (dạng text) vào một chuỗi std::string.
// Tham số fileName là tên file cần đọc, category là loại file để xây dựng đường dẫn đầy đủ.
// Nếu file mở thành công, hàm trả về nội dung file dưới dạng chuỗi. Nếu không mở được file, hàm trả về chuỗi rỗng.
std::string UserFileHelper::readStringFromFile(const std::string &fileName, FileCategory category)
{
    std::string path = buildPath(fileName, category);

    std::ifstream inFile(path);
    if (inFile.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
        return content;
    }

    return "";
}

/**
 * @brief Tạo đường dẫn đầy đủ tới file dựa trên tên file và loại file.
 *
 * Hàm này nhận vào tên file và loại file (User, Wallet, TransactionLog),
 * sau đó xác định thư mục cơ sở tương ứng với loại file. Nếu thư mục chưa tồn tại,
 * hàm sẽ tự động tạo thư mục đó. Cuối cùng, hàm trả về đường dẫn đầy đủ tới file.
 *
 * @param fileName Tên file cần tạo đường dẫn.
 * @param category Loại file (User, Wallet, TransactionLog).
 * @return Đường dẫn đầy đủ tới file dưới dạng std::string.
 */
std::string UserFileHelper::buildPath(const std::string &fileName, FileCategory category)
{
    std::string baseDir;
    switch (category)
    {
    case FileCategory::User:
        baseDir = USER_DATA_DIR;
        break;
    case FileCategory::Wallet:
        baseDir = WALLET_DATA_DIR;
        break;
    case FileCategory::TransactionLog:
        baseDir = TRANSACTION_DATA_DIR;
        break;
    }

    if (!fs::exists(baseDir))
    {
        fs::create_directories(baseDir);
    }

    return baseDir + "/" + fileName;
}

std::string UserFileHelper::getCurrentDate()
{
    time_t now = time(nullptr);
    tm *t;

#ifdef _WIN32
    tm timeInfo;
    localtime_s(&timeInfo, &now);
    t = &timeInfo;
#else
    t = localtime(&now);
#endif

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", t);
    return std::string(buffer);
}

std::string UserFileHelper::getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm *t;
#ifdef _WIN32
    std::tm timeInfo;
    localtime_s(&timeInfo, &in_time_t);
    t = &timeInfo;
#else
    t = localtime(&in_time_t);
#endif

    char buffer[24];
    std::snprintf(buffer, sizeof(buffer), "%04d%02d%02d%02d%02d%02d%03lld",
                  t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                  t->tm_hour, t->tm_min, t->tm_sec, static_cast<long long>(ms.count()));

    return std::string(buffer);
}

/**
 * @brief Sao lưu file cũ nếu tồn tại tại đường dẫn chỉ định.
 *
 * Kiểm tra xem file tại đường dẫn 'path' có tồn tại hay không.
 * Nếu không tồn tại thì kết thúc hàm.
 * Nếu tồn tại, thực hiện các bước sau:
 * - Lấy ngày hiện tại (định dạng yyyy-mm-dd).
 * - Lấy ngày giờ hiện tại (định dạng yyyy-mm-dd_HH-MM-SS).
 * - Tạo thư mục backup theo ngày nếu chưa tồn tại.
 * - Lấy tên file gốc và phần mở rộng.
 * - Tạo đường dẫn file backup với tên file gốc, ngày giờ và phần mở rộng.
 * - Sao chép file gốc sang đường dẫn backup, ghi đè nếu file backup đã tồn tại.
 *
 * @param path Đường dẫn tới file cần kiểm tra và sao lưu.
 */
void UserFileHelper::backupOldFileIfExists(const std::string &path)
{
    if (!fs::exists(path))
        return;

    std::string date = getCurrentDate();
    std::string dateTime = getCurrentDateTime();

    std::string backupDir = "./backup/" + date;
    if (!fs::exists(backupDir))
    {
        fs::create_directories(backupDir);
    }

    std::string fileName = fs::path(path).stem().string();
    std::string extension = fs::path(path).extension().string();
    std::string backupPath = backupDir + "/" + fileName + "_" + dateTime + extension;

    fs::copy(path, backupPath, fs::copy_options::overwrite_existing);
}

bool UserFileHelper::saveNewUser(const User &user)
{
    json j; // Khởi tạo đối tượng JSON để chứa dữ liệu người dùng

    // Ghi các thuộc tính của User vào JSON
    j["username"] = user.getUsername();
    j["password"] = user.getPassword();
    j["displayName"] = user.getDisplayName();
    j["walletId"] = user.getWalletId();
    j["phoneNumber"] = user.getPhoneNumber();
    j["role"] = static_cast<int>(user.getRole()); // Ép kiểu enum Role sang int
    j["isAutoPassword"] = user.getIsAutoPassword();

    // Tạo tên file dựa trên username, định dạng .json
    std::string fileName = user.getUsername() + ".json";

    // Xây dựng đường dẫn đầy đủ cho file, nằm trong thư mục User
    std::string path = buildPath(fileName, FileCategory::User);

    // Nếu file cũ tồn tại, backup lại trước khi ghi file mới
    backupOldFileIfExists(path);

    // Ghi JSON vào file (định dạng đẹp với 4 dấu cách thụt dòng)
    return writeStringToFile(fileName, j.dump(4), FileCategory::User);
}

bool UserFileHelper::saveNewWallet(const Wallet &wallet)
{
    json j;
    j["walletId"] = wallet.getWalletId();
    j["points"] = wallet.getPoints();
    j["transactionIds"] = wallet.getTransactionIds();

    std::string fileName = wallet.getWalletId() + ".json";
    std::string path = buildPath(fileName, FileCategory::Wallet);
    backupOldFileIfExists(path);

    return writeStringToFile(fileName, j.dump(4), FileCategory::Wallet);
}

bool UserFileHelper::saveTransactionLog(const Transaction &tx)
{
    json j;
    j["transactionId"] = tx.getTransactionId();
    j["type"] = static_cast<int>(tx.getType());
    j["from"] = tx.getFromWalletId();
    j["to"] = tx.getToWalletId();
    j["amount"] = tx.getAmount();
    j["timestamp"] = tx.getTimestamp();

    std::string fileName = tx.getTransactionId() + ".json";
    return writeStringToFile(fileName, j.dump(4), FileCategory::TransactionLog);
}

bool UserFileHelper::saveUpdatedUser(const User &user)
{
    json j;
    j["username"] = user.getUsername();
    j["password"] = user.getPassword();
    j["displayName"] = user.getDisplayName();
    j["walletId"] = user.getWalletId();
    j["phoneNumber"] = user.getPhoneNumber();
    j["role"] = static_cast<int>(user.getRole());

    std::string fileName = user.getUsername() + ".json";
    std::string path = buildPath(fileName, FileCategory::User);
    backupOldFileIfExists(path);

    return writeStringToFile(fileName, j.dump(4), FileCategory::User);
}

bool UserFileHelper::saveUpdatedWallet(const Wallet &wallet)
{
    json j;
    j["walletId"] = wallet.getWalletId();
    j["points"] = wallet.getPoints();
    j["transactionIds"] = wallet.getTransactionIds();

    std::string fileName = wallet.getWalletId() + ".json";
    std::string path = buildPath(fileName, FileCategory::Wallet);
    backupOldFileIfExists(path);

    return writeStringToFile(fileName, j.dump(4), FileCategory::Wallet);
}

std::vector<std::string> UserFileHelper::listFilesInCategory(FileCategory category)
{
    std::vector<std::string> files;
    std::string baseDir = buildPath("", category);

    for (const auto &entry : fs::directory_iterator(baseDir))
    {
        if (entry.path().extension() == ".json")
        {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}
