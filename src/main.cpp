#include "config_parser.h"
#include "tape_sorter.h"
#include <filesystem>
#include <iostream>

Config cfg("..\\config.txt");


int main() {
    Tape_sorter::sort("sample", "output");
    return 0;
}
