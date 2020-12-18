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

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    struct range_t
    {
        int64_t begin;
        int64_t end;
    };

    enum class mode_e
    {
        ranges,
        your_ticket,
        nearby_tickets,
    };

    struct range_pair_t
    {
        std::string name;
        std::pair<range_t, range_t> ranges;

        bool operator==(const range_pair_t& rp) const {
            return name == rp.name;
        }
    };

    using ticket_t = std::vector<int64_t>;
    using tickets_t = std::vector<ticket_t>;

    ticket_t your_ticket;
    tickets_t nearby_tickets;

    mode_e current_mode = mode_e::ranges;
    std::vector<range_pair_t> range_pairs;
    for (const auto& line : lines) {
        if (line.empty()) {
            continue;
        }

        if (auto identifier = line.find("your ticket");
            identifier != std::string::npos)
        {
            current_mode = mode_e::your_ticket;
            continue;
        }

        if (auto identifier = line.find("nearby tickets");
            identifier != std::string::npos)
        {
            current_mode = mode_e::nearby_tickets;
            continue;
        }

        const auto range_fn =
            [](int identifier_offset, const std::string& line) {
            auto ranges = line.substr(identifier_offset + 1, line.size());

            auto start = ranges.find(' ');
            auto mid = ranges.find('-');
            auto end = ranges.find(' ', start + 1);

            auto r_begin = ranges.substr(start, mid - start);
            auto r_end = ranges.substr(mid + 1, end - mid);

            return range_t{stol(r_begin), stol(r_end)};
        };

        auto ticket_fn = [](const std::string& line){
                ticket_t ticket;
                auto start = 0;
                for (auto end = line.find(',', start); end != std::string::npos;) {
                    auto str = line.substr(start, end - start);
                    ticket.push_back(stol(str));
                    start += (end - start) + 1;
                    end = line.find(',', start );
                }
                auto str = line.substr(start, line.size());
                ticket.push_back(stol(str));
                return ticket;
        };

        switch (current_mode)
        {
        case mode_e::ranges: {
            range_pair_t range_pair;
            if (auto identifier = line.find(':');
                identifier != std::string::npos) {
                range_pair.name = line.substr(0, identifier);
                range_pair.ranges.first = range_fn(identifier, line);
            }
            if (auto identifier = line.find(" or ");
                identifier != std::string::npos) {
                range_pair.ranges.second = range_fn(identifier, line);
            }
            range_pairs.push_back(range_pair);
        }
        break;
        case mode_e::your_ticket:
            your_ticket = ticket_fn(line);
            break;
        case mode_e::nearby_tickets:
            nearby_tickets.push_back(ticket_fn(line));
            break;
        }
    }

    for (const auto& range_pair : range_pairs) {
        std::cout << range_pair.name << ": "
                  << range_pair.ranges.first.begin << " - " << range_pair.ranges.first.end << ", "
                  << range_pair.ranges.second.begin << " - " << range_pair.ranges.second.end << "\n";
    }

    for (const auto& number : your_ticket) {
        std::cout << number << "\n";
    }

    std::cout << "before filter: " << nearby_tickets.size() << "\n";

    std::vector<int64_t> invalid_numbers;
     for (auto it = nearby_tickets.begin(); it != nearby_tickets.end();) {
//    for (int i = 0; i < nearby_tickets.size();) {
        const auto ticket = *it;//nearby_tickets[i];
        bool remove = false;
        for (const auto& number : ticket) {
            bool valid = false;
            for (const auto& range_pair : range_pairs) {
                if ((number >= range_pair.ranges.first.begin && number <= range_pair.ranges.first.end)
                || (number >= range_pair.ranges.second.begin && number <= range_pair.ranges.second.end)) {
                    valid = true;
                    break;
                }
            }
            if (!valid) {
                invalid_numbers.push_back(number);
                remove = true;
                break;
            }
        }

        if (remove) {
            it = nearby_tickets.erase(it);
//            nearby_tickets[i] = nearby_tickets.back();
//            nearby_tickets.pop_back();
        } else {
//            ++i;
            ++it;
        }

    }

    std::cout << "after filter: " << nearby_tickets.size() << "\n";

    int64_t invalid_total = 0;
    for (const auto number : invalid_numbers) {
        invalid_total += number;
    }

    std::cout << "part1: " << invalid_total << "\n";

    struct Hash {
       size_t operator() (const range_pair_t &rp) const {
         return std::hash<std::string>{}(rp.name);
       }
    };

    struct groups_t
    {
        range_pair_t range_pair;
        std::vector<int64_t> valid_col;
    };

    std::vector<groups_t> groups;
    for (const auto& range_pair : range_pairs) {
        groups_t group;
        for (int column = 0; column < nearby_tickets.front().size(); ++column) {
            group.range_pair = range_pair;
            bool possible = true;
            for (const auto& nearby_ticket : nearby_tickets) {
                const int number = nearby_ticket[column];
                const bool valid =
                    (number >= range_pair.ranges.first.begin && number <= range_pair.ranges.first.end)
                || (number >= range_pair.ranges.second.begin && number <= range_pair.ranges.second.end);

                if (!valid) {
                    possible = false;
                    break;
                }
            }
            if (possible) {
                group.valid_col.push_back(column);
            }
        }
        groups.push_back(group);
    }

    struct order_t
    {
        std::string name;
        int offset;
    };

    std::vector<order_t> ordering;

    std::vector<int> counts;
    counts.resize(groups.size());

    for (int i = 0; i < groups.size();) {
        for (auto& inner_group : groups) {
            for (int col : inner_group.valid_col) {
                counts[col]++;
            }
        }

        int c = -1;
        for (int j = 0; j < counts.size(); ++j) {
            if (counts[j] == 1) {
                c = j;
                break;
            }
        }
        if (c != -1) {
            int number = c;
            auto group_it = std::find_if(groups.begin(), groups.end(), [number](const groups_t& g) {
                auto found = std::find(g.valid_col.begin(), g.valid_col.end(), number);
                return found != g.valid_col.end();
            });
            auto& group = *group_it;
            for (auto& inner_group : groups) {
                inner_group.valid_col.erase(
                    std::remove(inner_group.valid_col.begin(), inner_group.valid_col.end(), number),
                    inner_group.valid_col.end());
            }
            ordering.push_back({group.range_pair.name, number});
            group.valid_col.clear();
            std::fill(counts.begin(), counts.end(), 0);
            i = 0;
        } else {
            i++;
        }
    }

    std::sort(ordering.begin(), ordering.end(),
        [](const auto& lhs, const auto& rhs) {
        return lhs.offset < rhs.offset;
    });

    std::vector<int> departure_lookups;
    for (int i = 0; i < ordering.size(); ++i) {
        const auto& ordered_range = ordering[i];
        if (auto found = ordered_range.name.find("departure");
            found != std::string::npos) {
            departure_lookups.push_back(i);
        }
        std::cout << ordered_range.name << "\n";
    }

    std::cout << "---\n";
    int64_t part2 = 1;
    for (auto lu : departure_lookups) {
        std::cout << lu << "\n";
        part2 *= your_ticket[lu];
    }

    std::cout << "part2: " << part2 << "\n";

    return 0;
}
