#ifndef OTP_H
#define OTP_H

#include <string>
#include <unordered_map>

namespace OtpManager {
    std::string generateOtp(const std::string& phone);
    bool verifyOtp(const std::string& phone, const std::string& otp);
    bool confirmOtpForAction(const std::string& phone);
}

#endif 