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
    joltages.push_back(0); // starting voltage
    for (std::string line; std::getline(reader, line);) {
        joltages.push_back(stoi(line));
    }

    // add in own adapter
    const int max = *std::max_element(joltages.begin(), joltages.end());
    joltages.push_back(max + 3);

    std::sort(joltages.begin(), joltages.end());

    for (const auto& joltage : joltages) {
        std::cout << joltage << "\n";
    }

    std::vector<int64_t> differences;
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

    std::vector<int64_t> diffs = std::accumulate(
        differences.begin(), differences.end(), std::vector<int64_t>{},
        [counter = 0](auto acc, auto val) mutable {
            if (val == 1) {
                counter++;
            } else {
                if (counter > 1) {
                    acc.push_back(counter);
                }
                counter = 0;
            }
            return acc;
        });

    // routes possible to take
    auto permutations = [](auto input) {
        if (input == 4) {
            return int64_t(7);
        }
        if (input == 3) {
            return int64_t(4);
        }
        if (input == 2) {
            return int64_t(2);
        }
        return int64_t(0);
    };

    auto part2 = std::accumulate(
        diffs.begin(), diffs.end(), int64_t(1),
        [permutations](auto acc, auto val) {
            acc *= permutations(val);
            return acc;
        }
    );

    std::cout << "part2: " << part2 << "\n";

    return 0;
}
