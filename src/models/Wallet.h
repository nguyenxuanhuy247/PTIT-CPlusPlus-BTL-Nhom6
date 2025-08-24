#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <random>
#include <sstream>
#include <vector>

class Wallet
{
private:
    std::string walletId;
    int points;
    std::vector<std::string> transactionIds;

    /**
     * @brief Tạo một chuỗi ID duy nhất dạng số hệ thập lục phân.
     *
     * Hàm này sử dụng bộ sinh số ngẫu nhiên để tạo ra một chuỗi gồm 8 ký tự số hệ thập lục phân (0-9, a-f).
     * Mỗi ký tự được sinh ngẫu nhiên, đảm bảo tính duy nhất tương đối cho ID.
     *
     * @return Chuỗi std::string chứa ID duy nhất vừa được tạo.
     */
    static std::string generateUniqueId()
    {
        std::stringstream ss;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        for (int i = 0; i < 8; ++i)
        {
            ss << std::hex << dis(gen);
        }
        return ss.str();
    }

public:
    // ✅ Constructor mặc định – dùng khi tạo ví mới
    Wallet() : walletId(generateUniqueId()), points(0) {}

    // ✅ Constructor mở rộng – dùng khi khôi phục ví từ dữ liệu cũ
    Wallet(const std::string &id, int pts = 0, const std::vector<std::string> &txIds = {})
        : walletId(id), points(pts), transactionIds(txIds) {}

    std::string getWalletId() const
    {
        return walletId;
    }

    int getPoints() const
    {
        return points;
    }

    const std::vector<std::string> &getTransactionIds() const
    {
        return transactionIds;
    }

    void setPoints(int newPoints)
    {
        points = newPoints;
    }

    void addTransactionId(const std::string &txId)
    {
        transactionIds.push_back(txId);
    }

    void setTransactionIds(const std::vector<std::string> &ids)
    {
        transactionIds = ids;
    }

    void addPoints(int amount)
    {
        points += amount;
    }

    bool deductPoints(int amount)
    {
        if (amount > points)
            return false;
        points -= amount;
        return true;
    }
};

#endif // WALLET_H
