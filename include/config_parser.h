#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

struct Config {
    size_t N;
    size_t M;
    int R_DELAY;
    int W_DELAY;
    int S_DELAY;
    std::string TMP_DIR;
    std::string FILE_DIR;


    Config(const std::string &filename) {
        std::ifstream fin(filename);
        std::string line;
        while (getline(fin, line)) {
            std::istringstream sin(line.substr(line.find("=") + 1));
            if (line.find("$N$") != -1) {
                sin >> N;
            } else if (line.find("$M$") != -1) {
                sin >> M;
            } else if (line.find("$R_DELAY$") != -1) {
                sin >> R_DELAY;
            } else if (line.find("$W_DELAY$") != -1) {
                sin >> W_DELAY;
            } else if (line.find("$S_DELAY$") != -1) {
                sin >> S_DELAY;
            } else if (line.find("$FILE_DIR$") != -1) {
                sin >> FILE_DIR;
            } else if (line.find("$TMP_DIR") != -1) {
                sin >> TMP_DIR;
            }
        }
    }
};