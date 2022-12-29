#include "tape.h"
#include <exception>
#include <iostream>

Tape::Tape(const std::string &filename, bool formatting) {
    if (formatting) {
        std::ifstream in(filename);
        std::ofstream out(filename + "_formatted");
        int x;
        while (in >> x) {
            out << x << std::string(NUM_SIZE - std::to_string(x).size(), ' ') << ' ';
        }
        out << '$';
        file = std::fstream(filename + "_formatted", std::ios::in | std::ios::out);
    } else {
        file = std::fstream(filename, std::ios::in | std::ios::out);
    }
    //    if (!file) {
    //        std::cerr << "Can not open file: " << filename << ";";
    //        throw std::exception();
    //    }
}



int Tape::w_current_cell(int new_value) {
    file.seekg(-NUM_SIZE - 1, std::ios::cur);
    file << new_value;
    current = new_value;
    file.seekg(file.tellg(), std::ios::beg);
}

void Tape::move_right() {
    file >> current;
    while (file.get() == ' ') {
        //do nothing;
    }
    file.seekg(-1, std::ios::cur);
}

void Tape::move_left() {
    //std::cout << "before left " << file.tellg() << '\n';
    file.seekg(-2 * NUM_SIZE - 2, std::ios::cur);
    //std::cout << "after left" << file.tellg() << '\n';
    move_right();
}

int main() {
    Tape sample("..\\files\\sample.txt", true);
    sample.move_right();
    std::cout << sample.r_current_cell() << '\n';
    sample.move_right();
    std::cout << sample.r_current_cell() << '\n';
    sample.w_current_cell(228);
    std::cout << sample.r_current_cell() << '\n';
    return 0;
}