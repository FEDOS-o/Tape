#pragma once


#include <fstream>


struct Tape {

    Tape(const std::string& filename, bool formatting);

    ~Tape() {
        file.close();
    }

    Tape make_copy(std::string filename);

    int r_current_cell() {
        return current;
    }

    int w_current_cell(int);

    void move_left();

    void move_right();

private:
    int current;
    std::fstream file;
    static const int NUM_SIZE = 11; //std::to_string(INT32_MIN).size();
};