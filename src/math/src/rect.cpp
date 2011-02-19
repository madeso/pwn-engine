#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

#ifdef PWN_USE_CUSTOM_MATH
#include <cmath>
#include <memory> // memcpy
#else
#include <cml/cml.h>
#endif

namespace pwn
{
	namespace math
	{
		rect::rect(const point2& ul, const point2& lr)
			: upperLeft(ul)
			, lowerRight(lr)
		{
			Assert(ul.x() < lr.x());
			Assert(ul.y() > lr.y());
		}

		// -----------------------------------------------------------------------------

		real TopOf(const rect& r)
		{
			return Y(r.upperLeft.vec);
		}

		real BottomOf(const rect& r)
		{
			return Y(r.lowerRight.vec);
		}

		real LeftOf(const rect& r)
		{
			return X(r.upperLeft.vec);
		}

		real RightOf(const rect& r)
		{
			return X(r.lowerRight.vec);
		}

		real WidthOf(const rect& r)
		{
			return RightOf(r) - LeftOf(r);
		}

		real HeightOf(const rect& r)
		{
			return TopOf(r) - BottomOf(r);
		}

		real AspectOf(const rect& r)
		{
			return WidthOf(r) / HeightOf(r);
		}

		const rect FromAspectAndContainingInCenter(const rect& containing, real raspect)
		{
			const real aspect = AspectOf(containing);

			// todo: fix equal test...?
			if( aspect == raspect ) {
				return containing;
			}
			else {
				if( aspect > raspect ) {
					// current width is greater than supported
					// base new on current height
					const real width = HeightOf(containing) * raspect;
					const real theRest = WidthOf(containing) - width;
					const real borderSize = theRest / 2;

					return FromLrud(LeftOf(containing)+borderSize, LeftOf(containing)+borderSize+width, TopOf(containing), BottomOf(containing));
				}
				else {
					// current height is greater than supported
					// base new on current width
					const real height = WidthOf(containing) / raspect;
					const real theRest = HeightOf(containing) - height;
					const real borderSize = theRest / 2;

					return FromLrud(LeftOf(containing), RightOf(containing), TopOf(containing)-borderSize, BottomOf(containing)+borderSize);
				}
			}
		}

		const rect FromUpperLeftAndSize(const point2& point, const direction2& size)
		{
			Assert(X(size.vec) >= 0);
			Assert(Y(size.vec) >= 0);
			return rect(point, point2(point.x() + X(size.vec), point.y()-Y(size.vec)) );
		}

		const rect FromUpperLeftAndSize(const point2& point, const real width, const real height)
		{
			return FromUpperLeftAndSize(point, direction2(width, height) );
		}

		const rect FromUpperLeftAndSize(const point2& point, const real size)
		{
			return FromUpperLeftAndSize(point, direction2(size, size) );
		}

		const rect FromUpperLeftAndLowerRight(const point2& ul, const point2& lr)
		{
			return rect(ul, lr);
		}

		const rect FromSizeAndCenter(const direction2& size, const point2& center)
		{
			const real w = X(size.vec);
			const real h = Y(size.vec);
			const real cx = X(center.vec);
			const real cy = 1-Y(center.vec);
			return FromUpperLeftAndLowerRight(point2(-w*cx, h*cy), point2(w*(1-cx), -h*(1-cy)) );
		}

		const rect FromLrud(const real left, const real right, const real up, const real down)
		{
			return FromUpperLeftAndLowerRight(point2(left, up), point2(right, down) );
		}

		const rect FromSizeAndCenter(const real size, const point2& center)
		{
			return FromSizeAndCenter(direction2(size, size), center);
		}


		const rect MoveCopyTo(const rect& r, const point2& point)
		{
			const point2 ul = point2( r.upperLeft.vec + point.vec );
			const point2 lr = point2( r.lowerRight.vec + point.vec );
			return rect(ul, lr);
		}

		const rect TurnCopyUpsideDown(const rect& r, const real scale)
		{
			const real lower = Y(r.upperLeft.vec) * scale;
			const real upper = Y(r.lowerRight.vec) * scale;
			return FromUpperLeftAndLowerRight(ChangeY(r.upperLeft, upper), ChangeY(r.lowerRight, lower));
		}

		const point2 Remap(const rect& from, const point2& p, const rect& to)
		{
			return point2( Remap(LeftOf(from), RightOf(from), X(p.vec), LeftOf(to), RightOf(to)) ,
				Remap(TopOf(from), BottomOf(from), Y(p.vec), TopOf(to), BottomOf(to)) );
		}

		const point2 KeepWithin(const point2& loc, const rect& region)
		{
			return point2( KeepWithin(LeftOf(region), X(loc.vec), RightOf(region)), KeepWithin(BottomOf(region), Y(loc.vec), TopOf(region)));
		}

		const point2 KeepWithin(const point2& loc, const rect& region, const rect& object)
		{
			return point2( KeepWithin(LeftOf(region)-LeftOf(object), X(loc.vec), RightOf(region)-RightOf(object)), KeepWithin(BottomOf(region)-BottomOf(object), Y(loc.vec), TopOf(region)-TopOf(object)));
		}

		const bool IsWithin(const point2& loc, const rect& region)
		{
			return IsWithin(LeftOf(region), X(loc.vec), RightOf(region))
				&& IsWithin(BottomOf(region), Y(loc.vec), TopOf(region));
		}

		// ------------------------------------------------------------------------

		const rect operator+(const rect& lhs, const direction2& rhs)
		{
			return rect(lhs.upperLeft+rhs, lhs.lowerRight+rhs);
		}

		const rect operator+(const direction2& lhs, const rect& rhs)
		{
			return rect(rhs.upperLeft+lhs, rhs.lowerRight+lhs);
		}

		const rect operator*(const rect& r, const real scale)
		{
			return rect(r.upperLeft*scale, r.lowerRight*scale);
		}

		const rect operator*(const real scale, const rect& r)
		{
			return rect(r.upperLeft*scale, r.lowerRight*scale);
		}
	}
}
