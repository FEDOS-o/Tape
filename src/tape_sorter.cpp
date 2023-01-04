#include "tape_sorter.h"

extern Config cfg;

namespace {


    void merge_move(Tape &t, size_t &pointer, int shift) {
        pointer += shift;
        if (pointer < cfg.N) {
            t.move(shift);
        }
    }

    void merge_blocks(Tape &tmp1, Tape &tmp2, Tape &res, size_t block_size, size_t N) {
        tmp2.move(block_size);
        size_t pointer1, pointer2, res_pointer = 0;
        pointer1 = 0, pointer2 = block_size;
        while (res_pointer != N) {
            size_t border1 = std::min(pointer1 + block_size, N), border2 = std::min(pointer2 + block_size, N);
            while (pointer1 < border1 || pointer2 < border2) {
                if (pointer1 >= border1) {
                    auto x = tmp2.read();
                    res.write(x);
                    merge_move(res, res_pointer, 1);
                    merge_move(tmp2, pointer2, 1);
                } else if (pointer2 >= border2) {
                    auto x = tmp1.read();
                    res.write(x);
                    merge_move(res, res_pointer, 1);
                    merge_move(tmp1, pointer1, 1);

                } else {
                    auto x1 = tmp1.read(), x2 = tmp2.read();
                    if (x1 <= x2) {
                        res.write(x1);
                        merge_move(res, res_pointer, 1);
                        merge_move(tmp1, pointer1, 1);

                    } else {
                        res.write(x2);
                        merge_move(res, res_pointer, 1);
                        merge_move(tmp2, pointer2, 1);
                    }
                }
            }
            merge_move(tmp1, pointer1, block_size);
            merge_move(tmp2, pointer2, block_size);
        }
    }
}// namespace

void Tape_sorter::sort(std::string input_file, std::string output_file) {
    size_t N = cfg.N, M = cfg.M;
    std::vector<Tape> tmp;
    Tape tmp1(cfg.FILE_DIR + input_file);
    std::string filename = tmp1.filename;
    Tape tmp3 = tmp1.make_copy(input_file + "_formatted3");
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
    Tape tmp2 = tmp1.make_copy(input_file + "_formatted2");
    int tmp_file_number = 0, max_tmp_files = 2;
    for (size_t block_size = M; 2 * block_size <= N + block_size - 1; block_size *= 2) {
        merge_blocks(tmp1, tmp2, tmp3, block_size, N);
        tmp1.swap(tmp3);
        tmp1.move_to_start();
        tmp3.move_to_start();
        Tape tmp4 = tmp1.make_copy(input_file + "_formatted" + std::to_string(tmp_file_number++ % max_tmp_files));
        tmp2.swap(tmp4);
    }
    tmp1.save(cfg.FILE_DIR + "\\" + output_file);
}
