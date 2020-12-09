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

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct nop_t { int value; };
struct acc_t { int value; };
struct jmp_t { int value; };

using instruction_t = std::variant<std::monostate, nop_t, acc_t, jmp_t>;

instruction_t decode_instruction(
    const std::string& opcode, const std::string& value) {
    if (opcode == "nop") {
        return nop_t{ stoi(value) };
    } else if (opcode == "acc") {
        return acc_t { stoi(value) };
    } else if (opcode == "jmp") {
        return jmp_t { stoi(value) };
    }

    return std::monostate{};
}

int main(int argc, char** argv) {
    std::ifstream reader("input.txt");

    std::vector<std::string> instruction_strs;
    for (std::string line; std::getline(reader, line);) {
        instruction_strs.push_back(line);
    }

    for (const auto& instruction : instruction_strs) {
        std::cout << instruction << "\n";
    }

    std::vector<instruction_t> instructions;
    for (const auto& instruction : instruction_strs) {
        std::stringstream ss(instruction);
        std::string opcode, value;
        ss >> opcode >> value;

        std::cout << opcode << ":" << value << "\n";

        instructions.push_back(decode_instruction(opcode, value));
    }

    for (const auto& instruction : instructions) {
        std::visit(overloaded {
            [](nop_t arg) { std::cout << "nop\n"; },
            [](acc_t arg) { std::cout << "acc:" << arg.value << "\n"; },
            [](jmp_t arg) { std::cout << "jmp:" << arg.value << "\n"; },
            [](std::monostate arg) { std::cout << "unhandled\n"; }
        }, instruction);
    }

    struct program_t {
        int ip = 0;
        int acc = 0;
    };

    auto check = [&](program_t prog, std::vector<int>& memory) { 
        int value = memory[prog.ip];
        if (value > 0) {
            // enable for part 1
            // std::cout << "part1: " << prog.acc << "\n";
            return false;
        }
        memory[prog.ip]++;
        return true;
    };

    int jmp_change_index = 0;
    int nop_change_index = 0;

    program_t program;
    for (bool valid_program = false, loop_detected = false; !valid_program && !loop_detected;) {
        std::vector<int> memory(instructions.size(), 0);

        std::vector instructions_copy = instructions;
        // enable for part 2
        if (auto it = std::find_if(
            instructions.begin() + jmp_change_index, instructions.end(),
            [](const auto& inst) {
                return std::holds_alternative<jmp_t>(inst);
            });
            it != instructions.end()) {
            const auto offset = it - instructions.begin();
            instruction_t inst = *it;
            if (auto jmp = std::get_if<jmp_t>(&inst)) {
                instructions_copy[offset] = nop_t{ jmp->value };
            }
            jmp_change_index = offset + 1;
        }

        program = {};
        for (;;) {
            const auto instruction = instructions_copy[program.ip];
            auto okay = std::visit(overloaded {
                [&](nop_t arg) { program.ip++; return check(program, memory); },
                [&](acc_t arg) { program.acc += arg.value; program.ip++; return check(program, memory); },
                [&](jmp_t arg) { program.ip += arg.value; return check(program, memory); },
                [](std::monostate arg) { std::cout << "unhandled\n";  return false; }
            }, instruction);

            if (!okay) {
                // enable for part 1
                // loop_detected = true;
                break;
            }

            if (program.ip >= instructions.size()) {
                valid_program = true;
                break;
            }
        }
    }

    std::cout << "part2: " << program.acc << "\n";

    return 0;
}
