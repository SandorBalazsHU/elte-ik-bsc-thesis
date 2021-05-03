#include <gtest/gtest.h>
#include "pch.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

int main(int argc, char* args[]) {
    ::testing::InitGoogleTest(&argc, args);
    RUN_ALL_TESTS();
}