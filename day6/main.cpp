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

    std::vector<std::string> forms;
    std::string form;
    for (std::string line; std::getline(reader, line);) {
        if (line.empty()) {
            forms.push_back(form);
            form.clear();
            continue;
        }
        form += line;
    }

    forms.push_back(form);

    // check input
    for (const auto& form : forms) {
        std::cout << form << "\n";
    }

    std::cout << "---\n";

    int part1 = 0;
    for (auto& form : forms) {
        std::sort(form.begin(), form.end());
        form.erase(std::unique(form.begin(), form.end()), form.end());
        part1 += form.size();
    }

    std::cout << "part1: " << part1 << "\n";

    return 0;
}
