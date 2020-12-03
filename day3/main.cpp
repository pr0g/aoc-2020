#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::vector<char>> grid;
    for (std::string line; std::getline(reader, line);) {
        std::vector<char> row;
        for (const auto& c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    // input check
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << "\n";
    }

    int tree_count = 0;

    int x = 3;
    int y = 1;
    while (y < grid.size()) {
        const auto c = grid[y][x];
        if (c == '#') {
            tree_count++;
        }

        x = (x + 3) % grid[y].size();
        y++;
    }

    std::cout << "tree_count: " << tree_count << "\n";

    return 0;
}
