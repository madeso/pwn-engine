#include "TestBase.h"

namespace // local
{
	using namespace pwn::math;
	bool equal(real lhs, real rhs)
	{
		return Abs(lhs-rhs)< 0.0001;
	}
}

namespace pwn
{
	namespace math
	{
		bool operator==(const vec2& d, const vec2& rhs)
		{
			return equal(d.x,rhs.x) && equal(d.y, rhs.y);
		}

		std::ostream& operator<<(std::ostream& o, const vec2& t)
		{
			o << "(" << t.x << ", " << t.y << ")";
			return o;
		}

		bool operator==(const point2& d, const point2& rhs)
		{
			return equal(d.x(), rhs.x()) && equal(d.y(), rhs.y());
		}

		std::ostream& operator<<(std::ostream& o, const point2& t)
		{
			o << "(" << t.x() << ", " << t.y() << ")";
			return o;
		}

		bool operator==(const vec3& d, const vec3& rhs)
		{
			return equal(d.x, rhs.x) && equal(d.y, rhs.y) && equal(d.z, rhs.z);
		}

		std::ostream& operator<<(std::ostream& o, const vec3& t)
		{
			o << "(" << t.x << ", " << t.y << ", " << t.z << ")";
			return o;
		}

		bool operator==(const quat& lhs, const quat& rhs)
		{
			return equal(lhs.x, rhs.x) && equal(lhs.y, rhs.y) && equal(lhs.z, rhs.z) && equal(lhs.w, rhs.w);
		}

		std::ostream& operator<<(std::ostream& o, const quat& t)
		{
			o << "(" << t.x << ", " << t.y << ", " << t.z  << ", " << t.w << ")";
			return o;
		}

		bool operator==(const AxisAngle& lhs, const AxisAngle& rhs)
		{
			if( equal(lhs.angle.inDegrees(), rhs.angle.inDegrees()) && equal(lhs.angle.inDegrees(), 0)) return true; // zero rotation is always equal zero

			const bool a = (rhs.axis == lhs.axis && equal(rhs.angle.inDegrees(), lhs.angle.inDegrees() ));
			const bool inv = (rhs.axis == -lhs.axis && equal(rhs.angle.inDegrees(), -lhs.angle.inDegrees() ));
			return a || inv;
		}

		std::ostream& operator<<(std::ostream& o, const AxisAngle& aa)
		{
			o << "(" << aa.axis.x << ", " << aa.axis.y << ", " << aa.axis.z << ": " << aa.angle.inDegrees() << ")";
			return o;
		}
	}
}