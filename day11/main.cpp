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

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    for (const auto& line : lines) {
        std::cout << line << "\n";
    }

    std::vector<std::vector<char>> grid;
    for (const auto& line : lines) {
        grid.emplace_back();
        for (const auto& symbol : line) {
            grid.back().push_back(symbol);
        }
    }

    std::cout << "---\n";

    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid.back().size(); ++col) {
            std::cout << grid[row][col];
        }
        std::cout << "\n";
    }

    struct offset_t {
        int x;
        int y;
    };

    struct change_t {
        int row;
        int col;
        char symbol;
    };

    auto adjacent_seats_fn = [&grid](int row, int col, int width, int height) {
        offset_t offsets[] = {
            {0, 1}, {1, 1}, {1, 0},
            {1, -1}, {0, -1}, {-1, -1},
            {-1, 0}, {-1, 1}};

        std::vector<change_t> seats;
        offset_t start{row, col};
        for (const auto offset : offsets) {
            const offset_t next_offset = {start.x + offset.x, start.y + offset.y};
            if (next_offset.x >= 0 && next_offset.x < width
                && next_offset.y >= 0 && next_offset.y < height) {
                seats.push_back(change_t{next_offset.x, next_offset.y, grid[row][col]});
            }
        }

        return seats;
    };

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
            if (grid[row][col] == '#' && occupied >= 4) {
                changes.push_back(change_t{row, col, 'L'});
            }
        }
    }

    for (const auto change : changes) {
        grid[change.row][change.col] = change.symbol;
    }

    std::cout << "---\n";

    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid.back().size(); ++col) {
            std::cout << grid[row][col];
        }
        std::cout << "\n";
    }

    return 0;
}
