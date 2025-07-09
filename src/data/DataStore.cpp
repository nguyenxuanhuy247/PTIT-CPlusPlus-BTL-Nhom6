#include "../include/DataStore.h"
#include "../include/UserFileHelper.h"
#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

namespace DataStore {
    std::unordered_map<std::string, Wallet> allWallets;
    std::vector<Transaction> allTransactions;
    std::vector<User> allUsers;

    const std::unordered_map<std::string, Wallet>& getAllWallets() {
        return allWallets;
    }

    const std::vector<Transaction>& getAllTransactions() {
        return allTransactions;
    }

    const std::vector<User>& getAllUsers() {
        return allUsers;
    }

    Wallet* getWalletById(const std::string& id) {
        auto it = allWallets.find(id);
        if (it != allWallets.end()) return &it->second;
        return nullptr;
    }

    User* getUserByPhone(const std::string& phone) {
        for (auto& user : allUsers) {
            if (user.getPhoneNumber() == phone)
                return &user;
        }
        return nullptr;
    }

     bool syncWallet(const std::string& walletId) {
        auto it = allWallets.find(walletId);
        if (it == allWallets.end()) return false;
        return UserFileHelper::saveUpdatedWallet(it->second);
    }

bool syncUser(const User& updatedUser) {
    // Cập nhật user tương ứng trong allUsers
    for (auto& user : allUsers) {
        if (user.getUsername() == updatedUser.getUsername()) {
            user = updatedUser; // ✅ Ghi đè user trong allUsers
            break;
        }
    }

    // Ghi file
    return UserFileHelper::saveUpdatedUser(updatedUser);
}

    void syncAll() {
        for (const auto& [id, wallet] : allWallets) {
            UserFileHelper::saveUpdatedWallet(wallet);
        }
        for (const auto& user : allUsers) {
            UserFileHelper::saveUpdatedUser(user);
        }
    }
    
    void loadAllUsers() {
        allUsers.clear();
        auto filenames = UserFileHelper::listFilesInCategory(FileCategory::User);
        for (const auto& file : filenames) {
            std::string content = UserFileHelper::readStringFromFile(file, FileCategory::User);
            if (!content.empty()) {
                json j = json::parse(content);
                User u;
                u.setUsername(j.value("username", ""));
                u.setPassword(j.value("password", ""));
                u.setDisplayName(j.value("displayName", ""));
                u.setWalletId(j.value("walletId", ""));
                u.setPhoneNumber(j.value("phoneNumber", ""));
                u.setRole(static_cast<UserRole>(j.value("role", 1)));
                allUsers.push_back(u);
            }
        }
    }

    void loadAllWallets() {
        allWallets.clear();
        auto filenames = UserFileHelper::listFilesInCategory(FileCategory::Wallet);
        for (const auto& file : filenames) {
            std::string content = UserFileHelper::readStringFromFile(file, FileCategory::Wallet);
            if (!content.empty()) {
                json j = json::parse(content);

                std::string walletId = j.value("walletId", "");
                int points = j.value("points", 0);
                std::vector<std::string> txIds = j.value("transactionIds", std::vector<std::string>{});

                if (!walletId.empty()) {
                    Wallet w(walletId, points, txIds);
                    allWallets[walletId] = w;
                }
            }
        }
    }



    void loadAllTransactions() {
        allTransactions.clear();
        auto filenames = UserFileHelper::listFilesInCategory(FileCategory::TransactionLog);
        for (const auto& file : filenames) {
            std::string content = UserFileHelper::readStringFromFile(file, FileCategory::TransactionLog);
            if (!content.empty()) {
                json j = json::parse(content);
                TransactionType type = static_cast<TransactionType>(j.value("type", 1));
                std::string from = j.value("from", "");
                std::string to = j.value("to", "");
                int amount = j.value("amount", 0);
                std::time_t timestamp = j.value("timestamp", std::time(nullptr));
                std::string txId = j.value("transactionId", "");

                Transaction tx(type, from, to, amount);
                tx.setTimestamp(timestamp);
                tx.setTransactionId(txId);  // ✅ FIX: đảm bảo ID không bị sinh lại

                allTransactions.push_back(tx);
            }
        }
    }

    void loadAllData() {
        loadAllUsers();
        loadAllWallets();
        loadAllTransactions();
    }
}
