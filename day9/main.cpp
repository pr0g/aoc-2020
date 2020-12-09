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

using number_t = std::vector<int>;

void print(const number_t& num) {
    if (num.empty()) {
        return;
    }
    for (int i = 0; i < num.size(); ++i) {
        std::cout << num[i];
    }
    std::cout << "\n";
}

number_t add(number_t lhs, number_t rhs) {
    std::reverse(lhs.begin(), lhs.end());
    std::reverse(rhs.begin(), rhs.end());

    auto safe_num = [](number_t num, int digit) {
        if (digit < num.size()) {
            return num[digit];
        }
        return 0;
    };
    
    int len = std::max(lhs.size(), rhs.size());
    number_t result;
    result.resize(len);
    int carry = 0;
    for (int i = 0; i < len; ++i) {
        int sum = safe_num(lhs,i) + safe_num(rhs, i) + carry;
        carry = 0;
        if (sum >= 10) {
            int r = sum % 10;
            int c = sum / 10;
            result[i] = r;
            carry = c;
            if (i == len - 1) {
                result.push_back(0);
                len++;
            }
        } else {
            result[i] = sum;
        }
    }
    std::reverse(result.begin(), result.end());
    result.shrink_to_fit();
    return result;
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<number_t> xmas;
    for(std::string line; std::getline(reader, line);) {
        number_t num;
        for (const auto c : line) {
            num.push_back(c - '0');
        }
        xmas.push_back(num);
    }

    for (const auto& number : xmas) {
        print(number);
    }

    const int64_t preamble = 25;

    int64_t offset = 0;
    for (int64_t slide = preamble; slide < xmas.size() - preamble; ++slide) {
        bool found_sum = false;
        for (int64_t i = slide - preamble; i < slide + preamble; ++i) {
            for (int64_t j = i + 1; j < slide + preamble; ++j) {
                auto total = add(xmas[i], xmas[j]);
                if (total == xmas[slide]) {
                    found_sum = true;
                    goto end;
                }
            }
        }
end:
        if (!found_sum) {
            offset = slide;
        }
    }

    std::cout << "part1: ";
    print(xmas[offset]);

    return 0;
}
