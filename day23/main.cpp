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

std::list<int64_t> splice_some(
    std::list<int64_t>& list,
    std::list<int64_t>::iterator pos, size_t n) {
    std::list<int64_t> result;

    auto next = pos;
    auto nn = *next;
    for (size_t count = 0; count < n; count++) {
        result.push_back(*next);
        next = std::next(next, 1);
        nn = *next;
        if (next == list.end()) {
            next = list.begin();
        }
    }

    list.erase(std::remove_if(list.begin(), list.end(), [&](const auto cup) {
        return std::find(result.begin(), result.end(), cup) != result.end();
    }), list.end());

    return result;
}

int main(int argc, char **argv)
{
    std::list<int64_t> all_cups{3,8,9,1,2,5,4,6,7}; // test
    std::unordered_map<int64_t, std::list<int64_t>::iterator> cup_lookup;
//    std::list<int64_t> all_cups{4,7,6,1,3,8,2,5,9};
    const auto min_elem = std::min_element(all_cups.begin(), all_cups.end());
    auto max_elem = std::max_element(all_cups.begin(), all_cups.end());

//    all_cups.resize(1000000);
//    int64_t max = *max_elem;
//    for (auto cup_it = std::next(all_cups.begin(), 9);
//        cup_it != all_cups.end(); ++cup_it) {
//        *cup_it = ++max;
//    }

    for (auto cup_it = all_cups.begin(); cup_it != all_cups.end(); ++cup_it) {
        cup_lookup[*cup_it] = cup_it;
    }

    max_elem = std::max_element(all_cups.begin(), all_cups.end());
    std::list<int64_t> cup_circle{all_cups};

    int64_t turns = 0;
    int64_t current_cup = cup_circle.front();
    while (turns < 100) {
//        auto current_cup_it = cup_lookup.find(current_cup);
//        auto next_cup_it = ++current_cup_it->second;
        auto current_cup_it = std::find(cup_circle.begin(), cup_circle.end(), current_cup);
        auto next_cup_it = std::next(current_cup_it, 1);
        if (next_cup_it == cup_circle.end()) {
            next_cup_it = cup_circle.begin();
        }
//        auto t = *next_cup_it;
        std::list<int64_t> held_cups = splice_some(cup_circle, next_cup_it, 3);

//        for (const auto cup : cup_circle) {
//            std::cout << cup << ", ";
//        }
//        std::cout << std::endl;

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

                destination_in_circle = std::next(destination_in_circle, 1);
                if (destination_in_circle == cup_circle.end()) {
                    destination_in_circle = cup_circle.begin();
                }

                cup_circle.insert(
                    destination_in_circle, held_cups.begin(), held_cups.end());

                break;
            }
        }

        current_cup_it = std::find(cup_circle.begin(), cup_circle.end(), current_cup);
        next_cup_it = std::next(current_cup_it, 1);

        if (next_cup_it == cup_circle.end()) {
            next_cup_it = cup_circle.begin();
        }
        current_cup = *next_cup_it;
        turns++;
    }

    auto one = std::find(cup_circle.begin(), cup_circle.end(), 1);

//    auto after_one = ++one;
//    auto another_after_one = ++after_one;
//    int64_t total = *after_one * *another_after_one;

//    std::cout << "after on: " << *after_one
//              << " another after: " << *another_after_one
//              << " total: " << total << '\n';

    size_t counter = 0;
    for (auto offset = std::next(one, 1);
        counter < cup_circle.size() - 1; counter++) {
        std::cout << *offset;
        offset = std::next(offset, 1);
        if (offset == cup_circle.end()) {
            offset = cup_circle.begin();
        }
    }
    std::cout << '\n';

    return 0;
}
