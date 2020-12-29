#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <functional>
#include <queue>
#include <list>

// use unordered_map with key/value as index and neighbor

std::vector<int64_t> splice_map(
    std::unordered_map<int64_t, int64_t>& cup_circle,
    int64_t lookup, int64_t count) {
    auto insert = cup_circle.find(lookup);
    std::vector<int64_t> result;
    auto next = cup_circle.find(insert->second);
    for (int64_t i = 0; i < count; ++i) {
        result.push_back(next->first);
        next = cup_circle.find(next->second);
    }
    insert->second = next->first;
    return result;
}

int main(int argc, char **argv)
{
    // test input
//    std::unordered_map<int64_t, int64_t> all_cups{{3, 8},{8, 9},{9, 1},{1, 2},{2, 5},{5, 4},{4, 6},{6, 7},{7, 3}};
    std::unordered_map<int64_t, int64_t> all_cups{{4,7},{7,6},{6,1},{1,3},{3,8},{8,2},{2,5},{5,9},{9,4}};
    const auto min_elem = std::min_element(all_cups.begin(), all_cups.end());
    auto max_elem = std::max_element(all_cups.begin(), all_cups.end());

    int64_t max = max_elem->first;
    auto last = all_cups.find(9);
    for (auto i = 0; i < 1000000 - 9; ++i) {
        all_cups[last->first] = ++max;
        all_cups.insert({max, -1});
        last = all_cups.find(max);
    }
    last->second = 4;

    max_elem = std::max_element(all_cups.begin(), all_cups.end());
    std::unordered_map<int64_t, int64_t> cup_circle{all_cups};

    int64_t turns = 0;
    int64_t current_cup = 4;
    while (turns < 10000000) {
        auto current_cup_it = cup_circle.find(current_cup);
        std::vector<int64_t> held_cups = splice_map(cup_circle, current_cup, 3);

        auto cup = cup_circle.find(current_cup);

        auto destination_cup = current_cup - 1;
        while (true) {
            auto found_in_held = std::find(
                held_cups.begin(), held_cups.end(), destination_cup);
            if (found_in_held != held_cups.end() || destination_cup == 0) {
                destination_cup--;
                if (destination_cup < min_elem->first) {
                    destination_cup = max_elem->first;
                }
            } else {
                auto destination_in_circle = cup_circle.find(destination_cup);
                auto dnc = destination_in_circle->second;

                auto old_next = destination_in_circle;
                auto next = old_next;
                next->second = held_cups[0];
                auto last_held_cup = cup_circle.find(held_cups[2]);
                last_held_cup->second = dnc;
                break;
            }
        }

        current_cup_it = cup_circle.find(current_cup);
        current_cup = current_cup_it->second;
        turns++;
    }

    auto one = cup_circle.find(1);

    std::vector<int64_t> result;
    auto next = cup_circle.find(one->second);
    for (int64_t i = 0; i < 2; ++i) {
        result.push_back(next->first);
        next = cup_circle.find(next->second);
    }

    int64_t r = result[0] * result[1];
    std::cout << "part 2: " << r << '\n';

    for (auto i : result) {
        std::cout << i << "\n";
    }
    std::cout << '\n';

    return 0;
}
