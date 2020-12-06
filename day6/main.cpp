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

    using adv_form_t = std::vector<std::string>;
    std::vector<adv_form_t> adv_forms;

    std::vector<std::string> forms;
    std::string form;
    adv_form_t adv_form;
    for (std::string line; std::getline(reader, line);) {
        if (line.empty()) {
            forms.push_back(form);
            adv_forms.push_back(adv_form);
            form.clear();
            adv_form.clear();
            continue;
        }
        form += line;
        adv_form.push_back(line);
    }

    forms.push_back(form);
    adv_forms.push_back(adv_form);

    // check input
    for (const auto& form : forms) {
        std::cout << form << "\n";
    }

    std::cout << "---\n";

    for (const auto& adv_form : adv_forms) {
        for (const auto& line : adv_form) {
            std::cout << line << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "---\n";

    int part1 = 0;
    for (auto& form : forms) {
        std::sort(form.begin(), form.end());
        form.erase(std::unique(form.begin(), form.end()), form.end());
        part1 += form.size();
    }

    std::cout << "part1: " << part1 << "\n";

    int part2 = 0;
    for (auto& adv_form : adv_forms) {
        // start on first letter, count it in all other groups
        for (int i = 0; i < adv_form[0].size(); ++i) {
            char c = adv_form[0][i];

            bool all = true;
            for (int j = 1; j < adv_form.size(); ++j) {
                auto it = std::find(adv_form[j].begin(), adv_form[j].end(), c);
                if (it == adv_form[j].end()) {
                    all = false;
                    break;
                }
            }
            
            if (all) {
                part2++;
            }
        }
    }

    std::cout << "part2: " << part2 << "\n";

    return 0;
}
