#include <gtest/gtest.h>

#include <rtx/Matrix.h>

TEST(TestMatrix, Inverse) {
  rtx::Matrix mat;

  float data[16] = {-1, 0, 1, 1, 1, -2, 1, -1, 1, 0, -1, 1, 1, 0, 1, -1};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat(i, j) = data[i + j * 4];
    }
  }

  float result[16] = {0.0f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.5f,
                      0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f,  0.5f, 0.0f};

  rtx::Matrix inv = mat.inverse();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQ(result[i + j * 4], inv(i, j));
    }
  }

  EXPECT_TRUE(true);
}