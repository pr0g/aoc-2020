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
#include <functional>

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct coord_t
{
    int x, y, z;
};

bool operator==(const coord_t& lhs, const coord_t& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

coord_t operator+(const coord_t& lhs, const coord_t& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    std::vector<std::vector<char>> symbol_grid;
    for (const auto& line : lines) {
        std::vector<char> symbols;
        for (const auto symbol : line) {
            symbols.push_back(symbol);
        }
        symbol_grid.push_back(symbols);
    }

//    for (auto& i : symbol_grid) {
//        for (char j : i) {
//            std::cout << j;
//        }
//        std::cout << "\n";
//    }

    struct coord_hash_t
    {
        std::size_t operator()(const coord_t& coord) const noexcept
        {
            std::size_t seed;
            hash_combine(seed, coord.x);
            hash_combine(seed, coord.y);
            hash_combine(seed, coord.z);
            return seed;
        }
    };

    auto on = [](const char symbol) {
        return symbol != '.';
    };

    using grid_t = std::unordered_map<coord_t, bool, coord_hash_t>;
    grid_t grid;
    for (int y = 0; y < symbol_grid.size(); ++y) {
        for (int x = 0; x < symbol_grid[y].size(); ++x) {
            const char c = symbol_grid[y][x];
            grid.insert({coord_t{x, y, 0}, on(c)});
        }
    }

    for (int z = -1; z <= 1; ++z) {
        for (int y = -1; y <= (int)symbol_grid.size(); ++y) {
            for (int x = -1; x <= (int)symbol_grid.size(); ++x) {
                auto c = grid.find(coord_t{x, y, z});
                if (c == grid.end()) {
                    grid.insert({coord_t{x, y, z}, false});
                }
            }
        }
    }
    
    auto process_fn =
        [](grid_t& grid) {
            grid_t grid_copy = grid;
            coord_t offsets[] = {
                {1, 0, 0},
                {1, -1, 0},
                {0, -1, 0},
                {-1, -1, 0},
                {-1, 0, 0},
                {-1, 1, 0},
                {0, 1, 0},
                {1, 1, 0},
                //
                {0, 0, 1},
                {1, 0, 1},
                {1, -1, 1},
                {0, -1, 1},
                {-1, -1, 1},
                {-1, 0, 1},
                {-1, 1, 1},
                {0, 1, 1},
                {1, 1, 1},
                //
                {0, 0, -1},
                {1, 0, -1},
                {1, -1, -1},
                {0, -1, -1},
                {-1, -1, -1},
                {-1, 0, -1},
                {-1, 1, -1},
                {0, 1, -1},
                {1, 1, -1}
            };

        for (const auto& coord : grid) {
            int neighbor_count = 0;
            for (const auto& offset : offsets) {
                auto neighbor = coord.first + offset;
                if (auto it = grid.find(neighbor);
                    it != grid.end()) {
                    if (it->second) {
                        neighbor_count++;
                    }
                } else {
                    grid_copy.insert({neighbor, false});
                }
            }
            if (auto it = grid_copy.find(coord.first);
                it != grid_copy.end()) {
                if (it->second) {
                    if (neighbor_count == 2 || neighbor_count == 3) {
                        it->second = true;
                    } else {
                        it->second = false;
                    }
                } else {
                    if (neighbor_count == 3) {
                        it->second = true;
                    }
                }
            }
        }

        return grid_copy;
    };

    auto next_grid = grid;
    for (int i = 0; i < 6; ++i) {
        next_grid = process_fn(next_grid);
    }

    int active = 0;
    for (const auto coord : next_grid) {
        if (coord.second) {
            active++;
        }
    }

    std::cout << "part1 : " << active << "\n";

    return 0;
}
