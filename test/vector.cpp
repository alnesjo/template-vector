#include "gtest/gtest.h"
#include "vector.hpp"

#define EXPECT_ERROR(expr) {                    \
    bool error = false;                         \
    try {                                       \
      expr;                                     \
    } catch (std::exception e) {                \
      error = true;                             \
    }                                           \
    EXPECT_TRUE(error);                         \
  }                                             \

TEST(Constructor, InitializerList) {
  alnesjo::vector<int> u, v({1,2,3,4,5,6});
  u.push_back(1);
  u.push_back(2);
  u.push_back(3);
  u.push_back(4);
  u.push_back(5);
  u.push_back(6);
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
}

TEST(Constructor, EmptyInitializerList) {
  alnesjo::vector<int> u, v({});
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
}

TEST(Constructor, PositiveSize) {
  alnesjo::vector<int> u({0,0,0,0,0,0}), v(6);
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
}

TEST(Constructor, ZeroSize) {
  alnesjo::vector<int> u, v(0);
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
}

TEST(Constructor, NegativeSize) {
  EXPECT_ERROR(alnesjo::vector<int> u(-6));
}

TEST(Constructor, Copy) {
  alnesjo::vector<int> u(10,7), v;
  v = u;
  EXPECT_NE(u.begin(), v.begin());
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
  v.fill(0);
  v.clear();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(7, u[i]);
  }
}

TEST(Constructor, Move) {
  alnesjo::vector<int> u(10,7), v;
  auto it = u.begin();
  v = std::move(u);
  EXPECT_EQ(it, v.begin());
}

TEST(PushBack, Natural) {
  alnesjo::vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }
  EXPECT_EQ(9, v[9]);
  EXPECT_EQ(4, v[4]);
}

TEST(EmplaceBack, Natural) {
  alnesjo::vector<std::tuple<int,int,int>> u({std::make_tuple(1,2,3),
                                              std::make_tuple(4,5,6)}), v;
  v.emplace_back(1,2,3);
  v.emplace_back(4,5,6);
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
}


TEST(Insert, AtFront) {
  alnesjo::vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.insert(0,i);
  }
  EXPECT_EQ(0, v[9]);
  EXPECT_EQ(5, v[4]);
}

TEST(Emplace, AtFront) {
  alnesjo::vector<std::tuple<int,int>> u({std::make_tuple(5,6),
                                          std::make_tuple(3,4),
                                          std::make_tuple(1,2)}), v;
  v.emplace(0,1,2);
  v.emplace(0,3,4);
  v.emplace(0,5,6);
  EXPECT_TRUE(std::equal(u.begin(), u.end(), v.begin()));
}


TEST(Erase, Single) {
  alnesjo::vector<int> v({1,2,3,4,5,6});
  v.erase(2);
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(4, v[2]);
  EXPECT_EQ(6, v[4]);
  EXPECT_EQ(5, v.size());
}

TEST(Erase, Range) {
  alnesjo::vector<int> v({1,2,3,4,5,6});
  v.erase(v.begin(), v.end()-3);
  EXPECT_EQ(4, v[0]);
  EXPECT_EQ(5, v[1]);
  EXPECT_EQ(6, v[2]);
  EXPECT_EQ(3, v.size());
}

TEST(Erase, Remove) {
  alnesjo::vector<int> v({1,2,3,2,5,2});
  v.erase(std::remove(v.begin(), v.end(), 2), v.end());
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(3, v[1]);
  EXPECT_EQ(5, v[2]);
  EXPECT_EQ(3, v.size());
}

TEST(Find, Natural) {
  alnesjo::vector<int> v({1,2,3,4,5,6});
  EXPECT_EQ(v.begin()+2, v.find(3));
  EXPECT_EQ(v.begin()+5, v.find(6));
}
