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

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

enum class direction_e {
    east,
    north_east,
    south_east,
    west,
    north_west,
    south_west
};

struct offset_t {
    int horizontal = 0;
    int vertical = 0;
};

bool operator==(const offset_t& lhs, const offset_t& rhs) {
    return lhs.horizontal == rhs.horizontal && lhs.vertical == rhs.vertical;
}

offset_t operator+(const offset_t& lhs, const offset_t& rhs) {
    return {lhs.horizontal + rhs.horizontal, lhs.vertical + rhs.vertical};
}

int main(int, char**)
{
    std::ifstream reader("input.txt");
    std::vector<std::string> lines;
    for (std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    using directions_t = std::vector<direction_e>;
    using all_directions_t = std::vector<directions_t>;
    
    all_directions_t all_directions;
    for (const auto& line : lines) {
        directions_t directions;
        std::string buffer;
        for (const auto symbol : line) {
            if (symbol == 'e' && buffer.empty()) {
                directions.push_back(direction_e::east);
            } else if (symbol == 'w' && buffer.empty()) {
                directions.push_back(direction_e::west);
            } else if (symbol == 's' || symbol == 'n') {
                buffer.push_back(symbol);
            } else if (symbol == 'w' && !buffer.empty() ||
                       symbol == 'e' && !buffer.empty()) {
                if (buffer[0] == 's' && symbol == 'w') {
                    directions.push_back(direction_e::south_west);
                } else if (buffer[0] == 's' && symbol == 'e') {
                    directions.push_back(direction_e::south_east);
                } else if (buffer[0] == 'n' && symbol == 'w') {
                    directions.push_back(direction_e::north_west);
                } else if (buffer[0] == 'n' && symbol == 'e') {
                    directions.push_back(direction_e::north_east);
                }
                buffer.clear();
            }
        }
        all_directions.push_back(directions);
    }

    for (const auto& directions : all_directions) {
        for (const auto direction : directions) {
            switch (direction) {
            case direction_e::east:
                std::cout << "e";
                break;
            case direction_e::west:
                std::cout << "w";
                break;
            case direction_e::south_east:
                std::cout << "se";
                break;
            case direction_e::north_east:
                std::cout << "ne";
                break;
            case direction_e::north_west:
                std::cout << "nw";
                break;
            case direction_e::south_west:
                std::cout << "sw";
                break;
            }
        }
        std::cout << "\n";
    }

    struct offset_hash_t {
        std::size_t operator()(const offset_t& offset) const {
            std::size_t seed;
            hash_combine(seed, offset.horizontal);
            hash_combine(seed, offset.vertical);
            return seed;
        }
    };

    std::unordered_map<offset_t, bool, offset_hash_t> tiles;
    for (const auto& directions : all_directions) {
        offset_t offset;
        for (const auto direction : directions) {
            switch (direction) {
            case direction_e::east:
                offset.horizontal += 2;
                break;
            case direction_e::west:
                offset.horizontal -= 2;
                break;
            case direction_e::south_east:
                offset.horizontal += 1;
                offset.vertical -= 1;
                break;
            case direction_e::north_east:
                offset.horizontal += 1;
                offset.vertical += 1;
                break;
            case direction_e::north_west:
                offset.horizontal -= 1;
                offset.vertical += 1;
                break;
            case direction_e::south_west:
                offset.horizontal -= 1;
                offset.vertical -= 1;
                break;
            }
        }

        if (auto it = tiles.find(offset);
            it != tiles.end()) {
            it->second = !it->second;
            offset_t offsets[] = { {2, 0}, {-2, 0}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
            for (const auto& neighbor_offset : offsets) {
                auto next_offset = offset + neighbor_offset;
                if (auto next_it = tiles.find(next_offset);
                    next_it == tiles.end()) {
                    tiles.insert({next_offset, true});
                }
            }
        } else {
            tiles.insert({offset, false});
            offset_t offsets[] = { {2, 0}, {-2, 0}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
            for (const auto& neighbor_offset : offsets) {
                auto next_offset = offset + neighbor_offset;
                if (auto next_it = tiles.find(next_offset);
                    next_it == tiles.end()) {
                    tiles.insert({next_offset, true});
                }
            }
        }
    }

    int64_t count = 0;
    for (const auto& tile : tiles) {
        if (!tile.second) {
            count++;
        }
    }

    std::cout << "part 1: " << count << '\n';

    auto neighbor_fn = [&](const offset_t& offset) {
        int64_t count = 0;
        offset_t offsets[] = { {2, 0}, {-2, 0}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
        for (const auto& neighbor_offset : offsets) {
            auto next_offset = offset + neighbor_offset;
            if (auto it = tiles.find(next_offset);
                it != tiles.end()) {
                if (!it->second) {
                    count++;
                }
            }
        }
        return count;
    };

    // all start as white (true)
    // flipped to black (false)
    for (int day = 0; day < 100; day++) {
        std::vector<offset_t> offsets;
        for (auto& tile : tiles) {
            if (!tile.second) {
                auto black_neighbors = neighbor_fn(tile.first);
                if (black_neighbors == 0 || black_neighbors > 2) {
                    offsets.push_back(tile.first);
                }
            } else {
                auto black_neighbors = neighbor_fn(tile.first);
                if (black_neighbors == 2) {
                    offsets.push_back(tile.first);
                }
            }
        }

        for (const auto& offset : offsets) {
            tiles[offset] = !tiles[offset];
        }

        int64_t black_tiles = 0;
        for (auto& tile : tiles) {
            if (!tile.second) {
                black_tiles++;
            }
        }

        offsets.clear();
        for (const auto& tile : tiles) {
            if (tile.second) { continue; }
            offset_t offs[] = { {2, 0}, {-2, 0}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
            for (const auto& neighbor_offset : offs) {
                auto next_offset = tile.first + neighbor_offset;
                if (auto next_it = tiles.find(next_offset);
                    next_it == tiles.end()) {
                    offsets.push_back(next_offset);
                }
            }
        }

        for (const auto& off : offsets) {
            tiles.insert({off, true});
        }

        if (day == 99) {
            std::cout << "part 2: " << black_tiles << '\n';
        }

    }

    return 0;
}
