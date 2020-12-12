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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    std::vector<std::vector<char>> grid;
    for (const auto& line : lines) {
        grid.emplace_back();
        for (const auto& symbol : line) {
            grid.back().push_back(symbol);
        }
    }

    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid.back().size(); ++col) {
            std::cout << grid[row][col];
        }
        std::cout << "\n";
    }

    struct offset_t {
        int r;
        int c;
    };

    struct change_t {
        int row;
        int col;
        char symbol;
    };

    auto adjacent_seats_fn = [&grid](int row, int col, int width, int height) {
        std::vector<offset_t> offsets = {
            {0, 1}, {1, 1}, {1, 0},
            {1, -1}, {0, -1}, {-1, -1},
            {-1, 0}, {-1, 1}};

        std::vector<change_t> seats;
        offset_t start{row, col};
        // part 2 - multiple iterations (part 1 only one)
        while (!offsets.empty()) {
            for (auto it = offsets.begin(); it != offsets.end();) {
                auto offset = *it;
                const offset_t next_offset = {start.r + offset.r, start.c + offset.c};
                if (next_offset.r >= 0 && next_offset.r < height
                    && next_offset.c >= 0 && next_offset.c < width) {
                    if (grid[next_offset.r][next_offset.c] == '#') {
                        seats.push_back(
                            change_t{next_offset.r, next_offset.c,
                            grid[next_offset.r][next_offset.c]});
                        it = offsets.erase(it);
                    } else if (grid[next_offset.r][next_offset.c] == 'L') {
                        it = offsets.erase(it);
                    } else {
                        it++;
                    }
                } else {
                    it = offsets.erase(it);
                }
            }

            for (auto& offset : offsets) {
                offset.r += sgn(offset.r);
                offset.c += sgn(offset.c);
            }
        }
        return seats;
    };

    int iterations = 0;
    for (bool iterating = true; iterating;) {
        std::vector<change_t> changes;
        for (int row = 0; row < grid.size(); ++row) {
            for (int col = 0; col < grid.back().size(); ++col) {
                const auto adjacent_seats =
                    adjacent_seats_fn(row, col, grid.back().size(), grid.size());
                const auto occupied = std::count_if(
                    adjacent_seats.begin(), adjacent_seats.end(),
                    [](const auto change) {
                    return change.symbol == '#';
                });
                if (grid[row][col] == 'L' && occupied == 0) {
                    changes.push_back(change_t{row, col, '#'});
                }
                // part 1 (>= 4), part 2 (>= 5)
                if (grid[row][col] == '#' && occupied >= 5) {
                    changes.push_back(change_t{row, col, 'L'});
                }
            }
        }

        for (const auto change : changes) {
            grid[change.row][change.col] = change.symbol;
        }

        // std::cout << "---\n";

        // // debug
        // for (int row = 0; row < grid.size(); ++row) {
        //     for (int col = 0; col < grid.back().size(); ++col) {
        //         std::cout << grid[row][col];
        //     }
        //     std::cout << "\n";
        // }

        if (changes.empty()) {
            iterating = false;
        } else {
            iterations++;
        }
    }

    std::cout << "---\n";

    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid.back().size(); ++col) {
            std::cout << grid[row][col];
        }
        std::cout << "\n";
    }

    int occupied_seats = 0;
    for (auto & row : grid) {
        occupied_seats += std::count_if(
            row.begin(), row.end(),
            [](const auto change) {
            return change == '#';
        });
    }

    std::cout << "---\n";

    std::cout << "iterations: " << iterations << "\n";
    std::cout << "part2: " << occupied_seats << "\n";

    return 0;
}
