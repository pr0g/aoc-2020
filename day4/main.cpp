#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool validate_input(const std::string& key, const std::string& value)
{
    if (key == "byr") {
        const int year = stoi(value);
        return year >= 1920 && year <= 2002;
    } else if (key == "iyr") {
        const int year = stoi(value);
        return year >= 2010 && year <= 2020;
    } else if (key == "eyr") {
        const int year = stoi(value);
        return year >= 2020 && year <= 2030;
    } else if (key == "hgt") {
        std::string::size_type inches = value.find("in");
        if (inches != std::string::npos) {
            std::string height_str = value.substr(0, inches);
            const int height = stoi(height_str);
            return height >= 59 && height <= 76;
        }
        std::string::size_type cms = value.find("cm");
        if (cms != std::string::npos) {
            std::string height_str = value.substr(0, cms);
            const int height = stoi(height_str);
            return height >= 150 && height <= 193;
        }
        return false;
    } else if (key == "hcl") {
        bool valid = value[0] == '#' && value.size() == 7;
        for (int i = 1; i < value.size(); ++i) {
            bool ok = value[i] >= '0' && value[i] <= 'f';
            valid = ok && valid;
        }
        return valid;
    } else if (key == "ecl") {
        const std::vector<std::string> eye_colors = {
            "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
        };
        return std::any_of(
            eye_colors.cbegin(), eye_colors.cend(),
            [value](const auto& v) { return v == value; });
    } else if (key == "pid") {
        return value.size() == 9 && is_number(value);
    } else if (key == "cid") {
        return true;
    }

    return false;
}

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

    int part1_count = 0;
    int part2_count = 0;
    for (const auto& passport : passports) {
        std::vector<std::pair<std::string, std::string>> entries;
        size_t position = 0;
        for (std::string::size_type separator = passport.find(":", position);
            separator != std::string::npos;) {
            std::string::size_type space = passport.find(" ", position);

            std::string key = passport.substr(position, separator - position);
            std::string value = passport.substr(separator + 1, (space - separator) - 1);

            std::cout << key << "-" << value << "\n";
            entries.push_back(std::make_pair(key, value));

            position = space + 1;
            separator = passport.find(":", position);
        }

        bool valid = true;
        for (const auto key : valid_keys) {
            auto found_key = std::find_if(
                entries.cbegin(), entries.cend(),
                [key](const auto& entry) { return entry.first == key; });
            if (found_key == entries.end()) {
                valid = false;
                break;
            }
        }

        if (valid) {
            bool super_valid = true;
            for (const auto& entry : entries) {
                if (!validate_input(entry.first, entry.second)) {
                    super_valid = false;
                }
            }
            
            part1_count++;

            if (super_valid) {
                part2_count++;
            }
        }

        std::cout << "--\n";
    }

    std::cout << "valid passports (part1): " << part1_count << "\n";
    std::cout << "valid passports (part2): " << part2_count << "\n";

    return 0;
}
