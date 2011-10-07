#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>


namespace pwn
{
	namespace math
	{
		real X(const quat& q) { return q[0]; }
		real Y(const quat& q) { return q[1]; }
		real Z(const quat& q) { return q[2]; }
		real W(const quat& q) { return q[3]; }
		real& X(quat& q) { return q[0]; }
		real& Y(quat& q) { return q[1]; }
		real& Z(quat& q) { return q[2]; }
		real& W(quat& q) { return q[3]; }

		const quat cquat(const real ax, const real ay, const real az)
		{
			quat q;
			cml::quaternion_rotation_euler(q, ax, ay, az, cml::euler_order_xyz);
			return q;
		}

		const quat cquat(const AxisAngle& aa)
		{
			quat q;
			cml::quaternion_rotation_axis_angle(q, aa.axis, aa.angle.inRadians());
			return q;
		}

		// ------------------------------------------------------

		const quat quatw(const real aw, const real ax, const real ay, const real az)
		{
			return quat(ax, ay, az, aw);
		}

		const quat Lerp(const quat& f, const real scale, const quat& t)
		{
			return cml::lerp(f, t, scale);
		}

		const quat Qlerp(const quat& f, const real scale, const quat& t)
		{
			const real sscale = Square(scale);
			return f*(1-sscale) + t*sscale;
		}

		const quat Slerp(const quat& f, const real scale, const quat& t)
		{
			return cml::slerp(f, t, scale);
		}

		// forces the interpolatation to go the "short way"
		const quat SlerpShortway(const quat& f, const real scale, const quat& t)
		{
			if( dot(f, t) < PWN_MATH_VALUE(0.0) ) return Slerp(-f, scale, t);
			else return Slerp(f, scale, t);
		}

		const quat GetNormalized(const quat& q)
		{
			if( q.length_squared() < 0.001f ) return q;
			quat temp = q;
			temp.normalize();
			return temp;
		}

		const quat GetConjugate(const quat& q)
		{
			quat t = q;
			return t.conjugate();
		}

		const vec3 RotateAroundOrigoNorm(const quat& q, const vec3& v)
		{
			return GetNormalized(RotateAroundOrigo(q, v));
		}

		const vec3 In(const quat& q)
		{
			return RotateAroundOrigoNorm(q, In());
		}

		const vec3 Right(const quat& q)
		{
			return RotateAroundOrigoNorm(q, Right());
		}

		const vec3 Up(const quat& q)
		{
			return RotateAroundOrigoNorm(q, Up());
		}

		const vec3 Out(const quat& q)
		{
			return RotateAroundOrigoNorm(q, Out());
		}

		const vec3 Left(const quat& q)
		{
			return RotateAroundOrigoNorm(q, Left());
		}

		const vec3 Down(const quat& q)
		{
			return RotateAroundOrigoNorm(q, Down());
		}

		const vec3 RightUpIn(const quat& q, const vec3& v)
		{
			return Right(q)*X(v) + Up(q)*Y(v) + In(q)*Z(v);
		}

		const vec3 RotateAroundOrigo(const quat& q, const vec3& v)
		{
			const quat r = q * quat(v, PWN_MATH_VALUE(0.0)) * math::GetConjugate(q);
			return cvec3(r);
		}

		const quat Combine_Local(const quat& current, const quat& extra)
		{
			return GetNormalized(current * extra);
		}

		const quat Combine_Parent(const quat& current, const quat& extra)
		{
			return GetNormalized(extra * current);
		}

		const quat Combine(const quat& current, const quat& extra)
		{
			return Combine_Parent(current, extra);
		}

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
			const real len = dir.length_squared();
			if( len > 0.001f ) return qLookInDirection(dir, up);
			else return qIdentity();
		}

		const quat qLookInDirection(const vec3& adir, const vec3& up)
		{
			quat ret;
			mat44 mat;
			cml::matrix_look_at_LH(mat, vec3(0,0,0), 20*adir, up);
			cml::quaternion_rotation_matrix(ret,mat);
			return ret;
		}
	}
}
