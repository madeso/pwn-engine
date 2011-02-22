#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

namespace pwn
{
	namespace math
	{
		const vec3 In(const mat44& m) { return In(cmat33(m)); }
		const vec3 Right(const mat44& m) { return Right(cmat33(m)); }
		const vec3 Up(const mat44& m) { return Up(cmat33(m)); }

		const vec3 Out(const mat44& m) { return Out(cmat33(m)); }
		const vec3 Left(const mat44& m) { return Left(cmat33(m)); }
		const vec3 Down(const mat44& m) { return Down(cmat33(m)); }

		const mat44 mat44_FromColMajor(
									const real data0,  const real data1,  const real data2,  const real data3,
									const real data4,  const real data5,  const real data6,  const real data7, 
									const real data8,  const real data9,  const real data10, const real data11, 
									const real data12, const real data13, const real data14, const real data15 )
		{
			return mat44(data0, data4, data8,  data12,
				data1, data5, data9,  data13,
				data2, data6, data10, data14,
				data3, data7, data11, data15 );
		}

		const mat44 cmat44(const mat33& m)
		{
			return mat44( m(0, 0), m(0, 1), m(0, 2), 0,
				m(1, 0), m(1, 1), m(1, 2), 0,
				m(2, 0), m(2, 1), m(2, 2), 0,
				0         , 0         , 0         , 1);
		}

		namespace
		{
			real multsum(const mat44& a, const mat44& b, int row, int col)
			{
				return a(row, 0) * b(0, col) + a(row, 1) * b(1, col) + a(row, 2) * b(2, col) + a(row, 3) * b(3, col);
			}
			real multsum(const mat44& a, const vec3& b, int row)
			{
				return a(row, 0) * b[0] + a(row, 1) * b[1] + a(row, 2) * b[2]+ a(row, 3) * 1;
			}
		}

		const mat44 operator* (const mat44& a, const mat44& b)
		{
			return mat44( multsum(a,b, 0,0), multsum(a, b, 0, 1), multsum(a, b, 0, 2), multsum(a, b, 0, 3),
				multsum(a,b, 1,0), multsum(a, b, 1, 1), multsum(a, b, 1, 2), multsum(a, b, 1, 3),
				multsum(a,b, 2,0), multsum(a, b, 2, 1), multsum(a, b, 2, 2), multsum(a, b, 2, 3),
				multsum(a,b, 3,0), multsum(a, b, 3, 1), multsum(a, b, 3, 2), multsum(a, b, 3, 3) );
		}
		const vec3 operator *(const mat44& m, const vec3& v)
		{
			return vec3(multsum(m, v, 0), multsum(m, v, 1), multsum(m, v, 2));
		}

		const mat44 SetTransform(const mat44& m, const vec3& t)
		{
			mat44 tmp = m;
			tmp(0,3) = X(t);
			tmp(1,3) = Y(t);
			tmp(2,3) = Z(t);
			return tmp;
		}

		const mat44 cmat44(const AxisAngle& aa)
		{
			const real c = Cos(aa.angle);
			const real s = Sin(aa.angle);
			const real x = X(aa.axis);
			const real y = Y(aa.axis);
			const real z = Z(aa.axis);

			return mat44_FromColMajor( x*x*(1-c)+c,      x*y*(1-c)-z*s,  x*z*(1-c)+y*s,  0,
				y*x*(1-c)+z*s,    y*y*(1-c)+c,    y*z*(1-c)-x*s,  0,
				x*z*(1-c)-y*s,    y*z*(1-c)+x*s,  z*z*(1-c)+c,    0,
				0,                0,              0,              1 );
		}

		const mat44 cmat44(const quat& q)
		{
			return cmat44(cAxisAngle(q));
		}

		const mat44 mat44Identity()
		{
			return mat44(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		}

		const mat44 cmat44(const vec3& v)
		{
			return mat44(1, 0, 0, X(v),
				0, 1, 0, Y(v),
				0, 0, 1, Z(v),
				0, 0, 0, 1);
		}

		vec3 RotateWithInverseMatrix(const vec3 vec, const mat44& mat)
		{
			return vec3(
				X(vec)*mat(0,0) + Y(vec)*mat(1,0) + Z(vec)*mat(2,0),
				X(vec)*mat(0,1) + Y(vec)*mat(1,1) + Z(vec)*mat(2,1),
				X(vec)*mat(0,2) + Y(vec)*mat(1,2) + Z(vec)*mat(2,2));
		}

		vec3 TranslateWithInverseMatrix (const vec3 vec, const mat44& mat)
		{
			return cml::inverse(mat) * vec;
		}

		mat44helper::mat44helper(const mat44& mat)
			: mat(mat)
		{
		}
		mat44helper& mat44helper::mult(const mat44& m)
		{
			mat = mat * m;
			return *this;
		}
		mat44helper& mat44helper::rotate(const AxisAngle& aa)
		{
			return mult(cmat44(aa));
		}

		// GetConjugate(q) comes from making the test TestMat44=>TestRotationQuat run
		mat44helper& mat44helper::rotate(const quat& q)
		{
			return mult( cmat44(cmat33(GetConjugate(q))));
		}
		mat44helper& mat44helper::translate(const vec3& t)
		{
			return mult(cmat44(t));
		}
		const vec3 mat44helper::transform(const vec3& v) const
		{
			return mat * v;
		}
	}
}
