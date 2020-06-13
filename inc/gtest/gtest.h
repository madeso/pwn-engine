#ifndef FAKE_GTEST_H
#define FAKE_GTEST_H

#include "catch/catch.hpp"

#define TEST(class, name) TEST_CASE(#class #name, "[" #class "]")

#define EXPECT_NE(lhs, rhs) REQUIRE(lhs != rhs)
#define EXPECT_EQ(lhs, rhs) REQUIRE(lhs == rhs)
#define EXPECT_TRUE(hs) REQUIRE(hs)
#define EXPECT_FALSE(hs) REQUIRE_FALSE(hs)

#define EXPECT_FLOAT_EQ(lhs, rhs) REQUIRE(lhs == Approx(rhs))

#define EXPECT_PRED_FORMAT2(f, lhs, rhs) REQUIRE(f(#lhs, #rhs, lhs, rhs))

#endif  // FAKE_GTEST_H
