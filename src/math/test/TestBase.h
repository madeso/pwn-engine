// include this first in each test

#include <iostream>
#include <pwn/math/types.h>
#include <pwn/math/operations.h>

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

		bool operator==(const mat33& lhs, const mat33& rhs);
		std::ostream& operator<<(std::ostream& o, const mat33& m);

		bool operator==(const mat44& lhs, const mat44& rhs);
		std::ostream& operator<<(std::ostream& o, const mat44& m);

		bool operator==(const rect& lhs, const rect& rhs);
		std::ostream& operator<<(std::ostream& o, const rect& aa);

		bool operator==(const AxisAngle& lhs, const AxisAngle& rhs);
		std::ostream& operator<<(std::ostream& o, const AxisAngle& aa);
	}
}

#include <UnitTest++.h>