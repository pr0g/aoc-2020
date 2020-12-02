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

    int valid_count = 0;
    for (;reader >> begin >> blank >> end >> character >> blank >> password;) {
        std::cout << begin << " " << end << " " << character << " " << password << "\n";

        const auto count = std::count_if(
            password.cbegin(), password.cend(), [character](const char c) {
            return c == character;
        });

        if (count >= begin && count <= end) {
            valid_count++;
        }
    }

    std::cout << valid_count << "\n";

    return 0;
}
