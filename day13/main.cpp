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

    const int64_t depart_time = stol(lines[0]);
    
    std::unique_ptr<char[]> bus_times = std::make_unique<char[]>(lines[1].size() + 1);
    std::strncpy(bus_times.get(), lines[1].c_str(), lines[1].size());

    std::vector<std::string> input_times;
    char* token = std::strtok(bus_times.get(), ",");
    while (token != nullptr) {
        input_times.push_back(token);
        token = std::strtok(nullptr, ",");
    }

    std::cout << "depart time: " << depart_time << "\n";

    auto times = std::vector<std::pair<int64_t, int64_t>>{};
    for (int i = 0; i < input_times.size(); ++i) {
        try {
            int64_t time = stol(input_times[i]);
            times.push_back(std::pair{time, i});
        } catch (std::invalid_argument) {}
    }

    std::vector<int64_t> closest_times;
    for (const auto time : times) {
        int64_t closest_time = (depart_time / time.first) * time.first;
        if (closest_time < depart_time) {
            closest_time += time.first;
        }
        closest_times.push_back(closest_time);
        std::cout << closest_time << "\n";
    }

    for (auto time : times) {
        std::cout << "id: " << time.first
                  << " offset: " << time.second << "\n";
    }

    auto min_it = std::min_element(closest_times.begin(), closest_times.end());
    auto diff = (*min_it - depart_time) * times[std::distance(closest_times.begin(), min_it)].first;

    std::cout << "part1: " << diff << "\n";

//    {
//        // iterative approach (sloooooowww.....)
//        bool found = false;
//        int counter = 0;
//        while (!found) {
//            bool over = false;
//            for (int i = 1; i < times.size(); ++i) {
//                int64_t t = times.front().first * counter + times[i].second;
//                if (t % times[i].first != 0) {
//                    over = true;
//                    break;
//                }
//            }
//            if (!over) {
//                found = true;
//            } else {
//                counter++;
//            }
//        }
//    }

    int64_t lcm = times.front().first;
    int64_t searcher_index = 1;
    auto searcher = times[searcher_index];
    int64_t current = 0;
    int64_t next = 0;

    while (true) {
        if (((current + searcher.second) % searcher.first) == 0) {
            lcm *= searcher.first;
            searcher_index++;
            if (searcher_index >= times.size()) {
                break;
            }
            current += lcm;
            next = current;
            searcher = times[searcher_index];
            continue;
        }
        if (current < next) {
            current += lcm;
        } else {
            next += std::max(lcm, searcher.first);
        }
    }

    std::cout << "part2: " << current << "\n";

    return 0;
}
