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

    std::vector<int64_t> xmas;
    for(std::string line; std::getline(reader, line);) {
        xmas.push_back(stol(line));
    }

    const int64_t preamble = 25;

    int64_t offset = 0;
    for (int64_t slide = preamble; slide < xmas.size() - preamble; ++slide) {
        bool found_sum = false;
        for (int64_t i = slide - preamble; i < slide + preamble; ++i) {
            for (int64_t j = i + 1; j < slide + preamble; ++j) {
                auto total = xmas[i] + xmas[j];
                if (total == xmas[slide]) {
                    found_sum = true;
                    goto end;
                }
            }
        }
end:
        if (!found_sum) {
            offset = slide;
        }
    }

    std::cout << "part1: " << xmas[offset] << "\n";

    int64_t key = 258585477;
    int64_t result;

    for (bool range_found = false; !range_found;) {
        for (int window = 4;!range_found;window++) {
            for (int64_t start = 0; start < xmas.size() - window; ++start) {
                int64_t total {0};
                for (int64_t i = start; i < start + window; ++i) {
                    total = total + xmas[i];
                }
                if (total == key) {
                    std::vector<int64_t> range;
                    for (int i = start; i < start + window; ++i) {
                        range.push_back(xmas[i]);
                    }
                    auto minmax = std::minmax_element(std::begin(range), std::end(range));
                    result = *minmax.first + *minmax.second;
                    goto end2;
                }
            }
        }
    }

end2:

    std::cout << "part2: " << result << "\n";

    return 0;
}
