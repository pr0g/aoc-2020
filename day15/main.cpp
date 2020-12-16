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

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    std::unique_ptr<char[]> numbers = std::make_unique<char[]>(lines[0].size() + 1);
    std::strncpy(numbers.get(), lines[0].c_str(), lines[0].size());

    std::vector<std::string> number_strs;
    char* token = std::strtok(numbers.get(), ",");
    while (token != nullptr) {
        number_strs.emplace_back(token);
        token = std::strtok(nullptr, ",");
    }

    std::vector<int> nums;
    nums.reserve(number_strs.size());
    for (const auto& number : number_strs) {
        nums.push_back(stoi(number));    
    }
    
    for (auto number : nums) {
        std::cout << number << "\n";
    }

    struct info_t
    {
        int64_t offsets[2] = {-1, -1};
    };

    std::unordered_map<int64_t, info_t> number_to_offset;
    std::unordered_map<int64_t, int64_t> offset_to_number;

    for (int64_t i = 0; i < nums.size(); ++i) {
        number_to_offset.insert({nums[i], {{-1, i}}});
        offset_to_number.insert({i, nums[i]});
    }

    for (int64_t i = nums.size(); i < 30000000; ++i) {
        auto prev_number = offset_to_number[i - 1];
        auto info = number_to_offset[prev_number];
            if (info.offsets[0] != -1 && info.offsets[1] != -1) {
                auto next = info.offsets[1] - info.offsets[0];
                auto off = number_to_offset[next];
                number_to_offset.erase(next);
                off.offsets[0] = off.offsets[1];
                off.offsets[1] = i;
                number_to_offset.insert({next, off});
                offset_to_number[i] = next;
        } else {
                auto off = number_to_offset[0];
                number_to_offset.erase(0);
                off.offsets[0] = off.offsets[1];
                off.offsets[1] = i;
                number_to_offset.insert({0, off});
                offset_to_number[i] = 0;
        }
    }

    std::cout << "part1: " << offset_to_number[2019] << "\n";
    std::cout << "part2: " << offset_to_number[29999999] << "\n";

    return 0;
}
