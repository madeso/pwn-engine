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
		const vec3 cvec3(const quat& q)
		{
			return vec3(q.x, q.y, q.z);
		}

		const vec3 cvec3(const mat44& m)
		{
			return vec3(m.at(0, 3), m.at(1, 3), m.at(2, 3));
		}

		vec3::vec3(const real ax, const real ay, const real az)
			: x(ax)
			, y(ay)
			, z(az)
		{
		}

		vec3::vec3()
			: x(0)
			, y(0)
			, z(0)
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
			const real length = LengthOf(*vec);
			if( length > PWN_MATH_VALUE(0.001) )
			{
				*vec /= length;
			}
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

		const vec3 Lerp(const vec3& f, real scale, const vec3& t)
		{
			return f + (t - f) * scale;
		}

		const vec3 Qlerp(const vec3& f, real scale, const vec3& t)
		{
			const real sscale = Square(scale);
			return f*(1-sscale) + t*sscale;
		}

		namespace
		{
			// http://www.oroboro.com/rafael/docserv.php?index=programming&article=unitv2

			// A Unit Vector to 16-bit word conversion
			// algorithm based on work of Rafael Baptista (rafael@oroboro.com)
			// Accuracy improved by O.D. (punkfloyd@rocketmail.com)

			// port-note: there is a _finite portable in cppunit that can be found in include/cppunit/portability/FloatingPoint.h

			namespace unit
			{
				// upper 3 bits
				const uint16 SIGN_MASK = 0xe000;
				const uint16 XSIGN_MASK = 0x8000;
				const uint16 YSIGN_MASK = 0x4000;
				const uint16 ZSIGN_MASK = 0x2000;

				// middle 6 bits - xbits
				const uint16 TOP_MASK = 0x1f80;

				// lower 7 bits - ybits
				const uint16 BOTTOM_MASK = 0x007f;

				struct Constants
				{
					float32 mUVAdjustment[0x2000];

					Constants()
					{
						for ( uint16 idx = 0; idx < 0x2000; idx++ )
						{
							long xbits = idx >> 7;
							long ybits = idx & BOTTOM_MASK;

							// map the numbers back to the triangle (0,0)-(0,127)-(127,0)
							if (( xbits + ybits ) >= 127 )
							{
								xbits = 127 - xbits;
								ybits = 127 - ybits;
							}

							// convert to 3D vectors
							float32 x = (float32)xbits;
							float32 y = (float32)ybits;
							float32 z = (float32)( 126 - xbits - ybits );

							// calculate the amount of normalization required
							mUVAdjustment[idx] = 1.0f / sqrtf( y*y + z*z + x*x );
							//Assert( _finite( mUVAdjustment[idx]));
						}
					}
				};

				Constants& CompressedConstants()
				{
					static Constants c;
					return c;
				}
			}
		}

		const uint16 UnitVectorToCompressed(const vec3& vec)
		{
			// convert from vec3 to cUnitVector

			//Assert( vec.isValid());
			vec3 tmp = vec;

			// input vector does not have to be unit length
			// Assert( tmp.length() <= 1.001f );

			uint16 compressed = 0;
			if ( tmp.x < 0 ) { compressed |= unit::XSIGN_MASK; tmp.x = -tmp.x; }
			if ( tmp.y < 0 ) { compressed |= unit::YSIGN_MASK; tmp.y = -tmp.y; }
			if ( tmp.z < 0 ) { compressed |= unit::ZSIGN_MASK; tmp.z = -tmp.z; }

			// project the normal onto the plane that goes through
			// X0=(1,0,0),Y0=(0,1,0),Z0=(0,0,1).
			// on that plane we choose an (projective!) coordinate system
			// such that X0->(0,0), Y0->(126,0), Z0->(0,126),(0,0,0)->Infinity

			// a little slower... old pack was 4 multiplies and 2 adds.
			// This is 2 multiplies, 2 adds, and a divide....
			float32 w = 126.0f / ( tmp.x + tmp.y + tmp.z );
			long xbits = (long)( tmp.x * w );
			long ybits = (long)( tmp.y * w );

			Assert( xbits <  127 );
			Assert( xbits >= 0   );
			Assert( ybits <  127 );
			Assert( ybits >= 0   );

			// Now we can be sure that 0<=xp<=126, 0<=yp<=126, 0<=xp+yp<=126
			// however for the sampling we want to transform this triangle
			// into a rectangle.
			if ( xbits >= 64 )
			{
				xbits = 127 - xbits;
				ybits = 127 - ybits;
			}

			// now we that have xp in the range (0,127) and yp in
			// the range (0,63), we can pack all the bits together
			compressed |= ( xbits << 7 );
			compressed |= ybits;

			return compressed;
		}

		const vec3 CompressedToUnitVector(uint16 compressed)
		{
			// if we do a straightforward backward transform
			// we will get points on the plane X0,Y0,Z0
			// however we need points on a sphere that goes through
			// these points. Therefore we need to adjust x,y,z so
			// that x^2+y^2+z^2=1 by normalizing the vector. We have
			// already precalculated the amount by which we need to
			// scale, so all we do is a table lookup and a
			// multiplication

			// get the x and y bits
			long xbits = (( compressed & unit::TOP_MASK ) >> 7 );
			long ybits = ( compressed & unit::BOTTOM_MASK );

			// map the numbers back to the triangle (0,0)-(0,126)-(126,0)
			if (( xbits + ybits ) >= 127 )
			{
				xbits = 127 - xbits;
				ybits = 127 - ybits;
			}

			// do the inverse transform and normalization
			// costs 3 extra multiplies and 2 subtracts. No big deal.
			float32 uvadj = unit::CompressedConstants().mUVAdjustment[compressed & ~unit::SIGN_MASK];
			vec3 vec(uvadj * (float32) xbits, uvadj * (float32) ybits, uvadj * (float32)( 126 - xbits - ybits ));

			// set all the sign bits
			if ( compressed & unit::XSIGN_MASK ) vec.x = -vec.x;
			if ( compressed & unit::YSIGN_MASK ) vec.y = -vec.y;
			if ( compressed & unit::ZSIGN_MASK ) vec.z = -vec.z;

			//Assert( vec.isValid());

			return vec;
		}

		// ----------------------------------------------------------------------------------

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

		// -----------------------------------------------------------------------------------

		const Angle AngleBetween(const vec3& a, const vec3& b)
		{
			return Acos( KeepWithin(-1, dot(GetNormalized(a), b),1) );
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
	}
}