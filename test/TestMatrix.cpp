#include <gtest/gtest.h>
#include <rtx/Entity.h>
#include <rtx/Matrix.h>
#include <rtx/Point.h>
#include <rtx/Vector.h>

TEST(TestMatrix, Identity) {
  rtx::Matrix mat;

  float result[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQ(result[i + j * 4], mat(i, j));
    }
  }

  EXPECT_TRUE(true);
}

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

TEST(TestMatrix, PointMultiplication) {
  rtx::Point p(-1, -1, 0);
  rtx::Point result(-1, -1, 3);

  rtx::Entity e;
  e.translate(0, 0, 3);

  ASSERT_EQ(e.trans * p, result);
  EXPECT_TRUE(true);
}

TEST(TestMatrix, VectorMultiplication) {
  rtx::Vector v(-1, -1, 0);
  rtx::Point result(-1, -1, 0);

  rtx::Entity e;
  e.translate(0, 0, 3);

  ASSERT_EQ(e.trans * v, result);
  EXPECT_TRUE(true);
}