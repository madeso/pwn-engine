#include "TestBase.h"
#include <boost/utility.hpp>

SUITE(testRect)
{
	using namespace pwn;
	using namespace pwn::math;

	struct wh : boost::noncopyable
	{
		const real w;
		const real h;

		wh()
			: w(1)
			, h(2)
		{
		}
	};

	TEST_FIXTURE(wh, TestUpperLeftAndSize)
	{
		rect a = FromUpperLeftAndSize(Origo2(), direction2(w,h));
		CHECK_EQUAL(0, LeftOf(a));
		CHECK_EQUAL(w, RightOf(a));
		CHECK_EQUAL(0, TopOf(a));
		CHECK_EQUAL(-h, BottomOf(a));
	}

	TEST_FIXTURE(wh, TestSizeAndCenter1)
	{
		rect c = FromSizeAndCenter(direction2(w,h), point2(0.5f, 0.5f));
		CHECK_EQUAL(-w/2, LeftOf(c));
		CHECK_EQUAL(w/2, RightOf(c));
		CHECK_EQUAL(h/2, TopOf(c));
		CHECK_EQUAL(-h/2, BottomOf(c));
	}

	TEST_FIXTURE(wh, TestSizeAndCenter2)
	{
		rect c = FromSizeAndCenter(direction2(w,h), point2(0.0f, 0.0f));
		CHECK_EQUAL(0, LeftOf(c));
		CHECK_EQUAL(w, RightOf(c));
		CHECK_EQUAL(h, TopOf(c));
		CHECK_EQUAL(0, BottomOf(c));
	}

	TEST_FIXTURE(wh, TestSizeAndCenter3)
	{
		rect c = FromSizeAndCenter(direction2(w,h), point2(1.0f, 1.0f));
		CHECK_EQUAL(-w, LeftOf(c));
		CHECK_EQUAL(0, RightOf(c));
		CHECK_EQUAL(0, TopOf(c));
		CHECK_EQUAL(-h, BottomOf(c));
	}

	TEST_FIXTURE(wh, TestWidth)
	{
		CHECK_EQUAL(w, WidthOf(FromLrud(-w, 0, 1, 0)) );
		CHECK_EQUAL(w, WidthOf(FromLrud(0, w, 1, 0) ) );
		CHECK_EQUAL(w*2, WidthOf(FromLrud(-w, w, 1, 0)) );
	}

	TEST_FIXTURE(wh, TestHeight)
	{
		CHECK_EQUAL(h, HeightOf(FromLrud(0, 1, h, 0)) );
		CHECK_EQUAL(h, HeightOf(FromLrud(0, 1, 0, -h)) );
		CHECK_EQUAL(h*2, HeightOf(FromLrud(0, 1, h, -h)) );
	}

	TEST(TestAspectOf)
	{
		CHECK_EQUAL(1, AspectOf(FromLrud(0, 1, 1, 0)) );
		CHECK_EQUAL(2, AspectOf(FromLrud(0, 2, 1, 0)) );
		CHECK_EQUAL(0.5f, AspectOf(FromLrud(0, 1, 2, 0)) );
	}

	TEST(TestAspectCreatorFunction_basic)
	{
#define TEST_RECT(l, r, u, d) CHECK_EQUAL( FromLrud(l, r, u, d), FromAspectAndContainingInCenter(FromLrud(l, r, u, d), 1) )
		TEST_RECT(0, 2, 2, 0);
		TEST_RECT(1, 3, 3, 1);
		TEST_RECT(-1, 3, 5, 1);
#undef TEST_RECT
	}

	TEST(TestAspectCreatorFunction_CenterOnWidth)
	{
		CHECK_EQUAL( FromLrud(5, 6, 1, 0), FromAspectAndContainingInCenter(FromLrud(0, 11, 1, 0), 1) );
		CHECK_EQUAL( FromLrud(0, 1, 1, 0), FromAspectAndContainingInCenter(FromLrud(-5, 6, 1, 0), 1) );
	}

	TEST(TestAspectCreatorFunction_CenterOnHeight)
	{
		CHECK_EQUAL( FromLrud(0, 1, 6, 5), FromAspectAndContainingInCenter(FromLrud(0, 1, 11, 0), 1) );
		CHECK_EQUAL( FromLrud(0, 1, 1, 0), FromAspectAndContainingInCenter(FromLrud(0, 1, 6, -5), 1) );
	}

	TEST(tesRectKeepWithinAndBounds)
	{
		// todo: take another look at what I'm doing here???
		CHECK_EQUAL( point2(1, 0), KeepWithin(point2(0,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(1, 0), KeepWithin(point2(-5,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(5, 0), KeepWithin(point2(5,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(9, 0), KeepWithin(point2(10,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(9, 0), KeepWithin(point2(12,0),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );

		CHECK_EQUAL( point2(5, 4), KeepWithin(point2(5,5),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(5, 4), KeepWithin(point2(5,8),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(5, -4), KeepWithin(point2(5,-5),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
		CHECK_EQUAL( point2(5, -4), KeepWithin(point2(5,-9),FromLrud(0, 10, 5, -5), FromSizeAndCenter(2, point2(0.5, 0.5))) );
	}
}