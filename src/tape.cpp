#include "tape.h"
#include "config_parser.h"
#include <chrono>
#include <exception>
#include <filesystem>
#include <iostream>
#include <thread>
#include <utility>

extern Config cfg;

Tape::Tape(const std::string &path) {
    std::filesystem::path p(path);
    std::string _filename = p.filename().string();
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Can't open \"" + _filename + "\" file";
        throw std::exception();
    }
    filename = cfg.TMP_DIR + _filename + "_formatted";
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
    std::filesystem::remove(cfg.TMP_DIR + _filename);
    std::filesystem::copy(filename, cfg.TMP_DIR + _filename);
    file = std::fstream(filename, std::ios::in | std::ios::out);
    file.seekg(pos);
    return {cfg.TMP_DIR + _filename, current, pos};
}

void Tape::save(const std::string& path) const {
    auto pos = file.tellg();
    file.close();
    std::filesystem::remove(path);
    std::filesystem::copy(filename, path);
    file = std::fstream(filename, std::ios::in | std::ios::out);
    file.seekg(pos);
}

int Tape::read() const {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.R_DELAY));
    return current;
}

void Tape::write(int new_value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.W_DELAY));
    file.seekg(-NUM_SIZE - 1, std::ios::cur);
    file << new_value << std::string(NUM_SIZE - std::to_string(new_value).size() + 1, ' ');
    current = new_value;
    file.seekg(file.tellg(), std::ios::beg);
}

void Tape::move_to_start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.S_DELAY * file.tellg()));
    file.seekg(0);
    move_right();
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
        if (way) {
            move_right();
        } else {
            move_left();
        }
    }
}

void Tape::move_left() {
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.S_DELAY));
    file.seekg(-2 * NUM_SIZE - 2, std::ios::cur);
    move_right();
}
