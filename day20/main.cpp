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

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main(int, char**)
{
    std::ifstream reader("input.txt");

    using basis_t = std::vector<char>;
    using grid_t = std::vector<basis_t>;

    enum side_e {
        top,
        right,
        bottom,
        left
    };

    struct tile_t
    {
        int id = -1;
        grid_t grid;

        basis_t sides[4]{};

        bool operator==(const tile_t& tile) const {
            return sides[top] == tile.sides[top] &&
                sides[right] == tile.sides[right] &&
                sides[bottom] == tile.sides[bottom] &&
                sides[left] == tile.sides[left];
        }
    };

    auto rotate_right = [](const tile_t& tile_in) {
        tile_t tile = tile_in;
        basis_t bases[] = {tile.sides[top], tile.sides[right], tile.sides[bottom], tile.sides[left]};
        tile.sides[top] = bases[3];
        std::reverse(tile.sides[top].begin(), tile.sides[top].end());
        tile.sides[right] = bases[0];
        tile.sides[bottom] = bases[1];
        std::reverse(tile.sides[bottom].begin(), tile.sides[bottom].end());
        tile.sides[left] = bases[2];
        return tile;
    };

    auto rotate_left = [](const tile_t& tile_in) {
        tile_t tile = tile_in;
        basis_t bases[] = {tile.sides[top], tile.sides[right], tile.sides[bottom], tile.sides[left]};
        tile.sides[top] = bases[1];
        tile.sides[right] = bases[2];
        std::reverse(tile.sides[right].begin(), tile.sides[right].end());
        tile.sides[bottom] = bases[3];
        tile.sides[left] = bases[0];
        std::reverse(tile.sides[left].begin(), tile.sides[left].end());
        return tile;
    };

    auto flip_horizontal = [](const tile_t& tile_in) {
        tile_t tile = tile_in;
        basis_t bases[] = {tile.sides[right], tile.sides[left]};
        tile.sides[left] = bases[0];
        tile.sides[right] = bases[1];
        std::reverse(tile.sides[top].begin(), tile.sides[top].end());
        std::reverse(tile.sides[bottom].begin(), tile.sides[bottom].end());
        return tile;
    };

    auto flip_vertical = [](const tile_t& tile_in) {
        tile_t tile = tile_in;
        basis_t bases[] = {tile.sides[top], tile.sides[bottom]};
        tile.sides[bottom] = bases[0];
        tile.sides[top] = bases[1];
        std::reverse(tile.sides[left].begin(), tile.sides[left].end());
        std::reverse(tile.sides[right].begin(), tile.sides[right].end());
        return tile;
    };

    enum class mode_e
    {
        title,
        grid
    };

    // each tile is 10x10

    tile_t active_tile;
    mode_e mode = mode_e::title;
    std::vector<tile_t> tiles;
    for (std::string line; std::getline(reader, line);)
    {
        if (line.empty())
        {
            tiles.push_back(active_tile);
            active_tile.grid.clear();
            active_tile.id = -1;
            mode = mode_e::title;
            continue;
        }

        switch (mode)
        {
        case mode_e::title: {
            auto id_off = line.find(' ');
            auto colon_off = line.find(':');
            auto id_str = line.substr(id_off + 1, colon_off - id_off - 1);
            auto id = stoi(id_str);
            active_tile.id = id;
            mode = mode_e::grid;
        }
        break;
        case mode_e::grid: {
            std::vector<char> row;
            for (const auto c : line) {
                row.push_back(c);
            }
            active_tile.grid.push_back(row);
        }
        break;
        }
    }

    tiles.push_back(active_tile);

    // check input
    for (const auto& tile : tiles) {
        std::cout << "tile: " << tile.id << '\n';
        for (const auto& row : tile.grid) {
            for (const auto& col : row) {
                std::cout << col;
            }
            std::cout << '\n';
        }
    }

    for (auto& tile : tiles) {
        tile.sides[top] = tile.grid[0];
        tile.sides[bottom] = tile.grid[9];

        basis_t left, right;
        for (const auto& row : tile.grid) {
            left.push_back(row[0]);
            right.push_back(row[9]);
        }
        tile.sides[side_e::left] = left;
        tile.sides[side_e::right] = right;
    }

    std::deque<tile_t> available_tiles(tiles.begin(), tiles.end());
    std::vector<tile_t> pinned_tiles;

    std::vector<int64_t> corner_ids;
    for (int t = 0; t < available_tiles.size(); ++t) {
         pinned_tiles.push_back(available_tiles.front());
         available_tiles.pop_front();

        std::unordered_map<int, int> matching;
        for (int64_t pinned_tile_index = 0; pinned_tile_index < pinned_tiles.size(); ++pinned_tile_index) {
            for (int64_t tile_index = 0; tile_index < available_tiles.size(); ++tile_index) {
                for (int s = 0; s < 4; ++s) {
                    for (int f = 0; f < 2; ++f) {
                        for (int r = 0; r < 4; ++r) {
                            if (pinned_tiles[pinned_tile_index].sides[s] == available_tiles[tile_index].sides[s]) {
                                matching[tile_index]++;
                            }
                            available_tiles[tile_index] = rotate_right(available_tiles[tile_index]);
                        }
                        available_tiles[tile_index] = flip_horizontal(available_tiles[tile_index]);
                    }
                }
            }
        }

        if (matching.size() == 2) {
            corner_ids.push_back(pinned_tiles.front().id);
        }

        available_tiles.push_back(pinned_tiles.front());
        pinned_tiles.pop_back();
    }

    int64_t product = 1;
    for (const auto id : corner_ids) {
        std::cout << id << '\n';
        product *= id;
    }

    std::cout << "part 1: " << product << '\n';

    return 0;
}
