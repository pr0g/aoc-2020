#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <variant>

uint64_t decimal_from_binary(const std::string& value) {
    int64_t acc = 1;
    int64_t result = 0;
    for (auto it = value.rbegin(); it != value.rend(); ++it) {
        auto number = *it - '0';
        if (number > 0) {
            result += acc;
        }
        acc *= 2;
    }
    return result;
}

std::string binary_from_decimal(uint64_t value) {
    std::string binary_representation;
    for (int64_t i = 35; i >= 0; --i) {
        const char b = static_cast<char>((value >> i) & 1u);
        binary_representation.push_back('0' + b);
    }
    return binary_representation;
}

std::string apply_mask(const std::string& mask, const std::string& value) {
    std::string result;
    result.resize(value.size());
    for (int i = 0; i < value.size(); ++i) {
        if (mask[i] == '0') {
            result[i] = '0';
        } else if (mask[i] == '1') {
            result[i] = '1';
        } else {
            result[i] = value[i];
        }
    }
    return result;
}

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct mask_t
{
    std::string mask;
};

struct value_t
{
    int64_t address;
    int64_t value;
};

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    std::vector<std::variant<value_t, mask_t>> instructions;
    for (const auto& line : lines) {
        if (auto str = line.find("mask");
            str != std::string::npos) {
            auto assign = line.find('=');
            auto mask = line.substr(assign + 2, line.size());
            instructions.emplace_back(mask_t{mask});
        } else if (auto str = line.find("mem");
                   str != std::string::npos) {
            auto subscript_end = line.find(']');
            auto address = line.substr(str + 4, subscript_end - (str + 4));
            auto assign = line.find('=');
            auto value = line.substr(assign + 2, lines.size());
            instructions.emplace_back(value_t{stol(address), stol(value)});
        }
    }

    mask_t current_mask;
    std::unordered_map<int32_t, int64_t> memory;
    for (auto& instruction: instructions) {
        std::visit(overloaded {
            [&memory, &current_mask](const value_t& value) {
                std::cout << "addr: " << value.address << " val: " << value.value << "\n";
                auto num = decimal_from_binary(apply_mask(current_mask.mask, binary_from_decimal(value.value)));
                memory[value.address] = num;
            },
            [&memory, &current_mask](const mask_t& mask) {
                std::cout << "mask: " << mask.mask << "\n";
                current_mask = mask;
            },
        }, instruction);
    }

    uint64_t total = 0;
    for (auto address : memory) {
        total += address.second;
    }

    std::cout << "part1: " << total << "\n";

    return 0;
}
