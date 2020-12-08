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

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for (std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    auto bag_finder =
        [](const std::string::size_type start, const std::string& line) {
            if (auto comma_offset = line.find(",", start);
                comma_offset != std::string::npos) {
                return comma_offset;
            } else if (auto period_offset = line.find(".", start);
                      period_offset != std::string::npos) {
                return period_offset;
            }
            return std::string::npos;
        };

    using bags = std::pair<std::string, int>;
    std::unordered_map<std::string, std::vector<bags>> all_bags;

    using std::string_literals::operator""s;
    for (const auto& line : lines) {
        if (const auto contains_found = line.find("contain");
            contains_found != std::string::npos) {
            const std::string bag = line.substr(0, contains_found);
            std::string color = bag.substr(0, bag.find("bag"));
            trim(color);
            std::cout << color << "contains:\n";
            
            if (auto it = all_bags.find(color);
                it == all_bags.end()) {
                all_bags.insert({color, std::vector<bags>{}});
            }

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
                    trim(next_color);
                    std::cout << next_color << "\n";

                    if (auto it = all_bags.find(color);
                        it != all_bags.end()) {
                        it->second.push_back(std::make_pair(next_color, stoi(count)));
                    }

                    count_offset = next_bag_end_offset + 2;
                }
            }
        }
    }

    std::cout << all_bags.size() << "\n";

    for (const auto& bags : all_bags["light red"]) {
        std::cout << bags.first << ":" << bags.second << "\n";
    }

    for (auto it : all_bags) {
        std::cout << it.first << "\n";
        for (auto in : it.second) {
            std::cout << "  " << in.first << " -- " << in.second << "\n";
        }
    }

    std::deque<std::string> search_names;
    search_names.push_back("shiny gold"s);
    std::unordered_set<std::string> unique_bags;

    while(!search_names.empty()) {
        for (auto it : all_bags) {
            for (auto in : it.second) {
                if (in.first == search_names.front()) {
                    std::cout << it.first << "\n";
                    search_names.push_back(it.first);
                    unique_bags.insert(it.first);
                }
            }
        }
        search_names.pop_front();
    }

    std::cout << "part1: " << unique_bags.size() << "\n";

    return 0;
}
