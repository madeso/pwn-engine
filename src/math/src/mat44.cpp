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
		mat44::mat44()
		{
		}

		mat44::mat44(const real data[sizes::mat44_matrix_size])
		{
			memcpy(columnMajor, data, sizeof(real) * sizes::mat44_matrix_size);
		}

		real mat44::at(int row, int column) const
		{
			return columnMajor[column * sizes::mat44_size + row];
		}
		real& mat44::at(int row, int column)
		{
			return columnMajor[column * sizes::mat44_size + row];
		}

		real mat44::operator[](const unsigned int index) const
		{
			return columnMajor[index];
		}
		real& mat44::operator[](const unsigned int index)
		{
			return columnMajor[index];
		}

		const vec3 In(const mat44& m) { return In(cmat33(m)); }
		const vec3 Right(const mat44& m) { return Right(cmat33(m)); }
		const vec3 Up(const mat44& m) { return Up(cmat33(m)); }

		const vec3 Out(const mat44& m) { return Out(cmat33(m)); }
		const vec3 Left(const mat44& m) { return Left(cmat33(m)); }
		const vec3 Down(const mat44& m) { return Down(cmat33(m)); }

		const mat44 mat44_FromRowMajor(const real data[sizes::mat44_matrix_size])
		{
			const real temp[] = { data[0], data[4], data[8],  data[12],
				data[1], data[5], data[9],  data[13],
				data[2], data[6], data[10], data[14],
				data[3], data[7], data[11], data[15] };
			return mat44(temp);
		}

		const mat44 cmat44(const mat33& m)
		{
			const real temp[] = { m.at(0, 0), m.at(0, 1), m.at(0, 2), 0,
				m.at(1, 0), m.at(1, 1), m.at(1, 2), 0,
				m.at(2, 0), m.at(2, 1), m.at(2, 2), 0,
				0         , 0         , 0         , 1};
			return mat44_FromRowMajor(temp);
		}

		namespace
		{
			real multsum(const mat44& a, const mat44& b, int row, int col)
			{
				return a.at(row, 0) * b.at(0, col) + a.at(row, 1) * b.at(1, col) + a.at(row, 2) * b.at(2, col) + a.at(row, 3) * b.at(3, col);
			}
			real multsum(const mat44& a, const vec3& b, int row)
			{
				return a.at(row, 0) * b[0] + a.at(row, 1) * b[1] + a.at(row, 2) * b[2]+ a.at(row, 3) * 1;
			}
		}

		const mat44 operator* (const mat44& a, const mat44& b)
		{
			const real temp[] = { multsum(a,b, 0,0), multsum(a, b, 0, 1), multsum(a, b, 0, 2), multsum(a, b, 0, 3),
				multsum(a,b, 1,0), multsum(a, b, 1, 1), multsum(a, b, 1, 2), multsum(a, b, 1, 3),
				multsum(a,b, 2,0), multsum(a, b, 2, 1), multsum(a, b, 2, 2), multsum(a, b, 2, 3),
				multsum(a,b, 3,0), multsum(a, b, 3, 1), multsum(a, b, 3, 2), multsum(a, b, 3, 3) };
			return mat44_FromRowMajor(temp);
		}
		const vec3 operator *(const mat44& m, const vec3& v)
		{
			return vec3(multsum(m, v, 0), multsum(m, v, 1), multsum(m, v, 2));
		}

		const mat44 Inverse(const mat44& m)
		{
			const real kEpsilon = 0.0001f;

			const real a0 = m[0]*m[5] - m[1]*m[4];
			const real a1 = m[0]*m[6] - m[2]*m[4];
			const real a2 = m[0]*m[7] - m[3]*m[4];
			const real a3 = m[1]*m[6] - m[2]*m[5];
			const real a4 = m[1]*m[7] - m[3]*m[5];
			const real a5 = m[2]*m[7] - m[3]*m[6];
			const real b0 = m[8]*m[13] - m[9]*m[12];
			const real b1 = m[8]*m[14] - m[10]*m[12];
			const real b2 = m[8]*m[15] - m[11]*m[12];
			const real b3 = m[9]*m[14] - m[10]*m[13];
			const real b4 = m[9]*m[15] - m[11]*m[13];
			const real b5 = m[10]*m[15] - m[11]*m[14];

			const real det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
			if (Abs(det) < kEpsilon) return mat44Identity();
			const real invDet = 1/det;
			
			const real temp[] = { 
				+m[5]*b5 - m[6]*b4 + m[7]*b3, -m[1]*b5 + m[2]*b4 - m[3]*b3, +m[13]*a5 - m[14]*a4 + m[15]*a3, -m[9]*a5 + m[10]*a4 - m[11]*a3,
				-m[4]*b5 + m[6]*b2 - m[7]*b1, +m[0]*b5 - m[2]*b2 + m[3]*b1, -m[12]*a5 + m[14]*a2 - m[15]*a1, +m[8]*a5 - m[10]*a2 + m[11]*a1,
				+m[4]*b4 - m[5]*b2 + m[7]*b0, -m[0]*b4 + m[1]*b2 - m[3]*b0, +m[12]*a4 - m[13]*a2 + m[15]*a0, -m[8]*a4 + m[9]*a2  - m[11]*a0,
				-m[4]*b3 + m[5]*b1 - m[6]*b0, +m[0]*b3 - m[1]*b1 + m[2]*b0, -m[12]*a3 + m[13]*a1 - m[14]*a0, +m[8]*a3 - m[9]*a1  + m[10]*a0 
			};

			mat44 inverse(temp);
			for(int i=0; i<16; ++i)
			{
				inverse[i] *= invDet;
			}
			return inverse;
		}

		const mat44 SetTransform(const mat44& m, const vec3& t)
		{
			mat44 tmp = m;
			tmp.at(0,3) = t.x;
			tmp.at(1,3) = t.y;
			tmp.at(2,3) = t.z;
			return tmp;
		}

		const mat44 cmat44(const AxisAngle& aa)
		{
			const real c = Cos(aa.angle);
			const real s = Sin(aa.angle);
			const real x = aa.axis.x;
			const real y = aa.axis.y;
			const real z = aa.axis.z;

			const real temp[] = { x*x*(1-c)+c,      x*y*(1-c)-z*s,  x*z*(1-c)+y*s,  0,
				y*x*(1-c)+z*s,    y*y*(1-c)+c,    y*z*(1-c)-x*s,  0,
				x*z*(1-c)-y*s,    y*z*(1-c)+x*s,  z*z*(1-c)+c,    0,
				0,                0,              0,              1 };

			return mat44(temp);
		}

		const mat44 cmat44(const quat& q)
		{
			return cmat44(cAxisAngle(q));
		}

		const mat44 mat44Identity()
		{
			const real temp[] = { 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};
			return mat44_FromRowMajor(temp);
		}

		const mat44 cmat44(const vec3& v)
		{
			const real temp[] = { 1, 0, 0, v.x,
				0, 1, 0, v.y,
				0, 0, 1, v.z,
				0, 0, 0, 1};
			return mat44_FromRowMajor(temp);
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