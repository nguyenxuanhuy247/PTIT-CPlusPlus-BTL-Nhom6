#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

/**
 * Enum định nghĩa các loại giao dịch
 * Transfer: Giao dịch chuyển tiền (1)
 * Deposit: Giao dịch nạp tiền (2)
 */
enum class TransactionType {
    Transfer = 1,  // Chuyển tiền giữa các ví
    Deposit = 2    // Nạp tiền vào ví
};

/**
 * Lớp Transaction đại diện cho một giao dịch trong hệ thống
 * Lưu trữ thông tin chi tiết về giao dịch bao gồm:
 * - Mã giao dịch
 * - Loại giao dịch
 * - Ví nguồn (nếu có)
 * - Ví đích
 * - Số tiền
 * - Thời gian thực hiện
 */
class Transaction {
private:
    std::string transactionId;  // Mã giao dịch (duy nhất)
    TransactionType type;       // Loại giao dịch
    std::string fromWalletId;   // ID ví nguồn (đối với chuyển tiền)
    std::string toWalletId;     // ID ví đích
    int amount;                 // Số tiền giao dịch
    std::time_t timestamp;      // Thời điểm giao dịch (timestamp Unix)

public:
    /**
     * Constructor khởi tạo một giao dịch mới
     * @param type Loại giao dịch (Transfer/Deposit)
     * @param from ID ví nguồn (để trống nếu là nạp tiền)
     * @param to ID ví đích
     * @param amount Số tiền giao dịch (phải > 0)
     */
    Transaction(TransactionType type, const std::string& from, const std::string& to, int amount);

    // Getter/Setter cho mã giao dịch
    std::string getTransactionId() const;  // Lấy mã giao dịch
    void setTransactionId(const std::string& id);  // Thiết lập mã giao dịch

    // Các getter cho thông tin giao dịch
    std::string getFromWalletId() const;  // Lấy ID ví nguồn
    std::string getToWalletId() const;   // Lấy ID ví đích
    int getAmount() const;               // Lấy số tiền giao dịch
    std::time_t getTimestamp() const;    // Lấy thời gian giao dịch
    TransactionType getType() const;     // Lấy loại giao dịch

    /**
     * Thiết lập thời gian giao dịch
     * @param t Timestamp Unix
     */
    void setTimestamp(std::time_t t);

    /**
     * Chuyển thông tin giao dịch thành chuỗi để hiển thị
     * @return Chuỗi mô tả giao dịch (định dạng dễ đọc)
     */
    std::string toString() const;
};

/**
 * Hàm ghi lại giao dịch vào hệ thống
 * @param tx Đối tượng giao dịch cần ghi lại
 */
void recordTransaction(const Transaction& tx);

#endif // TRANSACTION_H