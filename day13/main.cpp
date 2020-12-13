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

    const int depart_time = stoi(lines[0]);
    
    std::unique_ptr<char[]> bus_times = std::make_unique<char[]>(lines[1].size() + 1);
    std::strncpy(bus_times.get(), lines[1].c_str(), lines[1].size());

    std::vector<std::string> input_times;
    char* token = std::strtok(bus_times.get(), ",");
    while (token != nullptr) {
        input_times.push_back(token);
        token = std::strtok(nullptr, ",");
    }

    std::cout << "depart time: " << depart_time << "\n";

    const auto times = std::accumulate(
        input_times.begin(), input_times.end(),
        std::vector<int>{}, [](auto acc, auto elem) {
            try {
                int time = stoi(elem);
                acc.push_back(time);
            } catch (std::invalid_argument) {}
            return acc;
        });

    std::vector<int> closest_times;
    for (const auto time : times) {
        int closest_time = (depart_time / time) * time;
        if (closest_time < depart_time) {
            closest_time += time;
        }
        closest_times.push_back(closest_time);
        std::cout << closest_time << "\n";
    }

    auto min_it = std::min_element(closest_times.begin(), closest_times.end());
    auto diff = (*min_it - depart_time) * times[std::distance(closest_times.begin(), min_it)];

    std::cout << "part1: " << diff << "\n";

    return 0;
}
