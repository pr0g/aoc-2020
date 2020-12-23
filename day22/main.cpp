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

int main(int argc, char **argv)
{
    std::ifstream reader("input.txt");

    std::deque<int64_t> player1;
    std::deque<int64_t> player2;

    std::deque<int64_t>* player = &player1;

    std::vector<std::string> lines;
    for (std::string line; std::getline(reader, line);) {
        if (line.find("Player") != std::string::npos) {
            continue;
        }
        if (line.empty()) {
            player = &player2;
            continue;
        }
        player->push_back(stol(line));
    }

    for (const int64_t card : player1) {
        std::cout << card << '\n';
    }

    std::cout << "---\n";

    for (const int64_t card : player2) {
        std::cout << card << '\n';
    }

    while (!player1.empty() && !player2.empty()) {
        auto p1_card = player1.front();
        player1.pop_front();
        auto p2_card = player2.front();
        player2.pop_front();

        if (p1_card > p2_card) {
            player1.push_back(p1_card);
            player1.push_back(p2_card);
        } else {
            player2.push_back(p2_card);
            player2.push_back(p1_card);
        }
    }

    if (player1.size() > player2.size()) {
        player = &player1;
    } else {
        player = &player2;
    }

    int64_t mul = 1;
    int64_t score = 0;
    for (auto b = player->rbegin(); b != player->rend(); ++b) {
        score += *b * mul;
        mul++;
    }

    std::cout << "part1: " << score << '\n';

    return 0;
}
