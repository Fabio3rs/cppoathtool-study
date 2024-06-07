#pragma once

#include <cstdint>
#include <string>

std::string generate_otp(std::string key, uint64_t hotp_value);
