#include <sml/rect>

#include <sml/pointdir2>
#include <sml/math1>
#include <sml/vec2op>

namespace sml
{
	rect::rect(const point2& ul, const point2& lr)
		: upperLeft(ul)
		, lowerRight(lr)
	{
	}

	const vec2 ChangeY(const vec2& v, const real newy)
	{
		return vec2(v.x, newy);
	}
	const point2 ChangeY(const point2& v, const real newy)
	{
		return point2(v.vec.x, newy);
	}

	const point2 Remap(const rect& from, const point2& p, const rect& to)
	{
		return point2( Remap(LeftOf(from), RightOf(from), p.vec.x, LeftOf(to), RightOf(to)) ,
		               Remap(TopOf(from), BottomOf(from), p.vec.y, TopOf(to), BottomOf(to)) );
	}

	real TopOf(const rect& r)
	{
		return r.upperLeft.vec.y;
	}
	real BottomOf(const rect& r)
	{
		return r.lowerRight.vec.y;
	}
	real LeftOf(const rect& r)
	{
		return r.upperLeft.vec.x;
	}
	real RightOf(const rect& r)
	{
		return r.lowerRight.vec.x;
	}

	const rect FromUpperLeftAndSize(const point2& point, const direction2& size)
	{
		return rect(point, point2(point.x() + size.vec.x, point.y()-size.vec.y) );
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
		const float w = size.vec.x;
		const float h = size.vec.y;
		const float cx = center.vec.x;
		const float cy = center.vec.y;
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
		const real lower = r.upperLeft.vec.y * scale;
		const real upper = r.lowerRight.vec.y * scale;
		return FromUpperLeftAndLowerRight(ChangeY(r.upperLeft, upper), ChangeY(r.lowerRight, lower));
	}
	
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

	const point2 Within(const point2& loc, const rect& region)
	{
		return point2( Within(LeftOf(region), loc.vec.x, RightOf(region)), Within(BottomOf(region), loc.vec.y, TopOf(region)));
	}
	const point2 Within(const point2& loc, const rect& region, const rect& object)
	{
		return point2( Within(LeftOf(region)-LeftOf(object), loc.vec.x, RightOf(region)-RightOf(object)), Within(BottomOf(region)-BottomOf(object), loc.vec.y, TopOf(region)-TopOf(object)));
	}
	const bool IsWithin(const point2& loc, const rect& region)
	{
		return IsWithin(LeftOf(region), loc.vec.x, RightOf(region))
		    && IsWithin(BottomOf(region), loc.vec.y, TopOf(region));
	}
}