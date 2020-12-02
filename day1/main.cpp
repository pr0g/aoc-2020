#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");
    
    std::vector<int> numbers;
    for (std::string line; std::getline(reader, line);) {
        std::cout << line << "\n";
        int number = std::stoi(line);
        numbers.push_back(number);
    }

    for (const auto number : numbers) {
        std::cout << number << "\n";
    }

    for (int i = 0; i < numbers.size(); ++i) {
        for (int j = i + 1; j < numbers.size(); ++j) {
            int result = numbers[i] + numbers[j];
            if (result == 2020) {
                std::cout << numbers[i] << " and " << numbers[j] << "\n";
                std::cout << numbers[i] * numbers[j] << "\n";
                goto end;
            }
        }
    }
end:

    return 0;
}
