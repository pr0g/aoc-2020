#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> reservations;
    for(std::string line; std::getline(reader, line);) {
        reservations.push_back(line);
    }

    // input check
    std::copy(
        reservations.begin(), reservations.end(),
        std::ostream_iterator<std::string>(std::cout, "\n"));

    struct Partitioner
    {
        int lower;
        int upper;

        void split_lower() {
            upper = lower + ((upper - lower) / 2);
        }

        void split_upper() {
            lower = lower + ((upper - lower + 1) / 2);
        }
    };

    std::vector<int> ids;
    for (const auto& reservation : reservations) {
        Partitioner row{0, 127};
        Partitioner seat{0, 8};

        for (const auto& code : reservation) {
            if (code == 'F') {
                // partition front half (0-63)
                row.split_lower();
            } else if (code == 'B') {
                // partition lower half (64-127)
                row.split_upper();
            } else if (code == 'R') {
                seat.split_upper();
            } else if (code == 'L') {
                seat.split_lower();
            }
        }

        ids.push_back((row.upper * 8) + seat.lower);
    }

    std::cout << "part1: " << *std::max_element(ids.begin(), ids.end()) << "\n";

    std::sort(ids.begin(), ids.end());
    if (auto it = std::adjacent_find(ids.begin(), ids.end(), [](const auto& lhs, const auto& rhs){
        return (rhs - lhs) > 1;
    }); it != ids.end()) {
        std::cout << "part2: " << *it + 1 << "\n";
    }

    return 0;
}
