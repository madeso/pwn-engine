#include "TestBase.h"

SUITE(testRect)
{
	using namespace pwn::math;
	TEST(TextureRectTest)
	{
		const float w = 1;
		const float h = 2;
		rect a = FromUpperLeftAndSize(Origo2(), direction2(w,h));
		CHECK_EQUAL(0, LeftOf(a));
		CHECK_EQUAL(w, RightOf(a));
		CHECK_EQUAL(0, TopOf(a));
		CHECK_EQUAL(-h, BottomOf(a));

		rect c = FromSizeAndCenter(direction2(w,h), point2(0.5f, 0.5f));
		CHECK_EQUAL(-w/2, LeftOf(c));
		CHECK_EQUAL(w/2, RightOf(c));
		CHECK_EQUAL(h/2, TopOf(c));
		CHECK_EQUAL(-h/2, BottomOf(c));
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