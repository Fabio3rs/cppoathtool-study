/**
 * @brief Based on python https://pypi.org/project/oathtool/ implementation
 */
#include "generate_totp.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <span>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
// Base32 lookup table
const std::unordered_map<char, unsigned int> b32_lookup = {
    {'A', 0},  {'B', 1},  {'C', 2},  {'D', 3},  {'E', 4},  {'F', 5},  {'G', 6},
    {'H', 7},  {'I', 8},  {'J', 9},  {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13},
    {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20},
    {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}, {'2', 26}, {'3', 27},
    {'4', 28}, {'5', 29}, {'6', 30}, {'7', 31}};

// Pad function
std::string pad(const std::string &input, size_t size = 8) {
    size_t remainder = input.length() % size;
    if (remainder == 0)
        return input;
    return input + std::string(size - remainder, '=');
}

auto inplace_clean_spaces(std::string &input) -> std::string & {
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

// Base32 decode function
std::vector<unsigned char> base32_decode(std::string input) {
    inplace_clean_spaces(input);
    input = pad(input);
    std::vector<unsigned char> output((input.length() * 5) / 8);

    size_t buffer = 0;
    int bitsLeft = 0;
    size_t index = 0;

    for (char c : input) {
        if (c == '=')
            break;

        auto it = b32_lookup.find(c);
        if (it == b32_lookup.end())
            continue;

        buffer = (buffer << 5) | (it->second & 0x1FU);
        bitsLeft += 5;

        if (bitsLeft >= 8) {
            output[index++] =
                static_cast<unsigned char>((buffer >> (bitsLeft - 8)) & 0xFF);
            bitsLeft -= 8;
        }
    }
    output.resize(index);
    return output;
}

// HMAC-SHA1 function
std::vector<unsigned char> hmac_sha1(const std::span<unsigned char> &key,
                                     const std::span<unsigned char> &msg) {
    std::array<unsigned char, SHA_DIGEST_LENGTH> hash{};
    unsigned int len = SHA_DIGEST_LENGTH;

    HMAC(EVP_sha1(), key.data(), static_cast<int>(key.size()), msg.data(),
         msg.size(), hash.data(), &len);

    return std::vector<unsigned char>(hash.data(), hash.data() + len);
}
} // namespace

// Generate OTP function
std::string generate_otp(std::string key, uint64_t hotp_value) {
    // Convert HOTP to bytes
    std::array<unsigned char, 8> hotp_bytes;
    for (int i = 7; i >= 0; --i) {
        hotp_bytes[static_cast<size_t>(i)] = hotp_value & 0xFF;
        hotp_value >>= 8;
    }

    // Convert base32 key to bytes
    std::vector<unsigned char> key_bytes = base32_decode(std::move(key));

    // Generate HMAC-SHA1 from HOTP based on key
    std::vector<unsigned char> msgbytes(hotp_bytes.data(),
                                        hotp_bytes.data() + 8);
    std::vector<unsigned char> hmac_result = hmac_sha1(key_bytes, msgbytes);

    // Compute hash truncation
    const unsigned int offset = hmac_result[19] & 0x0F;
    const uint32_t truncated_hash = ((hmac_result[offset] & 0x7FU) << 24U) |
                                    ((hmac_result[offset + 1] & 0xFFU) << 16U) |
                                    ((hmac_result[offset + 2] & 0xFFU) << 8U) |
                                    (hmac_result[offset + 3] & 0xFFU);

    // Encode into a smaller number of digits
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << (truncated_hash % 1000000);
    return oss.str();
}
