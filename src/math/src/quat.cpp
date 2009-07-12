#include <sml/quat>
#include <sml/math1>
#include <sml/vec3>
#include <sml/AxisAngle>
#include <sml/angleop>

namespace sml
{
	const vec3 vec(const quat& q)
	{
		return vec3(q.x, q.y, q.z);
	}

	// ---------------------------------------------------------

	quat::quat()
	{
	}

	quat::quat(const real ax, const real ay, const real az, const real aw)
		: x(ax)
		, y(ay)
		, z(az)
		, w(aw)
	{
	}

	void quat::operator*=(const quat& q)
	{
		const float sc = w*q.w - dot(vec(*this), vec(q));
		const vec3 r = vec(q)*w + vec(*this)*q.w + cross(vec(*this), vec(q));
		
		x = r.x;
		y = r.y;
		z = r.z;
		w = sc;
	}

	// ------------------------------------------------------

	real LengthOf(const quat& q)
	{
		return Sqrt( LengthOfSquared(q) );
	}

	real LengthOfSquared(const quat& q)
	{
		return Square(q.x) + Square(q.y) + Square(q.z) + Square(q.w);
	}

	/*
	const quat Slerp(const quat& a, const real v, const quat& b)
	{
		// implement me
		return a;
	}
	// forces the interpolatation to go the "short way"
	const quat SlerpShortway(const quat& a, const real v, const quat& b)
	{
		if( dot(a, b) < 0 ) return Slerp(t, -a, b);
		else Slerp(t, a, b);
	}*/

	/*const quat GetNormalized(const quat& q)
	{
		quat temp = q;
		Normalize(&temp);
		return temp;
	}*/

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
	const quat operator*(const quat& lhs, const quat& rhs)
	{
		quat temp = lhs;
		temp *= rhs;
		return temp;
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

	const vec3 RotateAroundOrigo(const quat& q, const vec3& v)
	{
		const quat r = q * quat(v.x, v.y, v.z, 0) * GetConjugate(q);
		return vec3(r.x, r.y, r.z);
	}

	const quat Combine(const quat& current, const quat& extra)
	{
		return extra * current;
	}

	// ------------------------------------------------------------



	namespace quatu
	{
		const quat From(const AxisAngle& aa)
		{
			const real s = Sin( aa.angle * 0.5 );
			quat q;
			q.x = aa.axis.x * s;
			q.y = aa.axis.y * s;
			q.z = aa.axis.z * s;
			q.w = Cos( aa.angle * 0.5 );

			Normalize(&q);
			return q;
		}

		bool isZero(real r)
		{
			return ( Abs(r) < 0.006 );
		}

		// rewrite to better fit the mathematics instead of this "hack"
		const AxisAngle ToAxisAngle(const quat& q)
		{
			// assert nonunit?
			// check if w is 1 instead?
			if ( isZero(q.x) && isZero(q.y) && isZero(q.z) )
			{
				return AxisAngle(In(), Angle::FromRadians(0));
			}
			else
			{
				return AxisAngle(vec(q), Acos(q.w)*2);
			}
		}

		//const quat From(const Euler& e);
		const quat Identity()
		{
			quat q;
			q.x = q.y = q.z = 0;
			q.w = 1;
			return q;
		}
		const quat LookAt(const vec3& from, const vec3& to, const vec3& up)
		{
			return LookInDirection(to-from, up);
		}
		quat FromMatrix3(const real mat[3][3])
		{
			int   NXT[] = {1, 2, 0};
			real q[4];

			// check the diagonal
			const real tr = mat[0][0] + mat[1][1] + mat[2][2];
			if( tr > 0.0f)
			{
				const real s = (float)Sqrt(tr + 1.0f);
				const real t = 0.5f / s;

				return quat(
					(mat[1][2] - mat[2][1]) * t,
					(mat[2][0] - mat[0][2]) * t,
					(mat[0][1] - mat[1][0]) * t,
					s * 0.5f);
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

				q[i] = s * 0.5f;

				if (s != 0.0f) s = 0.5f / s;

				q[j] = (mat[i][j] + mat[j][i]) * s;
				q[k] = (mat[i][k] + mat[k][i]) * s;
				q[3] = (mat[j][k] - mat[k][j]) * s;

				return quat(q[0], q[1], q[2], q[3]);
			}
		}
		const quat LookInDirection(const vec3& adir, const vec3& up)
		{
			const vec3 dir = GetNormalized(adir);
			const vec3 v = GetNormalized(cross(dir, up));
			const vec3 u = cross(v, dir);
			const vec3 n = -dir;
#define VECARR(v) { v.x, v.y, v.z }
			const real mat[3][3] = { VECARR(v), VECARR(u), VECARR(n) };
#undef VECARR
			return FromMatrix3(mat);
		}
	}
}