#pragma once


#include <fstream>


struct Tape {

    Tape(const std::string &path);

private:
    Tape(const std::string &_filename, int current, auto pos);
public:
    ~Tape() {
        file.flush();
        file.close();
    }

    Tape make_copy(const std::string &filename) const;

    void save(const std::string& path) const;

    int read() const;

    void write(int);

    void move_left();

    void move_right();

    void move_to_start();

    void move(int);

    void swap(Tape &other) {
        using std::swap;
        swap(current, other.current);
        swap(filename, other.filename);
        auto pos1 = file.tellg();
        auto pos2 = other.file.tellg();
        file.close();
        other.file.close();
        file = std::fstream(filename, std::ios::in | std::ios::out);
        file.seekg(pos2);
        other.file = std::fstream(other.filename, std::ios::in | std::ios::out);
        other.file.seekg(pos1);
    }

    std::string filename;

private:
    int current;
    mutable std::fstream file;
    static const int NUM_SIZE = 11;//std::to_string(INT32_MIN).size();
};