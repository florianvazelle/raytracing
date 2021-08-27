#include <gtest/gtest.h>
#include <rtx/Entity.h>

TEST(TestEntity, Translation) {
  float x = 2, y = 1, z = 5;

  rtx::Entity e;
  e.translate(x, y, z);

  float result[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f, x,    y,    z,    1.0f};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQ(result[i + j * 4], e.trans(i, j));
    }
  }

  float resultInv[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f, -x,   -y,   -z,   1.0f};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQ(resultInv[i + j * 4], e.transInv(i, j));
    }
  }

  EXPECT_TRUE(true);
}