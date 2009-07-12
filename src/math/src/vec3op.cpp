#include <sml/vec3d>
#include <sml/vec3op>
#include <sml/math1>
#include <sml/angleop>
#include <sml/angled>

namespace sml
{
	vec3::vec3(const real ax, const real ay, const real az)
		: x(ax)
		, y(ay)
		, z(az)
	{
	}

	real* vec3::data()
	{
		return &x;
	}

	const real* vec3::data() const
	{
		return &x;
	}

	real& vec3::operator[](int index)
	{
		if( index == 0 ) return x;
		else if( index == 1 ) return y;
		else return z;
	}

	const real vec3::operator[](int index) const
	{
		if( index == 0 ) return x;
		else if( index == 1 ) return y;
		else return z;
	}

	void vec3::operator+=(const vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}

	void vec3::operator-=(const vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}

	void vec3::operator*=(const real rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
	}

	void vec3::operator/=(const real rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
	}

	// -------------------------------------------------------

	real LengthOf(const vec3& vec)
	{
		return Sqrt(LengthOfSquared(vec));
	}

	real LengthOfSquared(const vec3& vec)
	{
		return Square(vec.x) + Square(vec.y) + Square(vec.z);
	}

	const vec3 GetNormalized(const vec3& vec)
	{
		vec3 temp = vec;
		Normalize(&temp);
		return temp;
	}

	void Normalize(vec3* vec)
	{
		*vec /= LengthOf(*vec);
	}

	const vec3 operator+(const vec3& lhs, const vec3& rhs)
	{
		vec3 temp = lhs;
		temp += rhs;
		return temp;
	}

	const vec3 operator-(const vec3& lhs, const vec3& rhs)
	{
		vec3 temp = lhs;
		temp -= rhs;
		return temp;
	}

	const vec3 operator*(const real scalar, const vec3& vec)
	{
		vec3 temp = vec;
		temp *= scalar;
		return temp;
	}
	const vec3 operator*(const vec3& vec, const real scalar)
	{
		vec3 temp = vec;
		temp *= scalar;
		return temp;
	}

	const vec3 operator/(const vec3& vec, const real scalar)
	{
		vec3 temp = vec;
		temp /= scalar;
		return temp;
	}
	const vec3 operator-(const vec3& vec)
	{
		return vec3(-vec.x, -vec.y, -vec.z);
	}

	const vec3 cross(const vec3& lhs, const vec3& rhs)
	{
		const vec3& A = lhs;
		const vec3& B = rhs;
		return vec3(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x);
	}

	const real dot(const vec3& lhs, const vec3& rhs)
	{
		const vec3& A = lhs;
		const vec3& B = rhs;
		return A.x * B.x + A.y * B.y + A.z * B.z;
	}

	// -----------------------------------------------------------------------------------

	const Angle AngleBetween(const vec3& a, const vec3& b)
	{
		return Acos( dot(GetNormalized(a), b) );
	}
	const Angle AngleBetween(const vec3& a, const vec3& b, const vec3& perpa)
	{
		const Angle s = AngleBetween(a,b);
		const Angle t = AngleBetween(perpa,b);
		if ( t.inDegrees() > 90 ) return Angle::FromDegrees(180)+s;
		else return s;
	}

	
	const vec3 Right()
	{
		return vec3(1,0,0);
	}
	const vec3 In()
	{
		return vec3(0,0,-1);
	}
	const vec3 Up()
	{
		return vec3(0,1,0);
	}

	const vec3 Left()
	{
		return -Right();
	}
	const vec3 Out()
	{
		return -In();
	}
	const vec3 Down()
	{
		return -Up();
	}
}