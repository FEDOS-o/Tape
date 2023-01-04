#include <gtest/gtest.h>
#include "tape_sorter.h"

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
       EXPECT_EQ(t.r_current_cell(), i);
   }
}

TEST(tape, double_read) {
    Tape t(cfg.FILE_DIR, "sample123");
    for (int i = 1; i < 11; i++) {
        t.move_right();
        EXPECT_EQ(t.r_current_cell(), i);
    }
    for (int i = 10; i > 0; i--) {
        EXPECT_EQ(t.r_current_cell(), i);
        t.move_left();
    }
}

TEST(tape, haotic_reading) {
    Tape t(cfg.FILE_DIR, "sample123");
    t.move_right();
    EXPECT_EQ(t.r_current_cell(), 1);
    t.move_right();
    EXPECT_EQ(t.r_current_cell(), 2);
    t.move_right();
    EXPECT_EQ(t.r_current_cell(), 3);
    t.move_right();
    EXPECT_EQ(t.r_current_cell(), 4);
    t.move_left();
    EXPECT_EQ(t.r_current_cell(), 3);
    t.move_left();
    EXPECT_EQ(t.r_current_cell(), 2);
    t.move_right();
    t.move_right();
    t.move_right();
    t.move_right();
    t.move_right();
    t.move_right();
    EXPECT_EQ(t.r_current_cell(), 8);
    t.move_left();
    t.move_left();
    t.move_left();
    EXPECT_EQ(t.r_current_cell(), 5);
}

TEST(tape, make_copy) {
    std::string filename1, filename2;
    {
        Tape t(cfg.FILE_DIR, "sample123");
        filename1 = t.filename;
        Tape t_copy = t.make_copy("sample123_copy");
        filename2 = t_copy.filename;
    }
  //  EXPECT_TRUE(compare_files(filename1, cfg.TMP_DIR + "\\" + filename2));
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
            EXPECT_EQ(t.r_current_cell(), i);
            EXPECT_EQ(t.r_current_cell(), t_copy.r_current_cell());
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
            t.w_current_cell(11 - i);
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
            t.w_current_cell(11 - i);
        }
        t.w_current_cell(10);
        for (int i = 2; i < 11; i++) {
            t.move_left();
            EXPECT_EQ(t.r_current_cell(), i);
            t.w_current_cell(11 - i);
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
        t.w_current_cell(25);
        t.move_left();
        t.w_current_cell(17);
        t.move_right();
        t.move_right();
        t.move_right();
        t.move_right();
        t.w_current_cell(555);
        t.move_left();
        t.w_current_cell(777);
        t.move_right();
        t.move_right();
        t.move_right();
        t.w_current_cell(1111);
    }
    EXPECT_TRUE(compare_files(filename, cfg.FILE_DIR + "\\" + "haotic_writing_result_formatted"));
}