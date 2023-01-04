#include "tape_sorter.h"
#include <chrono>
#include <gtest/gtest.h>
#include <random>


std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

Config cfg("..\\tests\\config.txt");


bool compare_files(std::string filename1, std::string filename2) {
    std::ifstream file1(filename1), file2(filename2);
    if (!file1) {
        throw std::exception();
    }
    if (!file2) {
        throw std::exception();
    }
    std::string f1, f2;
    std::getline(file1, f1);
    std::getline(file2, f2);
    file1.close();
    file2.close();
    return f1 == f2;
}

TEST(tape, straight_read) {
    Tape t(cfg.FILE_DIR, "sample123");
    for (int i = 1; i < 11; i++) {
        t.move_right();
        EXPECT_EQ(t.read(), i);
    }
}

TEST(tape, double_read) {
    Tape t(cfg.FILE_DIR, "sample123");
    for (int i = 1; i < 11; i++) {
        t.move_right();
        EXPECT_EQ(t.read(), i);
    }
    for (int i = 10; i > 0; i--) {
        EXPECT_EQ(t.read(), i);
        t.move_left();
    }
}

TEST(tape, haotic_reading) {
    Tape t(cfg.FILE_DIR, "sample123");
    t.move_right();
    EXPECT_EQ(t.read(), 1);
    t.move_right();
    EXPECT_EQ(t.read(), 2);
    t.move_right();
    EXPECT_EQ(t.read(), 3);
    t.move_right();
    EXPECT_EQ(t.read(), 4);
    t.move_left();
    EXPECT_EQ(t.read(), 3);
    t.move_left();
    EXPECT_EQ(t.read(), 2);
    t.move_right();
    t.move_right();
    t.move_right();
    t.move_right();
    t.move_right();
    t.move_right();
    EXPECT_EQ(t.read(), 8);
    t.move_left();
    t.move_left();
    t.move_left();
    EXPECT_EQ(t.read(), 5);
}

TEST(tape, make_copy) {
    std::string filename1, filename2;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename1 = t.filename;
        Tape t_copy = t.make_copy(cfg.TMP_DIR + "\\sample123_copy");
        filename2 = t_copy.filename;
    }
    EXPECT_TRUE(compare_files(filename1, filename2));
}

TEST(tape, make_copy_position) {
    std::string filename1, filename2;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        t.move_right();
        t.move_right();
        t.move_right();
        filename1 = t.filename;
        Tape t_copy = t.make_copy(cfg.TMP_DIR + "\\" + "sample123_copy");
        filename2 = t_copy.filename;
        for (int i = 3; i < 11; i++) {
            EXPECT_EQ(t.read(), i);
            EXPECT_EQ(t.read(), t_copy.read());
            t.move_right();
            t_copy.move_right();
        }
    }
    EXPECT_TRUE(compare_files(filename1, filename2));
}

TEST(tape, write_straight) {
    std::string filename;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename = t.filename;
        for (int i = 1; i < 11; i++) {
            t.move_right();
            t.write(11 - i);
        }
    }
    EXPECT_TRUE(compare_files(filename, cfg.FILE_DIR + "\\sample321_formatted"));
}

TEST(tape, write_double) {
    std::string filename;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename = t.filename;
        for (int i = 1; i < 11; i++) {
            t.move_right();
            t.write(11 - i);
        }
        t.write(10);
        for (int i = 2; i < 11; i++) {
            t.move_left();
            EXPECT_EQ(t.read(), i);
            t.write(11 - i);
        }
    }
    EXPECT_TRUE(compare_files(filename, cfg.FILE_DIR + "\\sample123_formatted"));
}

TEST(tape, haotic_writing) {
    //1 17 25 4 777 555 7 1111 9 10
    std::string filename;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename = t.filename;
        t.move_right();
        t.move_right();
        t.move_right();
        t.write(25);
        t.move_left();
        t.write(17);
        t.move_right();
        t.move_right();
        t.move_right();
        t.move_right();
        t.write(555);
        t.move_left();
        t.write(777);
        t.move_right();
        t.move_right();
        t.move_right();
        t.write(1111);
    }
    EXPECT_TRUE(compare_files(filename, cfg.FILE_DIR + "\\" + "haotic_writing_result_formatted"));
}

TEST(tape, move_to_start) {
    std::string filename;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename = t.filename;
        for (int i = 1; i < 4; i++) {
            t.move_right();
            t.write(999 + i);
        }
        t.move_to_start();
        for (int i = 10; i > 0; i--) {
            t.write(i);
            t.move_right();
        }
    }
    EXPECT_TRUE(compare_files(filename, cfg.FILE_DIR + "\\sample321_formatted"));
}

TEST(tape, move) {
    std::string filename1, filename2;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename1 = t.filename;
        Tape t_copy = t.make_copy(cfg.TMP_DIR + "\\" + "sample123_copy");
        filename2 = t_copy.filename;
        t.move(5);
        t_copy.move(5);
        EXPECT_EQ(t.read(), t_copy.read());
        t.move(-3);
        t_copy.move(-3);
        EXPECT_EQ(t.read(), t_copy.read());
        t.move(7);
        t_copy.move(7);
        EXPECT_EQ(t.read(), t_copy.read());
        t.move_left();
        t_copy.move(-1);
        EXPECT_EQ(t.read(), t_copy.read());
        t.move_right();
        t_copy.move(1);
        EXPECT_EQ(t.read(), t_copy.read());
    }
    EXPECT_TRUE(compare_files(filename1, filename2));
}

TEST(tape, swap) {
    std::string filename1, filename2;
    {
        Tape t1(cfg.FILE_DIR, "sample123");
        filename1 = t1.filename;
        Tape t2(cfg.FILE_DIR, "sample321");
        filename2 = t2.filename;
        t1.move(3);
        t2.move(6);
        EXPECT_EQ(t1.read(), 3);
        EXPECT_EQ(t2.read(), 5);
        t1.swap(t2);
        EXPECT_EQ(t1.read(), 5);
        EXPECT_EQ(t2.read(), 3);
        t1.move_right();
        EXPECT_EQ(t1.read(), 4);
        t2.move_left();
        EXPECT_EQ(t2.read(), 2);
    }
    EXPECT_TRUE(compare_files(filename2, cfg.FILE_DIR + "\\sample321_formatted"));
    EXPECT_TRUE(compare_files(filename1, cfg.FILE_DIR + "\\sample123_formatted"));
}


TEST(tape_sorter, already_sorted) {
    {
        cfg.N = 10;
        cfg.M = 3;
        Tape_sorter::sort("sample123", "sample123_sort_result");
    }
    EXPECT_TRUE(compare_files(cfg.FILE_DIR + "\\sample123_formatted", cfg.FILE_DIR + "\\sample123_sort_result"));
}

TEST(tape_sorter, reversed_order) {
    {
        cfg.N = 10;
        cfg.M = 3;
        Tape_sorter::sort("sample321", "sample123_sort_result");
    }
    bool res = compare_files(cfg.FILE_DIR + "\\sample123_formatted", cfg.FILE_DIR + "\\sample123_sort_result");
    EXPECT_TRUE(res);
}


TEST(tape_sorter, different_M) {
    for (int i = 2; i < 20; i++) {
        {
            cfg.N = 10;
            cfg.M = i;
            Tape_sorter::sort("sample321", "sample123_sort_result");
        }
        bool res = compare_files(cfg.FILE_DIR + "\\sample123_formatted", cfg.FILE_DIR + "\\sample123_sort_result");
        EXPECT_TRUE(res);
    }
}

TEST(tape_sorter, random_numbers) {
    cfg.N = 25;
    cfg.M = 7;
    std::vector<int> result;
    std::ofstream random_test(cfg.FILE_DIR + "\\random_test");
    for (int i = 0; i < cfg.N; i++) {
        int r = static_cast<int>(rnd());
        result.push_back(r);
        random_test << r << ' ';
    }
    random_test.close();
    std::sort(result.begin(), result.end());
    random_test = std::ofstream(cfg.FILE_DIR + "\\random_test_sorted");
    for (int i = 0; i < cfg.N; i++) {
        random_test << result[i] << ' ';
    }
    random_test.close();
    {
        Tape answer(cfg.FILE_DIR, "random_test_sorted");
        Tape_sorter::sort("random_test", "random_test_result");
    }
    bool cmp = compare_files(cfg.TMP_DIR + "\\random_test_sorted_formatted", cfg.FILE_DIR + "\\random_test_result");
    EXPECT_TRUE(cmp);
}

TEST(tape_sorter, random_number_diff_M) {
    cfg.N = 40;

    for (int j = 2; j < 50; j++) {
        cfg.M = j;
        std::vector<int> result;
        std::ofstream random_test(cfg.FILE_DIR + "\\random_test");
        for (int i = 0; i < cfg.N; i++) {
            int r = static_cast<int>(rnd());
            result.push_back(r);
            random_test << r << ' ';
        }
        random_test.close();
        std::sort(result.begin(), result.end());
        random_test = std::ofstream(cfg.FILE_DIR + "\\random_test_sorted");
        for (int i = 0; i < cfg.N; i++) {
            random_test << result[i] << ' ';
        }
        random_test.close();
        {
            Tape answer(cfg.FILE_DIR, "random_test_sorted");
            Tape_sorter::sort("random_test", "random_test_result");
        }
        bool cmp = compare_files(cfg.TMP_DIR + "\\random_test_sorted_formatted", cfg.FILE_DIR + "\\random_test_result");
        EXPECT_TRUE(cmp);
    }
}