#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

namespace pwn
{
	namespace math
	{
		// - comes from making the test TestMat44=>TestIn run
		const vec3 In(const mat33& m)
		{
			return vec3(m(0, 2), m(1, 2), -m(2, 2));
		}
		const vec3 Right(const mat33& m)
		{
			return vec3(m(0, 0), m(1, 0), -m(2, 0));
		}
		const vec3 Up(const mat33& m)
		{
			return vec3(m(0, 1), m(1, 1), -m(2, 1));
		}

		const vec3 Out(const mat33& m)
		{
			return -In(m);
		}
		const vec3 Left(const mat33& m)
		{
			return -Right(m);
		}
		const vec3 Down(const mat33& m)
		{
			return -Up(m);
		}

		const mat33 mat33_FromColMajor(const real data0, const real data1, const real data2,
		                               const real data3, const real data4, const real data5,
		                               const real data6, const real data7, const real data8)
		{
			return mat33(data0, data3, data6,
			             data1, data4, data7,
			             data2, data5, data8);
		}

		const mat33 Scale(const vec3& scale)
		{
			return mat33(X(scale), 0,       0,
			             0,       Y(scale), 0,
			             0,       0,       Z(scale));
		}

		const mat33 mat33Identity()
		{
			mat33 r;
			r.identity();
			return r;
		}

		const mat33 cmat33(const mat44& m)
		{
			return mat33(m(0, 0), m(0, 1), m(0, 2),
			             m(1, 0), m(1, 1), m(1, 2),
			             m(2, 0), m(2, 1), m(2, 2));
		}

		const mat33 cmat33(const quat& q)
		{
			const real x = X(q);
			const real y = Y(q);
			const real z = Z(q);
			const real w = W(q);

			const real tXX = 2 * Square(x);
			const real tYY = 2 * Square(y);
			const real tZZ = 2 * Square(z);
			const real tXY = 2 * x * y;
			const real tYZ = 2 * y * z;
			const real tZW = 2 * z * w;
			const real tXW = 2 * x * w;
			const real tXZ = 2 * x * z;
			const real tYW = 2 * y * w;

			return mat33(
			          1 - tYY - tZZ,  tXY - tZW,    tXZ + tYW,
			          tXY + tZW,    1 - tXX - tZZ,  tYZ - tXW,
			          tXZ - tYW,    tYZ + tXW,    1 - tXX - tYY);
		}
	}
}
