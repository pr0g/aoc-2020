#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>

using forest_t = std::vector<std::vector<char>>;

int treeCount(const forest_t& forest, int across, int down) {
    int x = across;
    int y = down;
    int tree_count = 0;
    while (y < forest.size()) {
        const auto c = forest[y][x];
        if (c == '#') {
            tree_count++;
        }

        x = (x + across) % forest[y].size();
        y += down;
    }
    return tree_count;
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    forest_t forest;
    for (std::string line; std::getline(reader, line);) {
        std::vector<char> row;
        for (const auto& c : line) {
            row.push_back(c);
        }
        forest.push_back(row);
    }

    // input check
    for (int i = 0; i < forest.size(); ++i) {
        for (int j = 0; j < forest[i].size(); ++j) {
            std::cout << forest[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::vector<std::pair<int, int>> slopes = {
        {3, 1}, {1, 1,}, {5, 1}, {7, 1}, {1, 2}
    };
    
    int tree_product = 1;
    for (const auto& slope : slopes) {
        tree_product *= treeCount(forest, slope.first, slope.second);
    }

    std::cout << "tree_count: " << treeCount(forest, 3, 1) << "\n";
    std::cout << "tree_product: " << tree_product << "\n";

    return 0;
}
