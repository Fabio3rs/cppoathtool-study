# OTP Generator

This project is a study about OAUTH TOTP (Time-based One-Time Password) based on the Python version available at [oathtool](https://pypi.org/project/oathtool). It provides a C++ implementation of generating TOTP using HMAC-SHA1.

## Description

This project implements a Time-based One-Time Password (TOTP) generator in C++.

## Features

- Base32 decoding
- HMAC-SHA1 calculation
- TOTP generation

## Requirements

- C++20 or higher
- OpenSSL library
- CMake
- Ninja (optional)
- GoogleTest (optional) check option ENABLE_TESTS in CMakeLists.txt

## Usage

1. **Clone the repository:**

    ```sh
    git clone https://github.com/yourusername/otp-generator.git
    cd otp-generator
    ```

2. **Compile the code:**

    ```sh
    mkdir build
    cd build
    cmake .. -G Ninja
    ninja
    ```

3. **Run the program:**

    Provide the secret key as the only argument:

    ```sh
    ./otp_generator MZXW6YTBOJUWU23MNU
    ```

    This will output a 6-digit TOTP.

## Example

```sh
./otp_generator MZXW6YTBOJUWU23MNU
123456
```

## How It Works
1. Base32 Decode: Converts the provided Base32 encoded key to a byte array.
2. HMAC-SHA1 Calculation: Computes the HMAC-SHA1 of the current time slice using the decoded key.
3. Hash Truncation: Truncates the hash to obtain a 6-digit OTP.
4. Output: Displays the generated OTP.

## References
* RFC 6238 - TOTP: Time-Based One-Time Password Algorithm
* RFC 2104 - HMAC: Keyed-Hashing for Message Authentication
* oathtool Python Package

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgements
Special thanks to the authors of the oathtool Python package for providing the inspiration and reference implementation for this project.
ChatGPT for generating this README.md file.
