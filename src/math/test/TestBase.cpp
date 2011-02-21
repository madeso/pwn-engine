#include "TestBase.h"

namespace // local
{
	using namespace pwn;
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
			return equal(math::X(d),math::X(rhs)) && equal(math::Y(d), math::Y(rhs));
		}

		std::ostream& operator<<(std::ostream& o, const vec2& t)
		{
			o << "(" << math::X(t) << ", " << math::Y(t) << ")";
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
			return equal(math::X(d), math::X(rhs)) && equal(math::Y(d), math::Y(rhs)) && equal(math::Z(d), math::Z(rhs));
		}

		std::ostream& operator<<(std::ostream& o, const vec3& t)
		{
			o << "(" << math::X(t) << ", " << math::Y(t) << ", " << math::Z(t) << ")";
			return o;
		}

		bool operator==(const quat& lhs, const quat& rhs)
		{
			return equal(math::X(lhs), math::X(rhs)) && equal(math::Y(lhs), math::Y(rhs)) && equal(math::Z(lhs), math::Z(rhs)) && equal(math::W(lhs), math::W(rhs));
		}

		std::ostream& operator<<(std::ostream& o, const quat& t)
		{
			o << "(" << math::X(t) << ", " << math::Y(t) << ", " << math::Z(t)  << ", " << math::W(t) << ")";
			return o;
		}

		bool operator==(const mat33& lhs, const mat33& rhs)
		{
			for(int i=0; i<3*3; ++i ) if( !equal(lhs.data()[i], rhs.data()[i]) ) return false;
			return true;
		}
		std::ostream& operator<<(std::ostream& o, const mat33& m)
		{
			o << "( ";
			for(int i=0; i<3*3; ++i ) o << m.data()[i] << " ";
			o << ")";
			return o;
		}

		bool operator==(const mat44& lhs, const mat44& rhs)
		{
			for(int i=0; i<4*4; ++i ) if( !equal(lhs.data()[i], rhs.data()[i]) ) return false;
			return true;
		}

		std::ostream& operator<<(std::ostream& o, const mat44& m)
		{
			o << "( ";
			for(int i=0; i<4*4; ++i ) o << m.data()[i] << " ";
			o << ")";
			return o;
		}

		bool operator==(const rect& lhs, const rect& rhs)
		{
			return lhs.upperLeft == rhs.upperLeft && lhs.lowerRight == rhs.lowerRight;
		}

		std::ostream& operator<<(std::ostream& o, const rect& t)
		{
			o << "(" << t.upperLeft.x() << ", " << t.upperLeft.y() << ", " << t.lowerRight.x()  << ", " << t.lowerRight.y() << ")";
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
			o << "(" << math::X(aa.axis) << ", " << math::Y(aa.axis) << ", " << math::Z(aa.axis) << ": " << aa.angle.inDegrees() << ")";
			return o;
		}
	}
}
