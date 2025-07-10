#ifndef WALLETSERVICE_H
#define WALLETSERVICE_H

#include "../models/User.h"

// Hiển thị menu ví điểm cho người dùng (và hiện số dư ngay lập tức)
void showWalletMenu(User& user);

// Giao diện nhập và thực hiện chuyển điểm giữa các ví (tìm user theo số điện thoại)
void transferPointsUI(User& user);

// Giao dịch chuyển điểm giữa hai ví (nguyên tử)
bool transferPointsBetweenWallets(const std::string& fromId, const std::string& toId, int amount);

// Gọi giao diện xem lịch sử giao dịch (cài đặt chi tiết trong transactionService.cpp)
void showTransactionHistory(User& user);

#endif // WALLETSERVICE_H
