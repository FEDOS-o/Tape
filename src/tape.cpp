#include "tape.h"
#include "config_parser.h"
#include <chrono>
#include <exception>
#include <filesystem>
#include <iostream>
#include <thread>
#include <utility>

extern Config cfg;

Tape::Tape(const std::string &_dirname, const std::string &_filename) {
    std::ifstream in(_dirname + "\\" + _filename);
    filename = cfg.FILE_DIR + "\\tmp\\" + _filename + "_formatted";
    std::ofstream out(filename);
    int x;
    while (in >> x) {
        out << x << std::string(NUM_SIZE - std::to_string(x).size(), ' ') << ' ';
    }
    out << '$';
    file = std::fstream(filename, std::ios::in | std::ios::out);
}

Tape::Tape(const std::string &_filename, int curr, auto p) {
    filename = _filename;
    current = curr;
    file = std::fstream(_filename, std::ios::in | std::ios::out);
    file.seekg(p);
}


Tape Tape::make_copy(const std::string &_filename) const {
    auto pos = file.tellg();
    file.close();
    std::filesystem::remove(_filename);
    std::filesystem::copy(filename, _filename);
    file = std::fstream(filename, std::ios::in | std::ios::out);
    file.seekg(pos);
    return {_filename, current, pos};
}

int Tape::r_current_cell() const {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.R_DELAY));
    return current;
}

void Tape::w_current_cell(int new_value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.W_DELAY));
    file.seekg(-NUM_SIZE - 1, std::ios::cur);
    file << new_value << std::string(NUM_SIZE - std::to_string(new_value).size(), ' ');
    current = new_value;
    file.seekg(file.tellg(), std::ios::beg);
}

void Tape::move_to_start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.S_DELAY * file.tellg()));
    file.seekg(0);
}

void Tape::move_right() {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.S_DELAY));
    int x;
    file >> x;
    current = x;
    while (file.get() == ' ') {
        //do nothing;
    }
    file.seekg(-1, std::ios::cur);
}

void Tape::move(int shift) {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.S_DELAY * std::abs(shift)));
    bool way = (shift > 0);

    for (size_t i = 0; i < abs(shift); i++) {
        auto pos = file.tellg();
        if (way) {
            move_right();
        } else {
            move_left();
        }
    }
    //file.seekg(shift * (NUM_SIZE + 1), std::ios::cur);
}

void Tape::move_left() {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.S_DELAY));
    file.seekg(-2 * NUM_SIZE - 2, std::ios::cur);
    move_right();
}
