// include this first in each test

#include <iostream>
#include <pwn/math/types>
#include <pwn/math/functions>

namespace pwn
{
	namespace math
	{
		bool operator==(const vec2& d, const vec2& rhs);
		std::ostream& operator<<(std::ostream& o, const vec2& t);

		bool operator==(const point2& d, const point2& rhs);
		std::ostream& operator<<(std::ostream& o, const point2& t);

		bool operator==(const vec3& d, const vec3& rhs);
		std::ostream& operator<<(std::ostream& o, const vec3& t);

		bool operator==(const quat& lhs, const quat& rhs);
		std::ostream& operator<<(std::ostream& o, const quat& t);

		bool operator==(const AxisAngle& lhs, const AxisAngle& rhs);
		std::ostream& operator<<(std::ostream& o, const AxisAngle& aa);
	}
}

#include <UnitTest++.h>