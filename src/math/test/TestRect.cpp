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