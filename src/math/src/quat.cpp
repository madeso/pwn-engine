#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/Assert.h>

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
		quat::quat(const real ax, const real ay, const real az, const real aw)
			: x(ax)
			, y(ay)
			, z(az)
			, w(aw)
		{
		}

		quat::quat(const vec3& v, const real aw)
			: x(v.x)
			, y(v.y)
			, z(v.z)
			, w(aw)
		{
		}
		quat::quat(const real aw, const vec3& v)
			: x(v.x)
			, y(v.y)
			, z(v.z)
			, w(aw)
		{
		}

		const quat cquat(const AxisAngle& aa)
		{
			const real s = Sin( aa.angle * 0.5 );

			const quat q(aa.axis * s, Cos( aa.angle * PWN_MATH_VALUE(0.5)));
			return GetNormalized(q);
		}

		void quat::operator*=(const quat& q)
		{
			const real sc = w*q.w - dot(cvec3(*this), cvec3(q));
			const vec3 r = cvec3(q)*w + cvec3(*this)*q.w + cross(cvec3(*this), cvec3(q));

			x = r.x;
			y = r.y;
			z = r.z;
			w = sc;
		}

		void quat::operator+=(const quat& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
		}

		void quat::operator-=(const quat& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
		}

		void quat::operator*=(const real rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
		}

		void quat::operator/=(const real rhs)
		{
			const real s = PWN_MATH_VALUE(1.0) / rhs;
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}

		// ------------------------------------------------------

		const quat quatw(const real aw, const real ax, const real ay, const real az)
		{
			return quat(ax, ay, az, aw);
		}

		real LengthOf(const quat& q)
		{
			return Sqrt( LengthOfSquared(q) );
		}

		real LengthOfSquared(const quat& q)
		{
			return Square(q.x) + Square(q.y) + Square(q.z) + Square(q.w);
		}

		const quat Lerp(const quat& f, const real scale, const quat& t)
		{
			return GetNormalized(f + scale * (t - f));
		}

		const quat Qlerp(const quat& f, const real scale, const quat& t)
		{
			const real sscale = Square(scale);
			return f*(1-sscale) + t*sscale;
		}

		const quat Slerp(const quat& f, const real scale, const quat& t)
		{
			real d = dot(f, t);
			if (d > PWN_MATH_VALUE(0.9995))
			{
				return Lerp(f, scale, t);
			}
			d = KeepWithin(-1, d, 1);
			const Angle theta0 = Acos(d);
			const Angle theta = theta0 * scale;

			const quat q = GetNormalized(t - f * d);
			return f * Cos(theta) + q * Sin(theta);
		}

		// forces the interpolatation to go the "short way"
		const quat SlerpShortway(const quat& f, const real scale, const quat& t)
		{
			if( dot(f, t) < PWN_MATH_VALUE(0.0) ) return Slerp(-f, scale, t);
			else return Slerp(f, scale, t);
		}

		const quat GetNormalized(const quat& q)
		{
			quat temp = q;
			Normalize(&temp);
			return temp;
		}

		void Normalize(quat* q)
		{
			const real l = LengthOf(*q);
			q->x /= l;
			q->y /= l;
			q->z /= l;
			q->w /= l;
		}

		const void Conjugate(quat* q)
		{
			q->x = -q->x;
			q->y = -q->y;
			q->z = -q->z;
			// ignore w
		}
		const quat GetConjugate(const quat& q)
		{
			quat t = q;
			Conjugate(&t);
			return t;
		}

		const vec3 In(const quat& q)
		{
			return RotateAroundOrigo(q, In());
		}

		const vec3 Right(const quat& q)
		{
			return RotateAroundOrigo(q, Right());
		}

		const vec3 Up(const quat& q)
		{
			return RotateAroundOrigo(q, Up());
		}

		const vec3 Out(const quat& q)
		{
			return RotateAroundOrigo(q, Out());
		}

		const vec3 Left(const quat& q)
		{
			return RotateAroundOrigo(q, Left());
		}

		const vec3 Down(const quat& q)
		{
			return RotateAroundOrigo(q, Down());
		}

		const vec3 RightUpIn(const quat& q, const vec3& v)
		{
			return Right(q)*v.x + Up(q)*v.x + In(q)*v.z;
		}

		const vec3 RotateAroundOrigo(const quat& q, const vec3& v)
		{
			const quat r = q * quat(v, PWN_MATH_VALUE(0.0)) * GetConjugate(q);
			return cvec3(r);
		}

		const quat Combine(const quat& current, const quat& extra)
		{
			return GetNormalized(extra * current);
		}

		namespace // local
		{
			bool isZero(real r)
			{
				return ( Abs(r) < PWN_MATH_VALUE(0.006) );
			}
		}

		//quat::quat(const Euler& e);

		const quat qIdentity()
		{
			return quat(PWN_MATH_VALUE(1.0),
				vec3(PWN_MATH_VALUE(0.0), PWN_MATH_VALUE(0.0), PWN_MATH_VALUE(0.0)));
		}

		const quat qLookAt(const vec3& from, const vec3& to, const vec3& up)
		{
			return qLookInDirection(to-from, up);
		}

		const quat qLookAtOrNot(const vec3& from, const vec3& to, const vec3& up)
		{
			const vec3 dir = to-from;
			const real len = LengthOfSquared(dir);
			if( len > 0.001f ) return qLookInDirection(dir, up);
			else return qIdentity();
		}

		// todo: convert to a mat33 constructor
		quat FromMatrix3(const real mat[3][3])
		{
			int   NXT[] = {1, 2, 0};
			real q[4];

			// check the diagonal
			const real tr = mat[0][0] + mat[1][1] + mat[2][2];
			if( tr > PWN_MATH_VALUE(0.0))
			{
				const real s = (real)Sqrt(tr + 1.0f);
				const real t = PWN_MATH_VALUE(0.5) / s;

				return quat(
					(mat[1][2] - mat[2][1]) * t,
					(mat[2][0] - mat[0][2]) * t,
					(mat[0][1] - mat[1][0]) * t,
					s * PWN_MATH_VALUE(0.5));
			}
			else
			{
				// diagonal is negative
				// get biggest diagonal element
				int i = 0;
				if (mat[1][1] > mat[0][0]) i = 1;
				if (mat[2][2] > mat[i][i]) i = 2;
				//setup index sequence
				int j = NXT[i];
				int k = NXT[j];

				real s = Sqrt((mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f);

				q[i] = s * PWN_MATH_VALUE(0.5);

				if (s != PWN_MATH_VALUE(0.0)) s = PWN_MATH_VALUE(0.5) / s;

				q[j] = (mat[i][j] + mat[j][i]) * s;
				q[k] = (mat[i][k] + mat[k][i]) * s;
				q[3] = (mat[j][k] - mat[k][j]) * s;

				return quat(q[0], q[1], q[2], q[3]);
			}
		}

		const quat qLookInDirection(const vec3& adir, const vec3& up)
		{
			Assert(LengthOf(adir) > 0.01f );
			const vec3 dir = GetNormalized(adir);
			const vec3 v = GetNormalized(cross(dir, up));
			const vec3 u = cross(v, dir);
			const vec3 n = -dir;
#define VECARR(v) { v.x, v.y, v.z }
			const real mat[3][3] = { VECARR(v), VECARR(u), VECARR(n) };
#undef VECARR
			return FromMatrix3(mat);
		}

		real dot(const quat& lhs, const quat& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z +lhs.w * rhs.w;
		}

		// -----------------------------------------------------------------------------------------------------------------

		const quat operator*(const quat& lhs, const quat& rhs)
		{
			quat temp = lhs;
			temp *= rhs;
			return temp;
		}

		const quat operator+(const quat& lhs, const quat& rhs)
		{
			quat temp = lhs;
			temp += rhs;
			return temp;
		}

		const quat operator-(const quat& lhs, const quat& rhs)
		{
			quat temp = lhs;
			temp -= rhs;
			return temp;
		}

		const quat operator*(const real scalar, const quat& q)
		{
			quat temp = q;
			temp *= scalar;
			return temp;
		}
		const quat operator*(const quat& q, const real scalar)
		{
			quat temp = q;
			temp *= scalar;
			return temp;
		}

		const quat operator/(const quat& q, const real scalar)
		{
			quat temp = q;
			temp /= scalar;
			return temp;
		}
		const quat operator-(const quat& q)
		{
			return quat(-cvec3(q), -q.w);
		}
	}
}