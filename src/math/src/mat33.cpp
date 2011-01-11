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
		mat33::mat33(const real data[sizes::mat33_matrix_size])
		{
			memcpy(columnMajor, data, sizeof(real) * sizes::mat33_matrix_size);
		}

		real mat33::at(int row, int column) const
		{
			return columnMajor[column * sizes::mat33_size + row];
		}

		real& mat33::at(int row, int column)
		{
			return columnMajor[column * sizes::mat33_size + row];
		}

		// - comes from making the test TestMat44=>TestIn run
		const vec3 In(const mat33& m)
		{
			return vec3( m.at(0,2), m.at(1,2), -m.at(2,2));
		}
		const vec3 Right(const mat33& m)
		{
			return vec3( m.at(0,0), m.at(1,0), -m.at(2,0));
		}
		const vec3 Up(const mat33& m)
		{
			return vec3( m.at(0,1), m.at(1,1), -m.at(2,1));
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

		const mat33 mat33_FromRowMajor(const real data[sizes::mat33_matrix_size])
		{
			const real temp[] = { data[0], data[3], data[6],
				data[1], data[4], data[7],
				data[2], data[5], data[8] };
			return mat33( temp );
		}

		const mat33 Scale(const vec3& scale)
		{
			const real temp[] = { scale.x, 0,       0,
				0,       scale.y, 0,
				0,       0,       scale.z };
			return mat33_FromRowMajor(temp);
		}

		const mat33 mat33Identity()
		{
			const real temp[] = { 1, 0, 0,
				0, 1, 0,
				0, 0, 1 };
			return mat33_FromRowMajor(temp);
		}

		const mat33 cmat33(const mat44& m)
		{
			const real temp[] = { m.at(0, 0), m.at(0, 1), m.at(0, 2),
				m.at(1, 0), m.at(1, 1), m.at(1, 2),
				m.at(2, 0), m.at(2, 1), m.at(2, 2) };
			return mat33_FromRowMajor(temp);
		}

		const mat33 cmat33(const quat& q)
		{
			const real x = q.x;
			const real y = q.y;
			const real z = q.z;
			const real w = q.w;

			const real tXX = 2 * Square(x);
			const real tYY = 2 * Square(y);
			const real tZZ = 2 * Square(z);
			const real tXY = 2 * x * y;
			const real tYZ = 2 * y * z;
			const real tZW = 2 * z * w;
			const real tXW = 2 * x * w;
			const real tXZ = 2 * x * z;
			const real tYW = 2 * y * w;

			const real temp[] = {
				1-tYY-tZZ,  tXY-tZW,    tXZ+tYW,
				tXY+tZW,    1-tXX-tZZ,  tYZ-tXW,
				tXZ-tYW,    tYZ+tXW,    1-tXX-tYY};
			return mat33_FromRowMajor(temp);
		}
	}
}