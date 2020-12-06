#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>
#include <chrono>

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

    auto intersect = [](auto&& lhs, const auto& rhs) {
        auto intersection = std::string{};
        std::set_intersection(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
            std::back_inserter(intersection));
        return intersection;
    };

    auto start_algo = std::chrono::steady_clock::now();
    auto part2_algo = std::transform_reduce(
        adv_forms.begin(), adv_forms.end(), 0,
        [](int acc, const std::string& intersection_form) {
            return acc + intersection_form.size();
        },
        [&intersect](adv_form_t& adv_form) {
            std::for_each(
                adv_form.begin(), adv_form.end(),
                [](auto& line) {
                    std::sort(line.begin(), line.end());
                });
            return adjacent_transform_reduce(
                adv_form.begin(), adv_form.end(),
                adv_form.front(),
                [&intersect](auto acc, const auto& str) {
                    return intersect(std::move(acc), str);
                },
                [&intersect](const auto& lhs, const auto& rhs) {
                    return intersect(lhs, rhs);
                });
        });
    auto stop_algo = std::chrono::steady_clock::now();
    std::chrono::duration<double> algo_diff = stop_algo - start_algo;
    std::cout << "algo duration " << std::setw(9) << algo_diff.count() << "s\n";

    std::cout << "part2 algo: " << part2_algo << "\n";

    // loop version
    auto start_loop = std::chrono::steady_clock::now();
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
    auto stop_loop = std::chrono::steady_clock::now();
    std::chrono::duration<double> loop_diff = stop_loop - start_loop;
    std::cout << "loop duration " << std::setw(9) << loop_diff.count() << "s\n";

    // 3550
    std::cout << "part2: " << part2 << "\n";

    return 0;
}
