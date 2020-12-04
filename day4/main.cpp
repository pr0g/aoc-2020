#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> passports;
    std::string passport;
    for (std::string line; std::getline(reader, line);) {
        if (line.empty()) {
            passports.push_back(passport);
            passport.clear();
            continue;
        }
        passport += line;
        passport += " ";
    }

    passports.push_back(passport);

    // input check
    for (const auto& passport : passports) {
        std::cout << passport << "\n";
    }

    std::cout << "-----\n";

    // "cid" - optional
    const std::vector<std::string> valid_keys = {
        "byr", "iyr", "eyr", "hgt",
        "hcl", "ecl", "pid"
    };

    int valid_count = 0;
    for (const auto& passport : passports) {
        std::vector<std::string> keys;
        size_t position = 0;
        for (std::string::size_type separator = passport.find(":", position);
            separator != std::string::npos;) {
            std::string::size_type space = passport.find(" ", position);

            std::string key = passport.substr(position, separator - position);
            std::string value = passport.substr(separator + 1, space - separator);

            std::cout << key << "-" << value << "\n";
            keys.push_back(key);

            position = space + 1;
            separator = passport.find(":", position);
        }

        bool valid = true;
        for (const auto key : valid_keys) {
            auto found_key = std::find(keys.cbegin(), keys.cend(), key);
            if (found_key == keys.end()) {
                valid = false;
                break;
            }
        }

        if (valid) {
            valid_count++;
        }

        std::cout << "--\n";
    }

    std::cout << "valid passports: " << valid_count << "\n";

    return 0;
}
