#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

namespace pwn
{
	namespace math
	{
		real X(const vec3& v) { return v[0]; }
		real Y(const vec3& v) { return v[1]; }
		real Z(const vec3& v) { return v[2]; }
		real& X(vec3& v) { return v[0]; }
		real& Y(vec3& v) { return v[1]; }
		real& Z(vec3& v) { return v[2]; }
		const vec3 cvec3(const quat& q)
		{
			return vec3(X(q), Y(q), Z(q));
		}

		const vec3 cvec3(const mat44& m)
		{
			return vec3(m(0, 3), m(1, 3), m(2, 3));
		}

		// -------------------------------------------------------

		const vec3 GetNormalized(const vec3& vec)
		{
			if( vec.length_squared() < 0.001f ) return vec;
			vec3 temp = vec;
			temp.normalize();
			return temp;
		}

		const vec3 crossNorm(const vec3& lhs, const vec3& rhs)
		{
			return GetNormalized( cross(lhs, rhs) );
		}

		vec3 Curve(const vec3& target, const vec3& old, real smoothing)
		{
			return vec3(Curve(X(target), X(old), smoothing), Curve(Y(target), Y(old), smoothing), Curve(Z(target), Z(old), smoothing) );
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
			if ( X(tmp) < 0 ) { compressed |= unit::XSIGN_MASK; X(tmp) = -X(tmp); }
			if ( Y(tmp) < 0 ) { compressed |= unit::YSIGN_MASK; Y(tmp) = -Y(tmp); }
			if ( Z(tmp) < 0 ) { compressed |= unit::ZSIGN_MASK; Z(tmp) = -Z(tmp); }

			// project the normal onto the plane that goes through
			// X0=(1,0,0),Y0=(0,1,0),Z0=(0,0,1).
			// on that plane we choose an (projective!) coordinate system
			// such that X0->(0,0), Y0->(126,0), Z0->(0,126),(0,0,0)->Infinity

			// a little slower... old pack was 4 multiplies and 2 adds.
			// This is 2 multiplies, 2 adds, and a divide....
			float32 w = 126.0f / ( X(tmp) + Y(tmp) + Z(tmp) );
			long xbits = (long)( X(tmp) * w );
			long ybits = (long)( Y(tmp) * w );

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
			if ( compressed & unit::XSIGN_MASK ) X(vec) = -X(vec);
			if ( compressed & unit::YSIGN_MASK ) Y(vec) = -Y(vec);
			if ( compressed & unit::ZSIGN_MASK ) Z(vec) = -Z(vec);

			//Assert( vec.isValid());

			return vec;
		}

		// ----------------------------------------------------------------------------------

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
			return vec3(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(1.0));
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

		namespace
		{
			real X(const vec4& v) { return v[0]; }
			real Y(const vec4& v) { return v[1]; }
			real Z(const vec4& v) { return v[2]; }
		}

		const vec3 cvec3(const vec4& v)
		{
			return vec3(X(v), Y(v), Z(v));
		}
		const vec4 cvec4(const vec3& v)
		{
			return vec4(X(v), Y(v), Z(v), 1);
		}
		const vec4 cvec4(const vec3& v, real w)
		{
			return vec4(X(v), Y(v), Z(v), w);
		}
	}
}
