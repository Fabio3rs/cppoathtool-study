#include <ctime>
#include <generate_totp.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Provide secret key as the only argument" << std::endl;
        return 1;
    }

    const uint64_t current_time =
        static_cast<uint64_t>(std::time(nullptr) / 30);
    std::cout << generate_otp(argv[1], current_time) << std::endl;

    return 0;
}
