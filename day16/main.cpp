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

    using range_pair_t = std::pair<range_t, range_t>;

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

            auto start = ranges.find(" ");
            auto mid = ranges.find("-");
            auto end = ranges.find(" ", start + 1);

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
                range_pair.first = range_fn(identifier, line);
            }
            if (auto identifier = line.find("or");
                identifier != std::string::npos) {
                range_pair.second = range_fn(identifier, line);
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
        std::cout << range_pair.first.begin << " - " << range_pair.first.end << ", "
                  << range_pair.second.begin << " - " << range_pair.second.end << "\n";
    }

    for (const auto& number : your_ticket) {
        std::cout << number << "\n";
    }

    std::vector<int64_t> invalid_numbers;
    for (const auto& ticket : nearby_tickets) {
        for (const auto& number : ticket) {
            bool valid = false;
            for (const auto& range_pair : range_pairs) {
                if ((number >= range_pair.first.begin && number <= range_pair.first.end)
                || (number >= range_pair.second.begin && number <= range_pair.second.end)) {
                    valid = true;
                    break;
                }
            }
            if (!valid) {
                invalid_numbers.push_back(number);
            }
        }
    }

    int invalid_total = 0;
    for (const auto number : invalid_numbers) {
        invalid_total += number;
        std::cout << number << "\n";
    }

    std::cout << "part1: " << invalid_total << "\n";

    return 0;
}
