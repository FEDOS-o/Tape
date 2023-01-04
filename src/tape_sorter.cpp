#include "tape_sorter.h"

extern Config cfg;

void Tape_sorter::sort(std::string input_file, std::string output_file) {
    size_t N = cfg.N, M = cfg.M;
    std::vector<Tape> tmp;
    Tape tmp1(cfg.FILE_DIR, input_file);
    std::string filename = tmp1.filename;
    Tape tmp3 = tmp1.make_copy(filename + "3");
    for (size_t i = 0; i < N;) {
        std::vector<int> list;
        for (int j = 0; j < M && i < N; j++, i++) {
            tmp1.move_right();
            list.push_back(tmp1.read());
        }
        std::sort(list.begin(), list.end());
        for (auto x: list) {
            tmp3.move_right();
            tmp3.write(x);
        }
    }
    tmp1.swap(tmp3);
    tmp1.move_to_start();
    tmp3.move_to_start();
    Tape tmp2 = tmp1.make_copy(filename + "2");
    for (size_t block_size = M; 2 * block_size < N + block_size - 1; block_size *= 2) {
        merge_blocks(tmp1, tmp2, tmp3, block_size, N);
        tmp1.swap(tmp3);
        tmp1.move_to_start();
        tmp3.move_to_start();
        Tape tmp4 = tmp1.make_copy(filename + "tmp" + std::to_string(block_size));
        tmp2.swap(tmp4);
    }
    Tape result = tmp1.make_copy(cfg.FILE_DIR + "\\" + output_file);
}

void Tape_sorter::merge_blocks(Tape& tmp1, Tape& tmp2, Tape& res, size_t block_size, size_t N) {
    tmp2.move(block_size);
    size_t pointer1, pointer2;
    pointer1 = 0, pointer2 = block_size;
    for (size_t i = 0; i < (N + block_size - 1) / block_size / 2; i++) {
        size_t border1 = pointer1 + block_size, border2 = std::min(pointer2 + block_size, N);
        while (pointer1 != border1 || pointer2 != border2) {
            if (pointer1 == border1) {
                auto x = tmp2.read();
                res.write(x);
                pointer2++;
                res.move_right();
                tmp2.move_right();
            } else if (pointer2 == border2) {
                auto x = tmp1.read();
                res.write(x);
                pointer1++;
                res.move_right();
                tmp1.move_right();
            } else {
                auto x1 = tmp1.read(), x2 = tmp2.read();
                if (x1 <= x2) {
                    res.write(x1);
                    pointer1++;
                    res.move_right();
                    tmp1.move_right();
                } else {
                    res.write(x2);
                    pointer2++;
                    res.move_right();
                    tmp2.move_right();
                }
            }
        }
        tmp1.move(block_size);
        pointer1 += block_size;
        tmp2.move(block_size);
        pointer2 += block_size;
    }
}