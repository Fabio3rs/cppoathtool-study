#include <generate_totp.hpp>
#include <gtest/gtest.h>

TEST(GenerateOtpTest, ValidOtpGeneration) {
    std::string key = "MZXW6YTBOJUWU23MNU"; // This is an example Base32 key
    uint64_t hotp_value = 52276810;         // Example HOTP value
    std::string expected_otp =
        "487656"; // Expected OTP for this key and counter value

    std::string generated_otp = generate_otp(key, hotp_value);

    EXPECT_EQ(generated_otp, expected_otp);
}
