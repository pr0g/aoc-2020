#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>

template<class I, class T, class R, class M>
auto adjacent_transform_reduce(I f, I l, T init, R r, M m) {
    return std::inner_product(f, --l, ++f, init, r, m);
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    using adv_form_t = std::vector<std::string>;
    using adv_forms_t = std::vector<adv_form_t>;
    adv_forms_t adv_forms;

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
    // for (const auto& form : forms) {
    //     std::cout << form << "\n";
    // }
    // std::cout << "---\n";

    int part1 = 0;
    for (auto& form : forms) {
        std::sort(form.begin(), form.end());
        form.erase(std::unique(form.begin(), form.end()), form.end());
        part1 += form.size();
    }

    std::cout << "part1: " << part1 << "\n";

    adv_forms_t next_adv_forms;
    std::transform(
        adv_forms.begin(), adv_forms.end(),
        std::back_inserter(next_adv_forms),
        [](const adv_form_t& adv_form) {
            auto next_adv_form = adv_form;
            std::transform(
                next_adv_form.begin(), next_adv_form.end(),
                next_adv_form.begin(),
                [](const auto& line) {
                    auto l = line;
                    std::sort(l.begin(), l.end());
                    return l;
                });
            return next_adv_form;
        });
    
    std::vector<std::string> intersection_forms;
    std::transform(
        next_adv_forms.begin(), next_adv_forms.end(),
        std::back_inserter(intersection_forms),
        [](const adv_form_t& adv_form) {
            auto next_adv_form = adv_form;
            auto intersection_form = adjacent_transform_reduce(
                next_adv_form.begin(), next_adv_form.end(),
                next_adv_form.front(),
                [](auto acc, const auto& str) {
                    auto r = std::string{};
                    std::set_intersection(
                        acc.begin(), acc.end(), str.begin(), str.end(),
                        std::back_inserter(r));
                    return r;
                },
                [](auto& lhs, auto& rhs) {
                    auto r = std::string{};
                    std::set_intersection(
                        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                        std::back_inserter(r));
                    return r;
                });

            return intersection_form;
        });

    auto part2_algo =
        std::accumulate(
            intersection_forms.begin(), intersection_forms.end(), 0,
            [](int acc, const std::string& intersection_form) {
                return acc + intersection_form.size();
            });

    std::cout << "part2 algo: " << part2_algo << "\n";

    // loop version
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

    // 3550
    std::cout << "part2: " << part2 << "\n";

    return 0;
}
