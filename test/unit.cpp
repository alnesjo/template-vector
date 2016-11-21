#include "gtest/gtest.h"
#include "vector.hpp"

namespace {
  class VectorTest : public ::testing::Test {
  protected:
    VectorTest() {
    }
    virtual ~VectorTest() {
    }
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
  };

  TEST_F(VectorTest, PushBack) {
    alnesjo::Vector<int> v;
    for (int i = 0; i < 10; i++) {
      v.push_back(i);
    }
    EXPECT_EQ(9, v[9]);
    EXPECT_EQ(4, v[4]);
  }
  TEST_F(VectorTest, InsertAtFront) {
    alnesjo::Vector<int> v;
    for (int i = 0; i < 10; i++) {
      v.insert(0,i);
    }
    EXPECT_EQ(0, v[9]);
    EXPECT_EQ(5, v[4]);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
