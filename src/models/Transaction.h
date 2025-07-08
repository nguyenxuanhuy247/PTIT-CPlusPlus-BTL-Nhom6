#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

enum class TransactionType {
    Transfer = 1,
    Deposit = 2
};

class Transaction {
private:
    std::string transactionId;
    TransactionType type;
    std::string fromWalletId;
    std::string toWalletId;
    int amount;
    std::time_t timestamp;

public:
    Transaction(TransactionType type, const std::string& from, const std::string& to, int amount);

    std::string getTransactionId() const;
    void setTransactionId(const std::string& id);

    std::string getFromWalletId() const;
    std::string getToWalletId() const;
    int getAmount() const;
    std::time_t getTimestamp() const;
    TransactionType getType() const;
    void setTimestamp(std::time_t t);

    std::string toString() const;
};

void recordTransaction(const Transaction& tx);

#endif // TRANSACTION_H
