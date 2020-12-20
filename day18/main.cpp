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

struct number_t
{
    int value;
};

struct open_paren_t
{
};

struct close_paren_t
{
};

struct add_t
{
};

struct multiply_t
{
};

struct sub_t
{
};

using operation_t =
    std::variant<number_t, open_paren_t, close_paren_t, add_t, multiply_t, sub_t>;

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

//struct expression_t
//{
//};

enum class mode_e
{
    none,
    value,
    op,
    paren
};

// stack
// tokenize first

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> lines;
    for(std::string line; std::getline(reader, line);) {
        lines.push_back(line);
    }

    std::vector<operation_t> operations;
    auto store_number = [&operations](mode_e& mode, std::string& number_buffer) {
        if (mode == mode_e::value) {
            int number = stoi(number_buffer);
            number_t num {number};
            operations.emplace_back(num);
            mode = mode_e::none;
            number_buffer.clear();
        }
    };

    for (const auto& line : lines) {
        mode_e active_mode = mode_e::none;
        std::string number_buffer;
        for (const auto& symbol : line) {
            if (symbol >= '0' && symbol <= '9') {
                active_mode = mode_e::value;
                number_buffer += symbol;
            } else if (symbol == ' ') {
                store_number(active_mode, number_buffer);
            } else if (symbol == '(') {
                operations.emplace_back(open_paren_t{});
            } else if (symbol == ')') {
                store_number(active_mode, number_buffer);
                operations.emplace_back(close_paren_t{});
            } else if (symbol == '+') {
                operations.emplace_back(add_t{});
            } else if (symbol == '*') {
                operations.emplace_back(multiply_t{});
            } else if (symbol == '-') {
                operations.emplace_back(sub_t{});
            }
        }
        store_number(active_mode, number_buffer);
    }

    for (auto& op: operations) {
        std::visit(overloaded {
            [](number_t arg) { std::cout << arg.value; },
            [](open_paren_t) { std::cout << "("; },
            [](close_paren_t) { std::cout << ")"; },
            [](add_t) { std::cout << " + "; },
            [](multiply_t) { std::cout << " * "; },
            [](sub_t) { std::cout << " - "; },
        }, op);
    }

    std::cout << "\n";

    std::queue<operation_t> output;
    std::stack<operation_t> op_stack;
    for (auto& op: operations) {
        std::visit(overloaded {
            [&output](number_t arg) {
                output.push(arg);
            },
            [&op_stack](open_paren_t) {
                op_stack.push(open_paren_t{});
            },
            [&op_stack, &output](close_paren_t) {
                while (!std::holds_alternative<open_paren_t>(op_stack.top())) {
                    operation_t op = op_stack.top();
                    op_stack.pop();
                    output.push(op);
                }
                if (std::holds_alternative<open_paren_t>(op_stack.top())) {
                    op_stack.pop();
                }
            },
            [&op_stack, &output](add_t) {
                while (!op_stack.empty() && (std::holds_alternative<add_t>(op_stack.top())
                    || std::holds_alternative<multiply_t>(op_stack.top()))) {
                    operation_t op = op_stack.top();
                    op_stack.pop();
                    output.push(op);
                }
                op_stack.push(add_t{});
            },
            [&op_stack, &output](multiply_t) {
                while (!op_stack.empty() && (std::holds_alternative<add_t>(op_stack.top())
                    || std::holds_alternative<multiply_t>(op_stack.top()))) {
                    operation_t op = op_stack.top();
                    op_stack.pop();
                    output.push(op);
                }
                op_stack.push(multiply_t{});
            },
            [&op_stack](sub_t) {
                op_stack.push(sub_t{});
            },
        }, op);
    }

    while (!op_stack.empty()) {
        operation_t op = op_stack.top();
        output.push(op);
        op_stack.pop();
    }

    std::stack<operation_t> rp;
    while (!output.empty()) {
        operation_t op = output.front();
        output.pop();

        if (std::holds_alternative<number_t>(op)) {
            rp.push(op);
            continue;
        }

        if (std::holds_alternative<multiply_t>(op)) {
            operation_t op1 = rp.top();
            rp.pop();
            operation_t op2 = rp.top();
            rp.pop();
            auto res = number_t{std::get<number_t>(op1).value * std::get<number_t>(op2).value};
            rp.push(res);
        }

        if (std::holds_alternative<add_t>(op)) {
            operation_t op1 = rp.top();
            rp.pop();
            operation_t op2 = rp.top();
            rp.pop();
            auto res = number_t{std::get<number_t>(op1).value + std::get<number_t>(op2).value};
            rp.push(res);
        }
    }

    std::cout << "part1: " << std::get<number_t>(rp.top()).value << '\n';

    // parse lines
    // 1 + (2 * 3) + (4 * (5 + 6))

    return 0;
}
