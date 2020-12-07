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

    std::vector<std::string> lines;
    for (std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    auto bag_finder =
        [](const std::string::size_type start, const std::string& line) {
            if (start >= line.size()) {
                return std::string::npos;
            }
            
            if (auto comma_offset = line.find(",", start);
                comma_offset != std::string::npos) {
                return comma_offset;
            } else if (auto period_offset = line.find(".", start);
                      period_offset != std::string::npos) {
                return period_offset;
            }

            return std::string::npos;
        };

    using namespace std::string_literals;
    for (const auto& line : lines) {
        if (const auto contains_found = line.find("contain");
            contains_found != std::string::npos) {
            const std::string bag = line.substr(0, contains_found);
            const std::string color = bag.substr(0, bag.find("bag"));
            std::cout << color << "contains:\n";

            if (const auto no_found = line.find("no", contains_found);
                no_found != std::string::npos) {
                std::cout << "none\n";
                continue;
            } else {
                std::string::size_type count_offset =
                    contains_found + "contain"s.size() + 1;
                
                while (true) {
                    auto next_bag_begin_offset = count_offset + 2;
                    auto next_bag_end_offset = bag_finder(next_bag_begin_offset, line);
                    if (next_bag_end_offset == std::string::npos) {
                        break;
                    }

                    std::string count = line.substr(count_offset, 1);
                    std::cout << count << " ";

                    auto next_bag = line.substr(next_bag_begin_offset, next_bag_end_offset - next_bag_begin_offset);
                    auto next_color = next_bag.substr(0, next_bag.find("bag"));
                    std::cout << next_color << "\n";

                    count_offset = next_bag_end_offset + 2;
                }
            }
        }
    }

    return 0;
}
