#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "config_parser.h"
#include "tape.h"



struct Tape_sorter {

    static void sort(std::string input_file, std::string output_file);

private:
    static void merge_blocks(Tape&, Tape&, Tape&, size_t, size_t);
};
