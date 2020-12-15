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

    std::vector<int> numbers_offsets;
    numbers_offsets.reserve(nums.size());
    for (int num : nums) {
        numbers_offsets.push_back(num);
    }

    for (int i = nums.size(); i < 2020; ++i) {
        auto prev_number = numbers_offsets[i - 1];
        auto repeats = std::count(numbers_offsets.begin(), numbers_offsets.end(), prev_number);
        if (repeats > 1) {
            auto n1 = std::find(numbers_offsets.rbegin(), numbers_offsets.rend(), prev_number);
            auto n2 = std::find(n1 + 1, numbers_offsets.rend(), prev_number);
            auto off1 = std::distance(n1, numbers_offsets.rend());
            auto off2 = std::distance(n2, numbers_offsets.rend());
            auto next = off1 - off2;
            numbers_offsets.push_back(next);
        } else {
            numbers_offsets.push_back(0);
        }
    }

    std::cout << "part1: " << numbers_offsets[2019] << "\n";

    return 0;
}
