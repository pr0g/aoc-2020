#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");
    
    std::vector<int> numbers;
    for (std::string line; std::getline(reader, line);) {
        numbers.push_back(std::stoi(line));
    }

    for (int i = 0; i < numbers.size(); ++i) {
        for (int j = i + 1; j < numbers.size(); ++j) {
            int result = numbers[i] + numbers[j];
            if (result == 2020) {
                std::cout << numbers[i] << " and " << numbers[j] << "\n";
                std::cout << numbers[i] * numbers[j] << "\n";
                goto end_part1;
            }
        }
    }
end_part1:

    for (int i = 0; i < numbers.size(); ++i) {
        for (int j = i + 1; j < numbers.size(); ++j) {
            for (int k = j + 1; k < numbers.size(); ++k) {
                int result = numbers[i] + numbers[j] + numbers[k];
                if (result == 2020) {
                    std::cout << numbers[i] << ", " << numbers[j] << " and " << numbers[k] << "\n";
                    std::cout << numbers[i] * numbers[j] * numbers[k] << "\n";
                    goto end_part2;
                }
            }
        }
    }
end_part2:

    return 0;
}
