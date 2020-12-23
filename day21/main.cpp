#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <functional>
#include <queue>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

int main(int argc, char **argv)
{
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for (std::string line; std::getline(reader, line);)
    {
        lines.push_back(line);
    }

    struct recipe_t {
        std::vector<std::string> ingredients;
        std::vector<std::string> allergens;
    };

    std::vector<recipe_t> recipes;
    for (const auto& line : lines) {
        const auto bracket_pos = line.find('(');
        const auto ingredients_str = line.substr(0, bracket_pos - 1);

        recipe_t recipe;
        std::stringstream ss_i(ingredients_str);
        std::string ingredient;
        while (ss_i >> ingredient) {
            recipe.ingredients.push_back(ingredient);
        }

        const std::string contains_str = "contains";
        const auto end_bracket_pos = line.find(')');
        const auto contains_pos = line.find(contains_str);
        const auto allergens_pos = contains_pos + contains_str.size();
        const auto allergens_str =
            line.substr(allergens_pos, end_bracket_pos - allergens_pos);
        
        std::stringstream ss_a(allergens_str);
        std::string allergen;
        while (std::getline(ss_a, allergen, ',')) {
            trim(allergen);
            recipe.allergens.push_back(allergen);
        }

        std::sort(recipe.ingredients.begin(), recipe.ingredients.end());
        std::sort(recipe.allergens.begin(), recipe.allergens.end());

        recipes.push_back(recipe);
    }

    std::unordered_map<std::string, std::vector<std::string>> unique_ingredients;
    for (const auto& recipe : recipes) {
        for (const auto& allergen : recipe.allergens) {
            auto all_it = unique_ingredients.find(allergen);
            if (all_it == unique_ingredients.end()) {
                all_it = unique_ingredients.insert({allergen, recipe.ingredients}).first;
            }
            std::vector<std::string> intersection;
            std::set_intersection(
                recipe.ingredients.begin(), recipe.ingredients.end(),
                all_it->second.begin(), all_it->second.end(),
                std::back_inserter(intersection));
            all_it->second = intersection;
        }
    }

    for (bool unique = false; !unique;) {
        unique = true;
        for (auto& lookup : unique_ingredients) {
            if (lookup.second.size() == 1) {
                for (auto& lu : unique_ingredients) {
                    if (lu.second.size() > 1) {
                        unique = false;
                        lu.second.erase(
                            std::remove_if(
                                lu.second.begin(), lu.second.end(), [&](const auto& ing) {
                                    return ing == lookup.second.front();
                                }), lu.second.end());
                    }
                }
            }
        }
    }

    std::vector<std::string> allergen_ingredients;
    allergen_ingredients.reserve(unique_ingredients.size());
    for (const auto& unique : unique_ingredients) {
        allergen_ingredients.push_back(unique.second.front());
    }

    std::vector<std::pair<std::string, std::string>> allergen_ingredient_pairs;
    allergen_ingredient_pairs.reserve(unique_ingredients.size());
    for (const auto& unique : unique_ingredients) {
        allergen_ingredient_pairs.emplace_back(unique.first, unique.second.front());
    }

    int64_t count = 0;
    for (const auto& recipe : recipes) {
        for (const auto& ingredient : recipe.ingredients) {
            if (auto ing_it = std::find(allergen_ingredients.begin(), allergen_ingredients.end(), ingredient);
                ing_it == allergen_ingredients.end()) {
                count++;
            }
        }
    }

    std::sort(allergen_ingredient_pairs.begin(), allergen_ingredient_pairs.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.first < rhs.first;
    });

    std::vector<std::string> no_allergens;
//    for (auto& recipe : recipes) {
    for (int rec_index = 0; rec_index < recipes.size();) {
        auto& recipe = recipes[rec_index];
        int h;
        h = 0;

//        if (recipe.allergens.size() > 1 || recipe.allergens.empty()) {
//            rec_index++;
//            continue;
//        }

        bool removed = false;
        for (auto all_it = recipe.allergens.begin(); all_it != recipe.allergens.end();) {
            bool outer_match = false;
            for (auto ing_it = recipe.ingredients.begin(); ing_it != recipe.ingredients.end();) {
                bool match = true;
                std::string allergen = *all_it;
                int g;
                g = 0;
                for (const auto& r : recipes) {
                    if (auto allergen_it = std::find(r.allergens.begin(), r.allergens.end(), allergen);
                        allergen_it != r.allergens.end()) {
                        if (auto ingredient_it = std::find(r.ingredients.begin(), r.ingredients.end(), *ing_it);
                            ingredient_it == r.ingredients.end()) {
                            match = false;
                        }
                    }
                }
                if (match) {
                    outer_match = true;
                    removed = true;
                    auto ingr = *ing_it;
                    int i;
                    i = 0;
                    for (auto& r : recipes) {
                        r.ingredients.erase(
                            std::remove_if(
                                r.ingredients.begin(), r.ingredients.end(), [ingr](const auto& ing) {
                                    return ingr == ing;
                                }), r.ingredients.end());
                        r.allergens.erase(
                            std::remove_if(
                                r.allergens.begin(), r.allergens.end(), [allergen](const auto& all) {
                                    return allergen == all;
                                }), r.allergens.end());
                    }
                    ing_it = recipe.ingredients.begin();
                    if (recipe.allergens.empty()) {
                        all_it = recipe.allergens.end();
                        break;
                    } else {
                        all_it = recipe.allergens.begin();
                    }
//                    break;
                } else{
                    ing_it++;
                } /*else {
                    no_allergens.push_back(*ing_it);
                    ing_it = recipe.ingredients.erase(ing_it);
                }*/
            }
            if (!outer_match) {
                all_it++;
            }
        }

        if (removed) {
            rec_index = 0;
        } else {
            rec_index++;
        }
    }

     for (auto& recipe : recipes) {
        for (const auto& ingredient : recipe.ingredients) {
            no_allergens.push_back(ingredient);
        }
     }

     std::cout << "part1: " << count << "\n";

     std::cout << "part2: ";
     for (const auto& ingredient : allergen_ingredient_pairs) {
         std::cout << ingredient.second << ",";
     }
     std::cout << '\n';

    return 0;
}
