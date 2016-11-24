#include "gtest/gtest.h"
#include "vector.hpp"

TEST(VectorTest, SizeConstructor) {
  alnesjo::vector<int> v(10);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(0, v[i]);
  }
}

TEST(VectorTest, InitializerListConstructor) {
  alnesjo::vector<char const *> v({"Hej!", "Jag vill", "ingen", "illa."});
  EXPECT_STREQ("Hej!", v[0]);
  EXPECT_STREQ("illa.", v[3]);
}

TEST(VectorTest, CopyConstructor) {
  alnesjo::vector<int> u(10,7), v;
  auto it = u.begin();
  v = u;
  EXPECT_NE(it, v.begin());
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(v[i], u[i]);
  }
  EXPECT_EQ(v.size(), u.size());
  v.fill(0);
  v.clear();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(7, u[i]);
  }
}

TEST(VectorTest, MoveConstructor) {
  alnesjo::vector<int> u(10,7), v;
  auto it = u.begin();
  v = std::move(u);
  EXPECT_EQ(it, v.begin());
}

TEST(VectorTest, PushBack) {
  alnesjo::vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }
  EXPECT_EQ(9, v[9]);
  EXPECT_EQ(4, v[4]);
}

TEST(VectorTest, InsertAtFront) {
  alnesjo::vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.insert(0,i);
  }
  EXPECT_EQ(0, v[9]);
  EXPECT_EQ(5, v[4]);
}

TEST(VectorTest, EraseSingle) {
  alnesjo::vector<int> v({1,2,3,4,5,6});
  v.erase(2);
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(4, v[2]);
  EXPECT_EQ(6, v[4]);
  EXPECT_EQ(5, v.size());
}

TEST(VectorTest, EraseRange) {
  alnesjo::vector<int> v({1,2,3,4,5,6});
  v.erase(v.begin(), v.end()-3);
  EXPECT_EQ(4, v[0]);
  EXPECT_EQ(5, v[1]);
  EXPECT_EQ(6, v[2]);
  EXPECT_EQ(3, v.size());
}

TEST(VectorTest, EraseRemove) {
  alnesjo::vector<int> v({1,2,3,2,5,2});
  v.erase(std::remove(v.begin(), v.end(), 2), v.end());
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(3, v[1]);
  EXPECT_EQ(5, v[2]);
  EXPECT_EQ(3, v.size());
}

TEST(VectorTest, Find) {
  alnesjo::vector<int> v({1,2,3,4,5,6});
  EXPECT_EQ(v.begin()+2, v.find(3));
  EXPECT_EQ(v.begin()+5, v.find(6));
}
