#include <gtest/gtest.h>
#include <rtx/Point.h>
#include <rtx/Vector.h>

TEST(TestVector, Distance) {
  rtx::Point v1(7, 4, 3);
  rtx::Point v2(17, 6, 2);

  ASSERT_EQ(v1.distance(v2), 10.246951f);

  EXPECT_TRUE(true);
}

TEST(TestVector, Bracket) {
  rtx::Vector vec(1, 2, 3);

  for (int i = 0; i < 3; i++) ASSERT_EQ(vec[i], i + 1);

  EXPECT_TRUE(true);
}