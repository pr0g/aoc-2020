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

enum class heading_e
{
    north,
    east,
    south,
    west,
    all_end
};

enum class rotation_e
{
    ninety = 1,
    one_eighty = 2,
    two_seventy = 3
};

struct ship_t
{
    heading_e heading;
    int x;
    int y;
};

struct control_t
{
    char action;
    int value;
};

rotation_e rotation(int value) {
    switch (value)
    {
    case 90:
        return rotation_e::ninety;
    case 180:
        return rotation_e::one_eighty;
    case 270:
        return rotation_e::two_seventy;
    }
    
    return rotation_e::ninety;
}

heading_e rotate_right(heading_e heading, rotation_e rotation) {
    auto h = static_cast<int>(heading);
    h = (h + static_cast<int>(rotation)) % static_cast<int>(heading_e::all_end);
    return static_cast<heading_e>(h);
}

heading_e rotate_left(heading_e heading, rotation_e rotation) {
    auto h = static_cast<int>(heading) + static_cast<int>(heading_e::all_end);
    h = (h - static_cast<int>(rotation)) % static_cast<int>(heading_e::all_end);
    return static_cast<heading_e>(h);
}

void navigate(ship_t& ship, control_t control) {
    switch (control.action)
    {
    case 'N':
        ship.y += control.value;
        break;
    case 'S':
        ship.y -= control.value;
        break;
    case 'E':
        ship.x += control.value;
        break;
    case 'W':
        ship.x -= control.value;
        break;
    case 'R':
        ship.heading = rotate_right(ship.heading, rotation(control.value));
        break;
    case 'L':
        ship.heading = rotate_left(ship.heading, rotation(control.value));
        break;
    case 'F':
        switch (ship.heading)
        {
        case heading_e::east:
            ship.x += control.value;
            break;
        case heading_e::south:
            ship.y -= control.value;
            break;
        case heading_e::west:
            ship.x -= control.value;
            break;
        case heading_e::north:
            ship.y += control.value;
            break;
        case heading_e::all_end:
            break;
        }
    default:
        break;
    }
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    std::vector<control_t> controls;
    for (const auto& line : lines) {
        const auto action = line.substr(0, 1);
        const auto value = line.substr(1,line.size());
        controls.push_back(control_t{action[0], stoi(value)});
    }

    ship_t ship{heading_e::east, 0, 0};
    for (const auto control : controls) {
        std::cout << control.action << ":" << control.value << "\n";
        navigate(ship, control);
    }

    std::cout << "part1: " << std::abs(ship.x) + std::abs(ship.y) << "\n";

    return 0;
}
