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

    //make copy of tape and place it to TMP DIR with filename
    Tape make_copy(const std::string &filename) const;

    //save tape to the path
    void save(const std::string& path) const;

    int read() const;

    void write(int);

    void move_left();

    void move_right();

    void move_to_start();

    //make shift to variable amount of cells
    void move(int);

    void swap(Tape &other);

    std::string filename;

private:
    int current;
    mutable std::fstream file;
    static const int NUM_SIZE = 11;//std::to_string(INT32_MIN).size();
};