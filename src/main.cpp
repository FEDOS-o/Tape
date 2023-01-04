#include "config_parser.h"
#include "tape_sorter.h"
#include <iostream>

Config cfg("..\\config.txt");


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: [input] [output]";
        return 0;
    }
    Tape_sorter::sort(argv[1], argv[2]);
    return 0;
}
