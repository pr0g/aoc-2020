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

#include "as/as-math-ops.hpp"

int main(int, char**)
{
    std::ifstream reader("input.txt");

    using image_t = as::mat<char, 8>;
    using scan_t = as::vec<char, 8>;
    using grid_t = as::mat<char, 10>;
    using basis_t = as::vec<char, 10>;

    auto basis_equal = [](const basis_t& lhs, const basis_t& rhs) {
        for (int i = 0; i < 10; ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    };

    enum side_e {
        top,
        right,
        bottom,
        left
    };

    struct vec2_t {
        int x;
        int y;
    };

    auto print_dirs = [](const vec2_t* dirs, as::index count) {
        std::cout << "---\n";
        for (as::index i = 0; i < 4; ++i) {
            std::cout << "x: " << dirs[i].x << " y: " << dirs[i].y << '\n';
        }
        std::cout << "---" << std::endl;
    };

    struct tile_t
    {
        int id = -1;
        grid_t grid;
        image_t image;

        vec2_t dirs[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        void rotate_right() {
            std::swap(dirs[0], dirs[3]);
            std::swap(dirs[2], dirs[3]);
            std::swap(dirs[1], dirs[2]);
        }

        void flip() {
            std::swap(dirs[1], dirs[3]);
        }

        [[nodiscard]] basis_t side(side_e side) const {
            switch (side) {
            case side_e::top:
                return as::mat_row(grid, 0);
            case side_e::right:
                return as::mat_col(grid, 9);
            case side_e::bottom:
                return as::mat_row(grid, 9);
            case side_e::left:
                return as::mat_col(grid, 0);
            }
            return basis_t{};
        }

        bool operator==(const tile_t& tile) const {
            for (int i = 0; i < grid_t::size(); ++i) {
                if (tile.grid[i] != grid[i]) {
                    return false;
                }
            }
            return true;
        }
    };

    auto rotate_right = [](const tile_t& tile_in) {
        tile_t tile = tile_in;
        tile.rotate_right();
        tile.grid = as::mat_transpose(tile.grid);
        for (int r = 0; r < 10; ++r) {
            auto row = as::mat_row(tile.grid, r);
            std::reverse(&row[0], &row[0] + 10);
            as::mat_row(tile.grid, r, row);
        }
        return tile;
    };

    auto flip_horizontal = [](const tile_t& tile_in) {
        tile_t tile = tile_in;
        tile.flip();
        for (int r = 0; r < 10; ++r) {
            auto row = as::mat_row(tile.grid, r);
            std::reverse(&row[0], &row[0] + 10);
            as::mat_row(tile.grid, r, row);
        }
        return tile;
    };

    auto print_grid = [](const tile_t& tile){
        std::cout << "---\n";
        for (int r = 0; r < 10; ++r) {
            auto row = as::mat_row(tile.grid, r);
            for (int c = 0; c < 10; ++c) {
                std::cout << row[c];
            }
            std::cout << '\n';
        }
        std::cout << "---\n";
    };

    auto print_image = [](const tile_t& tile){
        std::cout << "---\n";
        for (int r = 0; r < 8; ++r) {
            auto row = as::mat_row(tile.image, r);
            for (int c = 0; c < 8; ++c) {
                std::cout << row[c];
            }
            std::cout << '\n';
        }
        std::cout << "---\n";
    };

    enum class mode_e
    {
        title,
        grid
    };

    tile_t active_tile;
    int grid_row = 0;
    mode_e mode = mode_e::title;
    std::vector<tile_t> tiles;
    for (std::string line; std::getline(reader, line);) {
        if (line.empty()) {
            tiles.push_back(active_tile);
            grid_row = 0;
            active_tile.id = -1;
            mode = mode_e::title;
            continue;
        }

        switch (mode) {
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
            as::vec<char, 10> row;
            for (int i = 0; i < as::vec<char, 10>::size(); ++i) {
                row[i] = line[i];
            }
            as::mat_row(active_tile.grid, grid_row++, row);
        }
        break;
        }
    }

    tiles.push_back(active_tile);

    // check input
    for (const auto& tile : tiles) {
        std::cout << "tile: " << tile.id << '\n';
        for (int r = 0; r < 10; ++r) {
            auto row = as::mat_row(tile.grid, r);
            for (int c = 0; c < 10; ++c) {
                std::cout << row[c];
            }
            std::cout << '\n';
        }
    }

    std::deque<tile_t> available_tiles(tiles.begin(), tiles.end());
    std::vector<tile_t> pinned_tiles;

    struct position_t {
        int x{};
        int y{};
    };

    struct tile_position_t {
        int64_t id{};
        position_t pos;
    };

    // flip starting tile to be same as example
    available_tiles.front() = rotate_right(available_tiles.front());
    available_tiles.front() = rotate_right(available_tiles.front());
    available_tiles.front() = flip_horizontal(available_tiles.front());

     pinned_tiles.push_back(available_tiles.front());
     available_tiles.pop_front();

     std::vector<tile_position_t> tile_positions;
     tile_positions.push_back({pinned_tiles.front().id, position_t{}});

    std::vector<int64_t> corner_ids;
    while (!available_tiles.empty()) {
        for (int t = 0; t < available_tiles.size(); ++t) {
            for (int64_t pinned_tile_index = 0; pinned_tile_index < pinned_tiles.size(); ++pinned_tile_index) {
                for (int64_t tile_index = 0; tile_index < available_tiles.size(); ++tile_index) {
                    for (int s = 0; s < 4; ++s) {
                        for (int f = 0; f < 2; ++f) {
                            for (int r = 0; r < 4; ++r) {
                                auto opposite_side = (s + 2) % 4;
                                if (basis_equal(
                                    pinned_tiles[pinned_tile_index].side((side_e)s),
                                    available_tiles[tile_index].side((side_e)opposite_side))) {

                                    pinned_tiles.push_back(available_tiles[tile_index]);

                                    available_tiles[tile_index] = available_tiles.back();
                                    available_tiles.pop_back();

                                    auto found = std::find_if(tile_positions.begin(), tile_positions.end(),
                                        [pinned_tiles, pinned_tile_index](const tile_position_t& tile_pos) {
                                        return pinned_tiles[pinned_tile_index].id == tile_pos.id;
                                    });

                                    position_t next_pos = found->pos;
                                    next_pos.x += pinned_tiles[0].dirs[s].x;
                                    next_pos.y += pinned_tiles[0].dirs[s].y;

                                    tile_positions.push_back({pinned_tiles.back().id, next_pos});
                                    goto loop;
                                }
                                available_tiles[tile_index] = rotate_right(available_tiles[tile_index]);
                            }
                            available_tiles[tile_index] = flip_horizontal(available_tiles[tile_index]);
                        }
                    }
                }
            }
        }
loop:
        (void)sizeof(int);
    }

    auto min_x = std::min_element(
        tile_positions.begin(), tile_positions.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.pos.x < rhs.pos.x;
        });

    auto max_x = std::max_element(
        tile_positions.begin(), tile_positions.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.pos.x < rhs.pos.x;
        });

    auto min_y = std::min_element(
        tile_positions.begin(), tile_positions.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.pos.y < rhs.pos.y;
        });

    auto max_y = std::max_element(
        tile_positions.begin(), tile_positions.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.pos.y < rhs.pos.y;
        });

    for (int y = min_y->pos.y; y <= max_y->pos.y; ++y) {
        for (int x = min_x->pos.x; x <= max_x->pos.x; ++x) {
            if (auto tile_pos = std::find_if(
                tile_positions.begin(), tile_positions.end(), [x, y](const tile_position_t& tile_p) {
                    return tile_p.pos.x == x && tile_p.pos.y == y;
            }); tile_pos != tile_positions.end()) {
                std::cout << tile_pos->id << " ";
            }
        }
        std::cout << '\n';
    }
    std::cout << std::endl;

    for (tile_t& tile : pinned_tiles) {
        for (int r = 1, m = 0; r < 9; ++r, ++m) {
            scan_t scan;
            basis_t basis = as::mat_row(tile.grid, r);
            for (int c = 1, i = 0; c < 9; c++, i++) {
                scan[i] = basis[c];
            }
            as::mat_row(tile.image, m, scan);
        }
    }

    std::cout << "--------\n";

    for (int y = min_y->pos.y; y <= max_y->pos.y; ++y) {
        for (int x = min_x->pos.x; x <= max_x->pos.x; ++x) {
            if (auto tile_pos = std::find_if(
                tile_positions.begin(), tile_positions.end(),
                [x, y](const tile_position_t& tile_p) {
                    return tile_p.pos.x == x && tile_p.pos.y == y;
            }); tile_pos != tile_positions.end()) {
                if (auto t = std::find_if(pinned_tiles.begin(), pinned_tiles.end(),
                    [tile_pos](const tile_t& t) {
                    return t.id == tile_pos->id;
                }); t != pinned_tiles.end()) {
                     print_image(*t);
                }
            }
        }
        std::cout << '\n';
    }

    std::cout << std::endl;

    constexpr as::index full_image_size = 96;
    using full_image_t = as::mat<char, full_image_size>;

    full_image_t image;

    int row = 0;
    for (int y = min_y->pos.y; y <= max_y->pos.y; ++y) {
        for (int r = 0; r < scan_t::size(); ++r) {
            int col = 0;
            for (int x = min_x->pos.x; x <= max_x->pos.x; ++x) {
                if (auto tile_pos = std::find_if(
                    tile_positions.begin(), tile_positions.end(),
                    [x, y](const tile_position_t& tile_p) {
                        return tile_p.pos.x == x && tile_p.pos.y == y;
                }); tile_pos != tile_positions.end()) {
                    if (auto t = std::find_if(pinned_tiles.begin(), pinned_tiles.end(),
                        [tile_pos](const tile_t& t) {
                        return t.id == tile_pos->id;
                    }); t != pinned_tiles.end()) {
                        auto mat_row = as::mat_row(t->image, r);
                        for (int c = 0; c < scan_t::size(); ++c) {
                            std::cout << mat_row[c];
                            image[as::mat_rc(row, col++, full_image_size)] = mat_row[c];
                        }
                    }
                }
            }
            std::cout << '\n';
            row++;
        }
    }

    std::cout << std::endl;

    auto print_img = [](const full_image_t& image) {
        for (int r = 0; r < full_image_size; r++) {
            for (int c = 0; c < full_image_size; c++) {
                std::cout << image[as::mat_rc(r, c, full_image_size)];
            }
            std::cout << '\n';
        }
    };

    auto rotate_image = [](const full_image_t& image) {
        auto rotated_image = as::mat_transpose(image);
        for (int r = 0; r < full_image_size; ++r) {
            auto row = as::mat_row(rotated_image, r);
            std::reverse(&row[0], &row[0] + full_image_size);
            as::mat_row(rotated_image, r, row);
        }
        return rotated_image;
    };

    auto flip_image = [](const full_image_t& image) {
        auto flipped_image = image;
        for (int r = 0; r < full_image_size; ++r) {
            auto row = as::mat_row(flipped_image, r);
            std::reverse(&row[0], &row[0] + full_image_size);
            as::mat_row(flipped_image, r, row);
        }
        return flipped_image;
    };

    auto find_seamonsters = [](const full_image_t& image) {
        int monsters = 0;
        for (int r = 1; r < full_image_t::rows(); ++r) {
            for (int c = 0; c < full_image_t::cols() - 20; ++c) {
                if (    image[as::mat_rc(r, c, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 5, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 6, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 11, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 12, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 17, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 18, full_image_size)] == '#'
                    &&  image[as::mat_rc(r, c + 19, full_image_size)] == '#'
                    &&  image[as::mat_rc(r + 1, c + 1, full_image_size)] == '#'
                    &&  image[as::mat_rc(r + 1, c + 4, full_image_size)] == '#'
                    &&  image[as::mat_rc(r + 1, c + 7, full_image_size)] == '#'
                    &&  image[as::mat_rc(r + 1, c + 10, full_image_size)] == '#'
                    &&  image[as::mat_rc(r + 1, c + 13, full_image_size)] == '#'
                    &&  image[as::mat_rc(r + 1, c + 16, full_image_size)] == '#'
                    &&  image[as::mat_rc(r - 1, c + 18, full_image_size)] == '#') {
                    monsters++;
                }
            }
        }
        return monsters;
    };

    auto total_seamonsters = 0;
    auto next_image = image;
    for (int f = 0; f < 2; ++f) {
        for (int r = 0; r < 5; ++r) {
            std::cout << "r: " << r << " f: " << f << '\n';
            print_img(next_image);
            auto monsters = find_seamonsters(next_image);
            if (monsters > 0) {
                std::cout << "monsters: " << monsters << "\n";
                total_seamonsters = monsters;
                goto end;
            }
            next_image = rotate_image(next_image);
            std::cout << "---\n";
        }
        next_image = flip_image(next_image);
        std::cout << "---\n";
    }

end:

    auto total_octothorpe = 0;
    for (int r = 0; r < full_image_t::rows(); ++r) {
        for (int c = 0; c < full_image_t::cols(); ++c) {
            if (next_image[as::mat_rc(r, c, full_image_size)] == '#') {
                total_octothorpe++;
            }
        }
    }

    std::cout << "part 2: " << total_octothorpe - (total_seamonsters * 15) << '\n';

    return 0;
}
