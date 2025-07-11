
#ifndef SYS_WALLET_SERVICE_H
#define SYS_WALLET_SERVICE_H

#include "../models/User.h"

// Hàm dùng để cấp điểm từ ví tổng tới ví người dùng
bool issuePointsToWallet(const User& adminUser, const std::string& toWalletId, int amount);

// Giao diện hiển thị menu ví tổng (chỉ dành cho Manager)
void showSystemWalletView(User adminUser);

#endif /
