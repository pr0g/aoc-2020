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

    std::vector<int> joltages;
    for (std::string line; std::getline(reader, line);) {
        joltages.push_back(stoi(line));
    }

    for (const auto& joltage : joltages) {
        std::cout << joltage << "\n";
    }

    // add in own adapter
    const int max = *std::max_element(joltages.begin(), joltages.end());
    joltages.push_back(max + 3);

    std::sort(joltages.begin(), joltages.end());
    std::vector<int> differences;
    std::adjacent_difference(
        joltages.begin(), joltages.end(), std::back_inserter(differences));

    std::cout << "---\n";

    for (const auto& difference : differences) {
        std::cout << difference << "\n";
    }

    const auto ones = std::count_if(
        differences.begin(), differences.end(), [](const auto diff) {
            return diff == 1;
        });

    const auto threes = std::count_if(
        differences.begin(), differences.end(), [](const auto diff) {
            return diff == 3;
        });

    std::cout << "ones: " << ones << "\n";
    std::cout << "threes: " << threes << "\n";

    std::cout << "part1: " << ones * threes  << "\n";

    return 0;
}
