#include "./otp.h"
#include "../../utils/console.h"
#include <ctime>

static std::unordered_map<std::string, std::pair<std::string, std::time_t>> otpStorage;

namespace OtpManager
{

    bool confirmOtpForAction(const std::string &phone)
    {
        std::string otp = generateOtp(phone);
        print("OTP đã được gửi tới số điện thoại: " + phone, true, ColorEnum::Green);
        print("OTP của bạn là: " + otp + ". Vui lòng KHÔNG CHIA SẺ VỚI BẤT KỲ AI.", true);

        std::string userOtp = input("Nhập mã OTP để xác nhận: ");
        if (!verifyOtp(phone, userOtp))
        {
            print("OTP không đúng hoặc hết hạn.", true, ColorEnum::Red);
            return false;
        }
        return true;
    }

    std::string generateOtp(const std::string &phone)
    {
        std::string otp = generateRandomPassword();
        otpStorage[phone] = {otp, std::time(nullptr)};
        return otp;
    }

    bool verifyOtp(const std::string &phone, const std::string &otp)
    {
        auto it = otpStorage.find(phone);
        if (it == otpStorage.end())
            return false;

        auto [storedOtp, timestamp] = it->second;
        if (std::time(nullptr) - timestamp > 300)
            return false; // 5 phút
        return storedOtp == otp;
    }
}
