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
#include <functional>
#include <queue>
#include <regex>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using keys_t = std::vector<int64_t>;
using key_groups_t = std::vector<keys_t>;
using value_t = std::variant<char, key_groups_t>;

struct rule_t
{
    int64_t key{};
    value_t value;
};

using rules_t = std::vector<rule_t>;

void recurse_me(
    const rules_t& rules, const key_groups_t& key_groups, std::ostream& os) {
    for (int key_group_index = 0; key_group_index < key_groups.size(); ++key_group_index) {
        const auto& key_group = key_groups[key_group_index];
        for (const auto& key : key_group) {
            auto rule_it = std::find_if(
                  rules.begin(), rules.end(), [key](const rule_t& rule) {
                return rule.key == key;
            });
            if (std::holds_alternative<key_groups_t>(rule_it->value)) {
                os << "(?:";
                key_groups_t inner_key_group = std::get<key_groups_t>(rule_it->value);
                recurse_me(rules, inner_key_group, os);
                os << ")";
            } else {
                char character = std::get<char>(rule_it->value);
                os << character;
            }
        }
        if (key_groups.size() > 1 && key_group_index == 0) {
            os << "|";
        }
    }
}

int main(int argc, char **argv)
{
    std::ifstream reader("input.txt");

    enum class input_mode_e
    {
        rules,
        messages
    };

    input_mode_e input_mode = input_mode_e::rules;

    std::vector<std::string> rules_str;
    std::vector<std::string> messages;
    for (std::string line; std::getline(reader, line);)
    {
        if (line.empty())
        {
            input_mode = input_mode_e::messages;
            continue;
        }
        switch (input_mode)
        {
        case input_mode_e::rules:
            rules_str.push_back(line);
            break;
        case input_mode_e::messages:
            messages.push_back(line);
            break;
        }
    }

//    for (const auto &rule : rules_str) {
//        std::cout << rule << "\n";
//    }
//
//    for (const auto &message : messages) {
//        std::cout << message << "\n";
//    }

    rules_t rules;
    for (const auto &rule_str : rules_str)
    {
        auto sep = rule_str.find(':');
        auto key_str = rule_str.substr(0, sep);
        auto key = stoi(key_str);
        std::cout << key << '\n';

        auto parse_rules = [](const std::string& rule){
            std::stringstream ss(rule);
            int number;
            keys_t keys;
            while (ss >> number) {
                keys.push_back(number);
            }
            return keys;
        };

        if (auto quote = rule_str.find('\"');
            quote != std::string::npos) {
            auto sub_str = rule_str.substr(quote + 1, 1);
            rule_t rule;
            rule.key = key;
            rule.value = sub_str[0];
            rules.push_back(rule);
        } else if (auto orr = rule_str.find('|');
            orr == std::string::npos) {
            auto sub_str = rule_str.substr(sep + 1, rule_str.size());
            key_groups_t key_options;
            key_options.push_back(parse_rules(sub_str));
            rule_t rule;
            rule.key = key;
            rule.value = key_options;
            rules.push_back(rule);
        } else {
            auto sub_str_lhs = rule_str.substr(sep + 1, orr - (sep + 1));
            auto sub_str_rhs = rule_str.substr(orr + 1, rule_str.size());
            key_groups_t key_options;
            key_options.push_back(parse_rules(sub_str_lhs));
            key_options.push_back(parse_rules(sub_str_rhs));
            rule_t rule;
            rule.key = key;
            rule.value = key_options;
            rules.push_back(rule);
        }
    }

    std::cout << "---\n";

    std::unordered_map<int64_t, std::vector<std::string>> valid_messages;

    for (const auto& rule : rules) {
        std::cout << "key: " << rule.key << "\n";
        std::visit(overloaded {
            [](const key_groups_t& options) {
                for (const auto& keys : options) {
                    for (const auto v : keys) {
                        std::cout << v << " ";
                    }
                    std::cout << '\n';
                }
            },
            [&valid_messages, &rule](char arg) {
                valid_messages[rule.key] = std::vector{std::string(1, arg)};
                std::cout << arg << "\n";
            },
        }, rule.value);
    }

    std::cout << "---\n";

    auto rule_zero = std::find_if(rules.begin(), rules.end(), [](const rule_t& rule) {
        return rule.key == 0;
    });

    key_groups_t key_group = std::get<key_groups_t>(rule_zero->value);
    std::stringstream ss;
    recurse_me(rules, key_group, ss);
    std::cout << ss.str() << '\n';

    const std::regex pattern(ss.str());
    std::cout << "part1: " <<
        std::count_if(messages.begin(), messages.end(), [&pattern](const auto& message) {
        return std::regex_match(message, pattern);
    });

    std::cout << "\n---\n";

    return 0;
}
