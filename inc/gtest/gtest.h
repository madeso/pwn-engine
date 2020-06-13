#ifndef FAKE_GTEST_H
#define FAKE_GTEST_H

#include "catch/catch.hpp"

#define TEST(class, name) TEST_CASE(#class #name, "[" #class "]")

#define EXPECT_NE(lhs, rhs) REQUIRE(lhs != rhs)
#define EXPECT_EQ(lhs, rhs) REQUIRE(lhs == rhs)
#define EXPECT_TRUE(hs) REQUIRE(hs)
#define EXCEPT_FALSE(hs) REQUIRE_FALSE(hs)

#endif  // FAKE_GTEST_H

