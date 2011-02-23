#include "TestBase.h"
#include <boost/utility.hpp>

#define GTEST(x) TEST(Rect, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	struct wh  : ::testing::Test, boost::noncopyable
	{
		const real w;
		const real h;

		wh()
			: w(1)
			, h(2)
		{
		}
	};

	TEST_F(wh, TestUpperLeftAndSize)
	{
		rect a = FromUpperLeftAndSize(Origo2(), direction2(w,h));
		EXPECT_REAL_EQ(0, LeftOf(a));
		EXPECT_REAL_EQ(w, RightOf(a));
		EXPECT_REAL_EQ(0, TopOf(a));
		EXPECT_REAL_EQ(-h, BottomOf(a));
	}

	TEST_F(wh, TestSizeAndCenter1)
	{
		rect c = FromSizeAndCenter(direction2(w,h), point2(0.5f, 0.5f));
		EXPECT_REAL_EQ(-w/2, LeftOf(c));
		EXPECT_REAL_EQ(w/2, RightOf(c));
		EXPECT_REAL_EQ(h/2, TopOf(c));
		EXPECT_REAL_EQ(-h/2, BottomOf(c));
	}

	TEST_F(wh, TestSizeAndCenter2)
	{
		rect c = FromSizeAndCenter(direction2(w,h), point2(0.0f, 0.0f));
		EXPECT_REAL_EQ(0, LeftOf(c));
		EXPECT_REAL_EQ(w, RightOf(c));
		EXPECT_REAL_EQ(h, TopOf(c));
		EXPECT_REAL_EQ(0, BottomOf(c));
	}

	TEST_F(wh, TestSizeAndCenter3)
	{
		rect c = FromSizeAndCenter(direction2(w,h), point2(1.0f, 1.0f));
		EXPECT_REAL_EQ(-w, LeftOf(c));
		EXPECT_REAL_EQ(0, RightOf(c));
		EXPECT_REAL_EQ(0, TopOf(c));
		EXPECT_REAL_EQ(-h, BottomOf(c));
	}

	TEST_F(wh, TestWidth)
	{
		EXPECT_REAL_EQ(w, WidthOf(FromLrud(-w, 0, 1, 0)) );
		EXPECT_REAL_EQ(w, WidthOf(FromLrud(0, w, 1, 0) ) );
		EXPECT_REAL_EQ(w*2, WidthOf(FromLrud(-w, w, 1, 0)) );
	}

	TEST_F(wh, TestHeight)
	{
		EXPECT_REAL_EQ(h, HeightOf(FromLrud(0, 1, h, 0)) );
		EXPECT_REAL_EQ(h, HeightOf(FromLrud(0, 1, 0, -h)) );
		EXPECT_REAL_EQ(h*2, HeightOf(FromLrud(0, 1, h, -h)) );
	}

	GTEST(TestAspectOf)
	{
		EXPECT_REAL_EQ(1, AspectOf(FromLrud(0, 1, 1, 0)) );
		EXPECT_REAL_EQ(2, AspectOf(FromLrud(0, 2, 1, 0)) );
		EXPECT_REAL_EQ(0.5f, AspectOf(FromLrud(0, 1, 2, 0)) );
	}

	GTEST(TestAspectCreatorFunction_basic)
	{
#define TEST_RECT(l, r, u, d) EXPECT_PRED2(::pwn::math::rect_equal, FromLrud(l, r, u, d), FromAspectAndContainingInCenter(FromLrud(l, r, u, d), 1) )
		TEST_RECT(0, 2, 2, 0);
		TEST_RECT(1, 3, 3, 1);
		TEST_RECT(-1, 3, 5, 1);
#undef TEST_RECT
	}

	GTEST(TestAspectCreatorFunction_CenterOnWidth)
	{
		EXPECT_PRED2(::pwn::math::rect_equal, FromLrud(5, 6, 1, 0), FromAspectAndContainingInCenter(FromLrud(0, 11, 1, 0), 1) );
		EXPECT_PRED2(::pwn::math::rect_equal, FromLrud(0, 1, 1, 0), FromAspectAndContainingInCenter(FromLrud(-5, 6, 1, 0), 1) );
	}

	GTEST(TestAspectCreatorFunction_CenterOnHeight)
	{
		EXPECT_PRED2(::pwn::math::rect_equal, FromLrud(0, 1, 6, 5), FromAspectAndContainingInCenter(FromLrud(0, 1, 11, 0), 1) );
		EXPECT_PRED2(::pwn::math::rect_equal, FromLrud(0, 1, 1, 0), FromAspectAndContainingInCenter(FromLrud(0, 1, 6, -5), 1) );
	}

	GTEST(tesRectKeepWithinAndBounds)
	{
		// todo: take another look at what I'm doing here???
		EXPECT_PRED2(::pwn::math::point2_equal, point2(1, 0), KeepWithin(point2(0,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(1, 0), KeepWithin(point2(-5,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(5, 0), KeepWithin(point2(5,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(9, 0), KeepWithin(point2(10,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(9, 0), KeepWithin(point2(12,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );

		EXPECT_PRED2(::pwn::math::point2_equal, point2(5, 4), KeepWithin(point2(5,5),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(5, 4), KeepWithin(point2(5,8),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(5, -4), KeepWithin(point2(5,-5),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		EXPECT_PRED2(::pwn::math::point2_equal, point2(5, -4), KeepWithin(point2(5,-9),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
	}
}
