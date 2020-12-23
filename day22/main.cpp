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

    struct round_t {
        std::deque<int64_t> player1;
        std::deque<int64_t> player2;
    };

    enum class winner_e {
        none,
        player1,
        player2
    };

    winner_e winner = winner_e::none;
    std::vector<round_t> rounds;
    while (!player1.empty() && !player2.empty()) {
        if (!rounds.empty()) {
            if (auto found = std::find_if(rounds.begin(), rounds.end(), [&](const round_t& round) {
                return round.player1 == player1 && round.player2 == player2;
            }); found != rounds.end()) {
                winner = winner_e::player1;
                break;
            }
        }

        rounds.push_back({player1, player2});

        // draw top card as normal
        auto p1_card = player1.front();
        player1.pop_front();
        auto p2_card = player2.front();
        player2.pop_front();

        if (player1.size() >= p1_card && player2.size() > p2_card) {
            auto player1_copy = std::vector<int64_t>{};
            std::copy_n(player1.begin(), p1_card, std::back_inserter(player1_copy));
            auto player2_copy = std::vector<int64_t>{};
            std::copy_n(player2.begin(), p2_card, std::back_inserter(player2_copy));
            // recursive combat

        } else {
            if (p1_card > p2_card) {
                player1.push_back(p1_card);
                player1.push_back(p2_card);
            } else {
                player2.push_back(p2_card);
                player2.push_back(p1_card);
            }
        }
    }

    if (winner == winner_e::player1) {
        player = &player1;
    } else if (winner == winner_e::player2) {
        player = &player2;
    } else {
        int i;
        i = 0; // oh no
    }

//    part 1
//    if (player1.size() > player2.size()) {
//        player = &player1;
//    } else {
//        player = &player2;
//    }

    int64_t mul = 1;
    int64_t score = 0;
    for (auto b = player->rbegin(); b != player->rend(); ++b) {
        score += *b * mul;
        mul++;
    }

    std::cout << "part1: " << score << '\n';

    return 0;
}
