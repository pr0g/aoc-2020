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

std::string apply_mask_v2(const std::string& mask, const std::string& value) {
    std::string result;
    result.resize(value.size());
    for (int i = 0; i < value.size(); ++i) {
        if (mask[i] == '0') {
            result[i] = value[i];
        } else if (mask[i] == '1') {
            result[i] = '1';
        } else {
            result[i] = 'X';
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
    std::unordered_map<int64_t, int64_t> memory;
    for (auto& instruction: instructions) {
        std::visit(overloaded {
            [&memory, &current_mask](const value_t& value) {
                const auto floating = apply_mask_v2(current_mask.mask, binary_from_decimal(value.address));
                auto x_count = std::count_if(floating.begin(), floating.end(), [](const char c){ return c == 'X'; });

                const auto iterations = std::pow(2, x_count);
                for (uint64_t i = 0; i < iterations; ++i) {
                    auto next_floating = floating;
                    std::string str = binary_from_decimal(i);

                    int64_t ns = str.size() - 1;
                    for (int64_t s = floating.size() - 1; s >= 0; --s) {
                        if (floating[s] == 'X') {
                            next_floating[s] = str[ns--];
                        }
                    }
                    auto addr = decimal_from_binary(next_floating);
                    memory[addr] = value.value;
                }
            },
            [&current_mask](const mask_t& mask) {
                current_mask = mask;
            },
        }, instruction);
    }

    uint64_t total = 0;
    for (auto address : memory) {
        total += address.second;
    }

    std::cout << "part2: " << total << "\n";

    return 0;
}
