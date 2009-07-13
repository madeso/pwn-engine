#include <pwn/math/types>
#include <pwn/math/functions>

#include <cmath>

namespace pwn
{
	namespace math
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// math1

		const int Sign(const real r)
		{
			if( r > PWN_MATH_VALUE(0.0) ) return 1;
			else return -1;
		}

		const real Curve(const real newValue, const real oldValue, const real smoothingValue)
		{
			const int sign = Sign(oldValue - newValue);
			const real slip = (oldValue - newValue) / smoothingValue;
			const real val = oldValue - slip;
			if (sign != Sign(val - newValue)) return newValue;
			else return val;
		}

		const real Square(const real r)
		{
			return r*r;
		}

		const real Sqrt(const real r)
		{
			return std::sqrt(r);
		}

		const real Abs(const real r)
		{
			if(r < PWN_MATH_VALUE(0.0) ) return -r;
			else return r;
		}

		const real Min(const real lhs, const real rhs)
		{
			if( lhs < rhs ) return lhs;
			else return rhs;
		}

		const real Max(const real lhs, const real rhs)
		{
			if( lhs > rhs ) return lhs;
			else return rhs;
		}

		const real Limmit(const real min, const real value, const real max)
		{
			return Max(min, Min(max, value));
		}

		const real To01(const real L, const real v, const real R)
		{
			return (v-L)/(R-L);
		}

		const real From01(const real L, const real v, const real R)
		{
			return v*(R-L)+L;
		}

		const real Remap(const real ol, const real or, const real v, const real nl, const real nr)
		{
			return From01(nl, To01(ol,v,or), nr);
		}

		const real Get360Angular(const real min, const real value, const real max)
		{
			const real halfdiff = (max-min)/PWN_MATH_VALUE(2.0);
			return min+halfdiff-halfdiff*Cos(Angle::FromDegrees(value*PWN_MATH_VALUE(360.0)));
		}

		const real Within(const real min, const real v, const real max)
		{
			if( v > max ) return max;
			else if( v < min ) return min;
			else return v;
		}

		const bool IsWithin(const real min, const real c, const real max)
		{
			return c > min && c < max;
		}

		const real Wrap(const real min, const real v, const real max)
		{
			const real diff = max - min;
			real value = v - min;
			while( value < PWN_MATH_VALUE(0.0) ) value += diff;
			while( value > diff ) value -= diff;
			return min + value;
		}

		const int Wrapi(const int min, const int v, const int max)
		{
			const int diff = max - min+1;
			int value = v;
			while( value < min ) value += diff;
			while( value > max ) value -= diff;
			return value;
		}

		// return number of wraps (with sign)
		const int IncrementAndWrap(const real min, real* current, const real change, const real max)
		{
			const real diff = max - min;
			real value = *current + change;
			int wraps = 0;
			while( value < min ) {
				value += diff;
				--wraps;
			}
			while( value > max ) {
				value -= diff;
				++wraps;
			}
			*current = value;
			return wraps;
		}

		const int IncrementAndWrapi(const int min, int* current, const int change, const int max)
		{
			const int diff = max - min+1;
			int value = *current + change;
			int wraps = 0;
			while( value < min ) {
				value += diff;
				--wraps;
			}
			while( value > max ) {
				value -= diff;
				++wraps;
			}
			*current = value;
			return wraps;
		}

		const real Pi()
		{
			return PWN_MATH_VALUE(3.1415926535897932384626433832795);
		}

		const real HalfPi()
		{
			return Pi()/PWN_MATH_VALUE(2.0);
		}

		const bool kTrue()
		{
			return true;
		}

		const bool kFalse()
		{
			return false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// vec2

		vec2::vec2(real ax, real ay)
			: x(ax)
			, y(ay)
		{
		}

		real* vec2::data()
		{
			return &x;
		}

		const real* vec2::data() const
		{
			return &x;
		}

		real& vec2::operator[](int index)
		{
			if( index == 0 ) return x;
			else return y;
		}

		const real vec2::operator[](int index) const
		{
			if( index == 0 ) return x;
			else return y;
		}

		void vec2::operator+=(const vec2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
		}

		void vec2::operator-=(const vec2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		}

		void vec2::operator*=(const real rhs)
		{
			x *= rhs;
			y *= rhs;
		}

		void vec2::operator/=(const real rhs)
		{
			x /= rhs;
			y /= rhs;
		}

		real LengthOf(const vec2& vec)
		{
			//return c(&vec).length();
			return Sqrt(LengthOfSquared(vec));
		}

		real LengthOfSquared(const vec2& vec)
		{
			return Square(vec.x) + Square(vec.y);
		}

		const vec2 GetNormalized(const vec2& vec)
		{
			vec2 temp(vec);
			Normalize(&temp);
			return temp;
		}
		void Normalize(vec2* vec)
		{
			*vec /= LengthOf(*vec);
		}

		const vec2 operator+(const vec2& lhs, const vec2& rhs)
		{
			vec2 temp(lhs);
			temp+= rhs;
			return temp;
		}
		const vec2 operator-(const vec2& lhs, const vec2& rhs)
		{
			vec2 temp(lhs);
			temp-= rhs;
			return temp;
		}

		const vec2 operator*(const real scalar, const vec2& vec)
		{
			vec2 temp(vec);
			temp *= scalar;
			return temp;
		}
		const vec2 operator*(const vec2& vec, const real scalar)
		{
			vec2 temp(vec);
			temp *= scalar;
			return temp;
		}
		const vec2 operator/(const vec2& vec, const real scalar)
		{
			vec2 temp(vec);
			temp /= scalar;
			return temp;
		}
		const vec2 operator-(const vec2& vec)
		{
			return vec2(-vec.x, -vec.y);
		}

		const vec2 FromTo(const vec2& from, const vec2& to)
		{
			return to - from;
		}

		const vec2 Curve(const vec2& target, const vec2& old, float smoothing)
		{
			return vec2(Curve(target.x, old.x, smoothing), Curve(target.y, old.y, smoothing));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// vec3

		vec3::vec3(const quat& q)
			: x(q.x)
			, y(q.y)
			, z(q.z)
		{
		}

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
			return vec3(lhs.y*rhs.z - lhs.z*rhs.y, lhs.z*rhs.x - lhs.x*rhs.z, lhs.x*rhs.y - lhs.y*rhs.x);
		}

		const real dot(const vec3& lhs, const vec3& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}

		const vec3 crossNorm(const vec3& lhs, const vec3& rhs)
		{
			return GetNormalized( cross(lhs, rhs) );
		}

		vec3 Curve(const vec3& target, const vec3& old, real smoothing)
		{
			return vec3(Curve(target.x, old.x, smoothing), Curve(target.y, old.y, smoothing), Curve(target.z, old.z, smoothing) );
		}

		const vec3 FromTo(const vec3& from, const vec3& to)
		{
			return to - from;
		}

		vec3 lerp(const vec3& f, real scale, const vec3& t)
		{
			return f + (t - f) * scale;
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
			if ( t.inDegrees() > PWN_MATH_VALUE(90.0) ) return Angle::FromDegrees(180)+s;
			else return s;
		}


		const vec3 Right()
		{
			return vec3(PWN_MATH_VALUE(1.0),PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0));
		}
		const vec3 In()
		{
			return vec3(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(-1.0));
		}
		const vec3 Up()
		{
			return vec3(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(1.0),PWN_MATH_VALUE(0.0));
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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// quat

		const vec3 vec(const quat& q)
		{
			return vec3(q.x, q.y, q.z);
		}

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

		void quat::operator*=(const quat& q)
		{
			const float sc = w*q.w - dot(vec(*this), vec(q));
			const vec3 r = vec(q)*w + vec(*this)*q.w + cross(vec(*this), vec(q));

			x = r.x;
			y = r.y;
			z = r.z;
			w = sc;
		}

		void quat::operator+=(const quat& rhs)
		{
#define ACT +=
			x ACT rhs.x;
			y ACT rhs.y;
			z ACT rhs.z;
			w ACT rhs.w;
#undef ACT
		}

		void quat::operator-=(const quat& rhs)
		{
#define ACT -=
			x ACT rhs.x;
			y ACT rhs.y;
			z ACT rhs.z;
			w ACT rhs.w;
#undef ACT
		}

		void quat::operator*=(const real rhs)
		{
#define ACT *=
			x ACT rhs;
			y ACT rhs;
			z ACT rhs;
			w ACT rhs;
#undef ACT
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

		quat lerp(const quat& a, const float v, const quat& b)
		{
			return GetNormalized(a + v * (b - a));
		}

		const quat Slerp(const quat& a, const real v, const quat& b)
		{
			float d = dot(a, b);
			if (d > PWN_MATH_VALUE(0.9995))
			{
				return lerp(a, v, b);
			}
			d = Within(-1, d, 1);
			const Angle theta0 = Acos(d);
			const Angle theta = theta0 * v;

			const quat q = GetNormalized(b - a * d);
			return a * Cos(theta) + q * Sin(theta);
		}

#pragma warning(push)
// 'pwn::math::SlerpShortway' : not all control paths return a value
#pragma warning(disable:4715)
		// forces the interpolatation to go the "short way"
		const quat SlerpShortway(const quat& a, const real v, const quat& b)
		{
			if( dot(a, b) < PWN_MATH_VALUE(0.0) ) return Slerp(-a, v, b);
			else Slerp(a, v, b);
		}
#pragma warning(pop)

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

		const vec3 RightUpIn(const quat& q, const vec3& v)
		{
			return Right(q)*v.x + Up(q)*v.x + In(q)*v.z;
		}

		const vec3 RotateAroundOrigo(const quat& q, const vec3& v)
		{
			const quat r = q * quat(v.x, v.y, v.z, PWN_MATH_VALUE(0.0)) * GetConjugate(q);
			return vec3(r.x, r.y, r.z);
		}

		const quat Combine(const quat& current, const quat& extra)
		{
			return extra * current;
		}

		const quat cquat(const AxisAngle& aa)
		{
			const real s = Sin( aa.angle * 0.5 );
			quat q;
			q.x = aa.axis.x * s;
			q.y = aa.axis.y * s;
			q.z = aa.axis.z * s;
			q.w = Cos( aa.angle * PWN_MATH_VALUE(0.5) );

			Normalize(&q);
			return q;
		}

		namespace // local
		{
			bool isZero(real r)
			{
				return ( Abs(r) < PWN_MATH_VALUE(0.006) );
			}
		}

		// rewrite to better fit the mathematics instead of this "hack"
		const AxisAngle ToAxisAngle(const quat& q)
		{
			// assert nonunit?
			// check if w is 1 instead?
			if ( isZero(q.x) && isZero(q.y) && isZero(q.z) )
			{
				return AxisAngle(In(), Angle::FromRadians(PWN_MATH_VALUE(0.0)));
			}
			else
			{
				return AxisAngle(vec(q), Acos(q.w)*PWN_MATH_VALUE(2.0));
			}
		}

		const quat FpsQuat(const float dx, const float dy)
		{
			const quat rx = cquat(RightHandAround(Up(), Angle::FromDegrees(-dx)));
			const quat ry = cquat(RightHandAround(Right(), Angle::FromDegrees(-dy)));
			const quat final = rx * ry;
			return final;
		}

		// todo: add
		//const quat cquat(const Euler& e);

		const quat qIdentity()
		{
			quat q;
			q.x = q.y = q.z = PWN_MATH_VALUE(0.0);
			q.w = PWN_MATH_VALUE(1.0);
			return q;
		}

		const quat qLookAt(const vec3& from, const vec3& to, const vec3& up)
		{
			return qLookInDirection(to-from, up);
		}

		quat FromMatrix3(const real mat[3][3])
		{
			int   NXT[] = {1, 2, 0};
			real q[4];

			// check the diagonal
			const real tr = mat[0][0] + mat[1][1] + mat[2][2];
			if( tr > PWN_MATH_VALUE(0.0))
			{
				const real s = (float)Sqrt(tr + 1.0f);
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
			const vec3 dir = GetNormalized(adir);
			const vec3 v = GetNormalized(cross(dir, up));
			const vec3 u = cross(v, dir);
			const vec3 n = -dir;
#define VECARR(v) { v.x, v.y, v.z }
			const real mat[3][3] = { VECARR(v), VECARR(u), VECARR(n) };
#undef VECARR
			return FromMatrix3(mat);
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
			return quat(-vec3(q), -q.w);
		}

		real dot(const quat& lhs, const quat& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z +lhs.w * rhs.w;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// point2

		point2::point2(float x, float y)
			: vec(x, y)
		{
		}
		point2::point2(const vec2& data)
			: vec(data)
		{
		}
		const real point2::x() const
		{
			return vec.x;
		}
		const real point2::y() const
		{
			return vec.y;
		}
		void point2::operator+=(const direction2& dir)
		{
			vec += dir.vec;
		}
		const point2 Origo2()
		{
			return point2(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0));
		}

		const point2 operator+(const point2& lhs, const direction2& rhs)
		{
			return point2(lhs.vec+rhs.vec);
		}
		const point2 operator+(const direction2& lhs, const point2& rhs)
		{
			return point2(lhs.vec+rhs.vec);
		}
		const point2 operator-(const point2& lhs, const direction2& rhs)
		{
			return point2(lhs.vec-rhs.vec);
		}
		const point2 operator-(const direction2& lhs, const point2& rhs)
		{
			return point2(lhs.vec-rhs.vec);
		}
		const point2 operator*(const point2& lhs, const real rhs)
		{
			return point2(lhs.vec*rhs);
		}
		const point2 operator*(const real lhs, const point2& rhs)
		{
			return point2(lhs*rhs.vec);
		}
		
		const float DistanceBetween(const point2& from, const point2& to)
		{
			return LengthOf(Between(from, to));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// direction2

		direction2::direction2(float x, float y)
			: vec(x, y)
		{
		}
		direction2::direction2(const vec2& data)
			: vec(data)
		{
		}
		void direction2::operator+=(const direction2& rhs)
		{
			vec += rhs.vec;
		}
		
		const direction2 operator+(const direction2& lhs, const direction2& rhs)
		{
			return direction2(lhs.vec+rhs.vec);
		}

		
		const direction2 operator-(const direction2& lhs, const direction2& rhs)
		{
			return direction2(lhs.vec-rhs.vec);
		}

		const direction2 operator*(const direction2& lhs, const real rhs)
		{
			return direction2(lhs.vec*rhs);
		}
		const direction2 operator*(const real lhs, const direction2& rhs)
		{
			return direction2(lhs*rhs.vec);
		}
		const direction2 operator/(const direction2& lhs, const real rhs)
		{
			return direction2(lhs.vec / rhs);
		}

		const real LengthOf(const direction2& dir)
		{
			return LengthOf(dir.vec);
		}
		const direction2 ToUnit(const direction2& dir)
		{
			return direction2( GetNormalized(dir.vec) );
		}

		const direction2 Between(const point2& from, const point2& to)
		{
			return direction2( to.vec-from.vec );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// rect

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// Angle
		namespace // local
		{
			const real rad2deg(const real rad)
			{
				return (PWN_MATH_VALUE(180.0)/Pi()) * rad;
			}
			const real deg2rad(const real deg)
			{
				return Pi()/PWN_MATH_VALUE(180.0) * deg;
			}
		}

		const real Angle::inDegrees() const
		{
			return rad2deg(mRad);
		}

		const real Angle::inRadians() const
		{
			return mRad;
		}

		const Angle Angle::FromDegrees(real deg)
		{
			return Angle(deg2rad(deg));
		}

		const Angle Angle::FromRadians(real rad)
		{
			return Angle(rad);
		}

		Angle::Angle(real rad)
			: mRad(rad)
		{
		}

		void Angle::wrap()
		{
			mRad = Wrap(0, mRad, Pi()*PWN_MATH_VALUE(2.0));
		}

		void Angle::operator+=(const Angle& rhs)
		{
			mRad += rhs.mRad;
		}

		void Angle::operator-=(const Angle& rhs)
		{
			mRad -= rhs.mRad;
		}

		void Angle::operator *=(const real rhs)
		{
			mRad *= rhs;
		}

		const real Sin(const Angle& ang)
		{
			return std::sin(ang.inRadians());
		}

		const real Cos(const Angle& ang)
		{
			return std::cos(ang.inRadians());
		}

		const real Tan(const Angle& ang)
		{
			return std::tan(ang.inRadians());
		}

		const Angle Asin(const real v)
		{
			return Angle::FromRadians(std::asin(v));
		}

		const Angle Acos(const real v)
		{
			return Angle::FromRadians(std::acos(v));
		}

		const Angle Atan(const real v)
		{
			return Angle::FromRadians(std::atan(v));
		}

		const Angle GetWrapped(const Angle& a)
		{
			Angle temp(a);
			temp.wrap();
			return temp;
		}

		const Angle operator+(const Angle& lhs, const Angle& rhs)
		{
			Angle temp(lhs);
			temp+=rhs;
			return temp;
		}

		const Angle operator-(const Angle& lhs, const Angle& rhs)
		{
			Angle temp(lhs);
			temp-=rhs;
			return temp;
		}

		const Angle operator*(const Angle& lhs, const real rhs)
		{
			Angle temp(lhs);
			temp*=rhs;
			return temp;
		}

		const Angle operator*(const real rhs, const Angle& lhs)
		{
			return lhs*rhs;
		}

		const Angle FromPercentOf360(const real percent)
		{
			return Angle::FromRadians(percent * Pi() * PWN_MATH_VALUE(2.0));
		}

		const Angle Zero()
		{
			return Angle::FromRadians(0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// axis-angle

		AxisAngle::AxisAngle(const vec3& ax, const Angle& ang)
			: axis( GetNormalized(ax) )
			, angle(ang)
		{
		}

		const AxisAngle RightHandAround(const vec3& axis, const Angle& angle)
		{
			return AxisAngle(axis, angle);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// ArcBall

		ArcBall::ArcBall(const vec2& center, const real radius)
			: center(center)
			, radius(radius)
		{
		}

		// code found somewhere that seemed to be working, is this correct?

		namespace // local
		{
			vec3 PointOnBall(const ArcBall& ab, const vec2& v)
			{
				const real x = ( v.x - ab.center.x ) / ab.radius;
				const real y = ( v.y - ab.center.y ) / ab.radius;
				const real rsq = Square(x) + Square(y);
				const real z = rsq < PWN_MATH_VALUE(1.0) ? Sqrt(PWN_MATH_VALUE(1.0) - rsq) : PWN_MATH_VALUE(0.0);
				return GetNormalized(vec3(x,y,z));
			}
		}

		quat GetRotation(const ArcBall& arc, const vec2 from, const vec2 to)
		{
			const vec3 f = PointOnBall(arc, from);
			const vec3 t = PointOnBall(arc, to);
			const vec3 axis = crossNorm(f,t);
			const Angle angle = AngleBetween(f,t);
			return cquat(RightHandAround(axis, angle));
		}
	}
}