#include "gtest/gtest.h"
#include "vector.hpp"

TEST(VectorTest, PushBack) {
  alnesjo::Vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }
  EXPECT_EQ(9, v[9]);
  EXPECT_EQ(4, v[4]);
}

TEST(VectorTest, SizeConstructor) {
  alnesjo::Vector<int> v(10);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(0, v[i]);
  }
}

TEST(VectorTest, CopyConstructor) {
  alnesjo::Vector<int> u(10,7), v;
  auto it = u.begin();
  v = u;
  EXPECT_NE(it, v.begin());
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(v[i], u[i]);
  }
  EXPECT_EQ(v.size(), u.size());
  v.reset();
  v.clear();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(7, u[i]);
  }
}

TEST(VectorTest, MoveConstructor) {
  alnesjo::Vector<int> u(10,7), v;
  auto it = u.begin();
  v = std::move(u);
  EXPECT_EQ(it, v.begin());
}

TEST(VectorTest, InitializerListConstructor) {
  alnesjo::Vector<char const *> v({"Hej!", "Jag vill", "ingen", "illa."});
  EXPECT_STREQ("Hej!", v[0]);
  EXPECT_STREQ("illa.", v[3]);
}


TEST(VectorTest, InsertAtFront) {
  alnesjo::Vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.insert(0,i);
  }
  EXPECT_EQ(0, v[9]);
  EXPECT_EQ(5, v[4]);
}

TEST(VectorTest, Clear) {
  alnesjo::Vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.insert(0,i);
  }
  EXPECT_EQ(0, v[9]);
  EXPECT_EQ(5, v[4]);
}
