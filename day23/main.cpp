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

std::vector<int64_t> wrap_transfer_n(std::vector<int64_t>& vec, size_t index, size_t n) {
    std::vector<int64_t> result;
    for (size_t count = 0; count < n; count++) {
        size_t lookup = (index + count) % vec.size();
        result.push_back(vec[lookup]);
    }
    vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const auto cup) {
        return std::find(result.begin(), result.end(), cup) != result.end();
    }), vec.end());
    return result;
}

int main(int argc, char **argv)
{
//    const std::vector<int64_t> all_cups{3,8,9,1,2,5,4,6,7}; // test
    const std::vector<int64_t> all_cups{4,7,6,1,3,8,2,5,9};
    const auto min_elem = std::min_element(all_cups.begin(), all_cups.end());
    const auto max_elem = std::max_element(all_cups.begin(), all_cups.end());
    
    // puzzle - 476138259
    std::vector<int64_t> cup_circle{all_cups};

    for (const auto& cup : cup_circle) {
        std::cout << cup << '\n';
    }

    std::cout << "---\n";

    int64_t turns = 0;
    int64_t current_cup = cup_circle[0];
    while (turns < 100) {
        auto current_cup_it = std::find(
            cup_circle.begin(), cup_circle.end(), current_cup);

        std::vector<int64_t> held_cups = wrap_transfer_n(
            cup_circle, std::distance(cup_circle.begin(), current_cup_it + 1), 3);

        auto destination_cup = current_cup - 1;
        while (true) {
            auto found_in_held = std::find(
                held_cups.begin(), held_cups.end(), destination_cup);
            if (found_in_held != held_cups.end() || destination_cup == 0) {
                destination_cup--;
                if (destination_cup < *min_elem) {
                    destination_cup = *max_elem;
                }
            } else {
                auto destination_in_circle = std::find(
                    cup_circle.begin(), cup_circle.end(), destination_cup);

                auto dist = std::distance(cup_circle.begin(), destination_in_circle);

                if (dist >= cup_circle.size()) {
                    destination_in_circle = cup_circle.end();
                } else {
                    destination_in_circle = destination_in_circle + 1;
                }

                auto cup_circle_before = cup_circle;
                cup_circle.insert(destination_in_circle, held_cups.begin(), held_cups.end());

                break;
            }
        }

        current_cup_it = std::find(
            cup_circle.begin(), cup_circle.end(), current_cup);

        auto next_cup_it = current_cup_it + 1;
        if (next_cup_it == cup_circle.end()) {
            next_cup_it = cup_circle.begin();
        }
        current_cup = *next_cup_it;
        turns++;
    }

    auto counter = 0;
    auto one = std::find(cup_circle.begin(), cup_circle.end(), 1);
    for (const auto offset = std::distance(cup_circle.begin(), one) + 1;
        counter < cup_circle.size() - 1; counter++) {
        std::cout << cup_circle[(offset + counter) % cup_circle.size()];
    }

    std::cout << '\n';

    return 0;
}
