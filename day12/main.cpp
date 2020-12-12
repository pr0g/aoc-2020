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

struct waypoint_t
{
    int x;
    int y;
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

double deg_to_rad(double degrees) {
    return (3.14159265359 / 180.0) * degrees;
}

waypoint_t rotate(int angle, int direction, int xx, int yy) {
    double a = deg_to_rad(double(angle * direction));
    double xd = double(xx);
    double yd = double(yy);
    double x = (xd * std::cos(a) - yd * std::sin(a));
    double y = (xd * std::sin(a) + yd * std::cos(a));

    return waypoint_t{int(std::round(x)), int(std::round(y))};
}

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

void navigate(ship_t& ship, waypoint_t& waypoint, control_t control) {
    switch (control.action)
    {
    case 'N':
        waypoint.y += control.value;
        break;
    case 'S':
        waypoint.y -= control.value;
        break;
    case 'E':
        waypoint.x += control.value;
        break;
    case 'W':
        waypoint.x -= control.value;
        break;
    case 'R':
        waypoint = rotate(control.value, -1, waypoint.x, waypoint.y);
        break;
    case 'L':
        waypoint = rotate(control.value, 1, waypoint.x, waypoint.y);
        break;
    case 'F':
        ship.x += waypoint.x * control.value;
        ship.y += waypoint.y * control.value;
        break;
    default:
        break;
    }
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    // waypoint_t waypoint {1, 0};
    // auto wp = rotate(90, 1, 1, 0);
    // std::cout << wp.x << ":" << wp.y << "\n";

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

    waypoint_t waypoint {10, 1};
    ship_t ship{heading_e::east, 0, 0};
    for (const auto control : controls) {
        navigate(ship, waypoint, control);
    }

    std::cout << "part2: " << std::abs(ship.x) + std::abs(ship.y) << "\n";

    return 0;
}
