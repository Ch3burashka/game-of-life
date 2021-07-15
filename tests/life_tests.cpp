#include <gtest/gtest.h>
#include "Life.h"
#include "iostream"

TEST(Life, SizeThrow) {
    ASSERT_THROW(Life life(0, 0), LifeException);
}

TEST(Life, Values) {
    Life life(3, 3);
    ASSERT_FALSE(life.GetCellsStates()[0]);
}

TEST(Life, ToString) {
    Life life(3, 3);
    ASSERT_EQ(".......\n"
              "|x x x|\n"
              "|x x x|\n"
              "|x x x|\n"
              ".......", std::string(life));
}

TEST(Life, Set) {
    Life life(3, 3);
    life.SetStateAlive(0, 0);
    ASSERT_EQ(".......\n"
              "|o x x|\n"
              "|x x x|\n"
              "|x x x|\n"
              ".......", std::string(life));
}

TEST(Life, Set1) {
    Life life(3, 3);
    ASSERT_THROW(life.SetStateAlive(-1, 0), LifeException);
    ASSERT_THROW(life.SetStateAlive(4, 0), LifeException);
    ASSERT_THROW(life.SetStateDead(-1, 0), LifeException);
    ASSERT_THROW(life.SetStateDead(4, 0), LifeException);
}

TEST(Life, Set2) {
    Life life(3, 3);
    life.SetStateAlive(0, 0);
    life.SetStateDead(0, 0);
    ASSERT_EQ(".......\n"
              "|x x x|\n"
              "|x x x|\n"
              "|x x x|\n"
              ".......", std::string(life));
}

TEST(Life, Get) {
    Life life(3, 3);
    life.SetStateAlive(0, 0);
    ASSERT_THROW(life.GetStateCell(-1, 0), LifeException);
    ASSERT_TRUE(life.GetCellsStates()[0]);
    ASSERT_TRUE(life.GetStateCell(0, 0));
}

TEST(Life, Operator) {
    Life life(3, 3);
    std::stringstream ss;
    ss << life;
    ASSERT_EQ(ss.str(), ".......\n|x x x|\n|x x x|\n|x x x|\n.......");
}

TEST(Life, Near) {
    Life life(4, 3);
    life.SetStateAlive(1, 0);
    life.SetStateAlive(0, 1);
    life.SetStateAlive(1, 1);
    life.SetStateAlive(3, 0);
    life.SetStateAlive(3, 1);
    std::cout << life << std::endl;
    ASSERT_EQ(life.GetLiveCountNearCell(0, 0), 5);
    ASSERT_EQ(life.GetLiveCountNearCell(2, 2), 4);
}

TEST(Life, Mutate) {
    Life life(10, 10);
    life.SetStateAlive(3, 1);
    life.SetStateAlive(4, 2);
    life.SetStateAlive(4, 3);
    life.SetStateAlive(3, 3);
    life.SetStateAlive(2, 3);
    std::cout << life << std::endl;
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    life.Mutate();
    std::cout << life << std::endl;
}
