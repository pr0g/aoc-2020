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
    auto t = insert->first;
    auto tt = insert->second;
    auto nc = next->first;
    auto nn = next->second;
    insert->second = next->first;
    return result;
}

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
    // test input
//    std::unordered_map<int64_t, int64_t> all_cups{{3, 8},{8, 9},{9, 1},{1, 2},{2, 5},{5, 4},{4, 6},{6, 7},{7, 3}};
    //                                             4     7     6     1     3     8     2    5      9
    std::unordered_map<int64_t, int64_t> all_cups{{4,7},{7,6},{6,1},{1,3},{3,8},{8,2},{2,5},{5,9},{9,4}};
    const auto min_elem = std::min_element(all_cups.begin(), all_cups.end());
    auto max_elem = std::max_element(all_cups.begin(), all_cups.end());
    auto tom = max_elem->first;
    auto tom2 = max_elem->second;

    int64_t max = max_elem->first;
    auto last = all_cups.find(9);
    for (auto i = 0; i < 1000000 - 9; ++i) {
        all_cups[last->first] = ++max;
        all_cups.insert({max, -1});
        last = all_cups.find(max);
    }
    last->second = 4;

//    for (auto cup_it = all_cups.begin(); cup_it != all_cups.end(); ++cup_it) {
//        cup_lookup[*cup_it] = cup_it;
//    }

    max_elem = std::max_element(all_cups.begin(), all_cups.end());
    std::unordered_map<int64_t, int64_t> cup_circle{all_cups};

    int64_t turns = 0;
    int64_t current_cup = 4; //cup_circle.front();
    while (turns < 10000000) {
//        auto current_cup_it = cup_lookup.find(current_cup);
//        auto next_cup_it = ++current_cup_it->second;
        auto current_cup_it = cup_circle.find(current_cup); //std::find(cup_circle.begin(), cup_circle.end(), current_cup);
//        auto next_cup_it = cup_circle.find(current_cup_it->second);// std::next(current_cup_it, 1);
        auto cc = current_cup_it->first;
        auto nc = current_cup_it->second;
//        auto nc1 = next_cup_it->first;
//        auto nc2 = next_cup_it->second;
//        if (next_cup_it == cup_circle.end()) {
//            next_cup_it = cup_circle.begin();
//        }
//        auto t = *next_cup_it;
        std::vector<int64_t> held_cups = splice_map(cup_circle, current_cup, 3); //splice_some(cup_circle, next_cup_it, 3);

        auto cup = cup_circle.find(current_cup);
//        for (int64_t c = 0; c < cup_circle.size() - held_cups.size(); ++c) {
//            std::cout << cup->first << ", ";
//            cup = cup_circle.find(cup->second);
//        }
//        std::cout << std::endl;

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
                if (destination_cup < min_elem->first) {
                    destination_cup = max_elem->first;
                }
            } else {
                auto destination_in_circle = cup_circle.find(destination_cup);
//                destination_in_circle = cup_circle.find(destination_in_circle->second);

                auto dc = destination_in_circle->first;
                auto dnc = destination_in_circle->second;

                // auto ins = cup_circle.find(destination_in_circle->first);
                auto old_next = destination_in_circle;
                auto next = old_next;
                next->second = held_cups[0];
                auto last_held_cup = cup_circle.find(held_cups[2]);
                auto t1 = last_held_cup->first;
                auto tt1 = last_held_cup->second;
                auto t = old_next->first;
                auto tt = old_next->second;
                last_held_cup->second = dnc;//old_next->second;
                auto t2 = last_held_cup->first;
                auto tt2 = last_held_cup->second;
//                for (const auto held_cup : held_cups) {
//                    next = cup_circle.find(destination_in_circle->second);
//                }

                cup = cup_circle.find(current_cup);
//                for (int64_t c = 0; c < cup_circle.size(); ++c) {
//                    std::cout << cup->first << ", ";
//                    cup = cup_circle.find(cup->second);
//                }
//                std::cout << std::endl;

//                cup_circle.insert(
//                    destination_in_circle, held_cups.begin(), held_cups.end());

                break;
            }
        }

        current_cup_it = cup_circle.find(current_cup);
//        next_cup_it = cup_circle.find(current_cup_it->second);

//        current_cup_it = std::find(cup_circle.begin(), cup_circle.end(), current_cup);
//        next_cup_it = std::next(current_cup_it, 1);

//        if (next_cup_it == cup_circle.end()) {
//            next_cup_it = cup_circle.begin();
//        }
        current_cup = current_cup_it->second;
        turns++;
    }

    auto one = cup_circle.find(1); //std::find(cup_circle.begin(), cup_circle.end(), 1);

//    auto after_one = ++one;
//    auto another_after_one = ++after_one;
//    int64_t total = *after_one * *another_after_one;

//    std::cout << "after on: " << *after_one
//              << " another after: " << *another_after_one
//              << " total: " << total << '\n';

    std::vector<int64_t> result;
    auto next = cup_circle.find(one->second);
    for (int64_t i = 0; i < 2; ++i) {
        result.push_back(next->first);
        next = cup_circle.find(next->second);
    }

    int64_t r = result[0] * result[1];
    std::cout << r << '\n';

    for (auto i : result) {
        std::cout << i << "\n";
    }
    std::cout << '\n';

//    size_t counter = 0;
//    for (auto offset = std::next(one, 1);
//        counter < cup_circle.size() - 1; counter++) {
//        std::cout << *offset;
//        offset = std::next(offset, 1);
//        if (offset == cup_circle.end()) {
//            offset = cup_circle.begin();
//        }
//    }
//    std::cout << '\n';

    return 0;
}
