#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");
    
    std::vector<int> numbers;
    
    int begin, end;
    char blank;
    char character;
    std::string password;

    int valid_count_part1 = 0;
    int valid_count_part2 = 0;
    for (;reader >> begin >> blank >> end >> character >> blank >> password;) {
        const auto count = std::count_if(
            password.cbegin(), password.cend(), [character](const char c) {
            return c == character;
        });

        if (count >= begin && count <= end) {
            valid_count_part1++;
        }

        auto begin_lookup = begin - 1;
        auto end_lookup = end - 1;

        if ((password[begin_lookup] == character
            || password[end_lookup] == character)
            && password[begin_lookup] != password[end_lookup]) {
            valid_count_part2++;
        }
    }

    std::cout << valid_count_part1 << "\n";
    std::cout << valid_count_part2 << "\n";

    return 0;
}
