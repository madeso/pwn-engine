#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <cmath>
#include <memory> // memcpy
#include <pwn/Assert.h>

namespace pwn
{
	namespace math
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// math1

		bool IsEqual(real lhs, real rhs)
		{
			return IsZero(lhs-rhs);
		}

		bool IsZero(real r)
		{
			const real kRange = 0.0001f;
			return IsWithin(-kRange, r, kRange);
		}

		real ZeroOrValue(real r)
		{
			if( IsZero(r) ) return 0;
			else return r;
		}

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
			Assert(r >= 0 && "input must be bigger than 0");
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

		const real To01(const real L, const real v, const real U)
		{
			return (v-L)/(U-L);
		}

		const real From01(const real L, const real v, const real U)
		{
			return v*(U-L)+L;
		}

		const real Remap(const real ol, const real ou, const real v, const real nl, const real nu)
		{
			return From01(nl, To01(ol,v,ou), nu);
		}

		const real Get360Angular(const real min, const real value, const real max)
		{
			const real halfdiff = (max-min)/PWN_MATH_VALUE(2.0);
			return min+halfdiff-halfdiff*Cos(Angle::FromDegrees(value*PWN_MATH_VALUE(360.0)));
		}

		const real KeepWithin(const real min, const real v, const real max)
		{
			if( v > max ) return max;
			else if( v < min ) return min;
			else return v;
		}

		const bool IsWithin(const real min, const real c, const real max)
		{
			return c > min && c < max;
		}

		const bool IsWithinInclusive(const real min, const real c, const real max)
		{
			return c >= min && c <= max;
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

		namespace // internal
		{
			real LowerBound(real num, real gran)
			{
				return std::floor(num/gran) * gran;
			}

			real UpperBound(real num, real gran)
			{
				return std::ceil(num/gran) * gran;
			}
		}

		const real Round(const real num, const real gran)
		{
			const real lower = LowerBound(num, gran);
			const real upper = UpperBound(num, gran);
			const real percent = (num-lower)/gran; // 1.0 is upper, 0.0 is lower, 0.25 is 25% between lower and upper, moving from lower to upper

			if( percent >= 0.5 ) return upper;
			else return lower;
		}

		namespace // local
		{
			typedef union { float32 f; uint32 i; } uif;

			void initELut (uint16 eLut[]);

			int32 eLut(int32 e)
			{
				const int tableSize = 1 << 9; // grabbed from Half/eLut.cpp
				static uint16 eLutable[tableSize];
				static bool init = true;
				if( init )
				{
					initELut(eLutable);
					init = false;
				}
				return eLutable[e];
			}
		}

		/** Renamed and repurposed conversion routines from OpenEXR.
		Licensed under the \ref open-exr-license
		*/
		namespace
		{
			//---------------------------------------------------
			// Interpret an unsigned short bit pattern as a half,
			// and convert that half to the corresponding float's
			// bit pattern.
			//---------------------------------------------------
			uint32 Actual_HalfToFloat(uint16 y)
			{

				int32 s = (y >> 15) & 0x00000001;
				int32 e = (y >> 10) & 0x0000001f;
				int32 m =  y        & 0x000003ff;

				if (e == 0)
				{
					if (m == 0)
					{
						//
						// Plus or minus zero
						//

						return s << 31;
					}
					else
					{
						//
						// Denormalized number -- renormalize it
						//

						while (!(m & 0x00000400))
						{
							m <<= 1;
							e -=  1;
						}

						e += 1;
						m &= ~0x00000400;
					}
				}
				else if (e == 31)
				{
					if (m == 0)
					{
						//
						// Positive or negative infinity
						//

						return (s << 31) | 0x7f800000;
					}
					else
					{
						//
						// Nan -- preserve sign and significand bits
						//

						return (s << 31) | 0x7f800000 | (m << 13);
					}
				}

				//
				// Normalized number
				//

				e = e + (127 - 15);
				m = m << 13;

				//
				// Assemble s, e and m.
				//

				return (s << 31) | (e << 23) | m;
			}

			void initELut (uint16 eLut[])
			{
				for (int i = 0; i < 0x100; i++)
				{
					int e = (i & 0x0ff) - (127 - 15);

					if (e <= 0 || e >= 30)
					{
						//
						// Special case
						//

						eLut[i]         = 0;
						eLut[i | 0x100] = 0;
					}
					else
					{
						//
						// Common case - normalized half, no exponent overflow possible
						//

						eLut[i]         = static_cast<uint16>(e << 10);
						eLut[i | 0x100] = static_cast<uint16>((e << 10) | 0x8000);
					}
				}
			}

			//-----------------------------------------------
			// Overflow handler for float-to-half conversion;
			// generates a hardware floating-point overflow,
			// which may be trapped by the operating system.
			//-----------------------------------------------

			float32 overflow ()
			{
				volatile float32 f = 1e10;

				for (int i = 0; i < 10; i++)	
					f *= f;				// this will overflow before
				// the for­loop terminates
				return f;
			}
			//-----------------------------------------------------
			// Float-to-half conversion -- general case, including
			// zeroes, denormalized numbers and exponent overflows.
			//-----------------------------------------------------

			int16 convert (int32 i)
			{
				//
				// Our floating point number, f, is represented by the bit
				// pattern in integer i.  Disassemble that bit pattern into
				// the sign, s, the exponent, e, and the significand, m.
				// Shift s into the position where it will go in in the
				// resulting half number.
				// Adjust e, accounting for the different exponent bias
				// of float and half (127 versus 15).
				//

				register int32 s =  (i >> 16) & 0x00008000;
				register int32 e = ((i >> 23) & 0x000000ff) - (127 - 15);
				register int32 m =   i        & 0x007fffff;

				//
				// Now reassemble s, e and m into a half:
				//

				if (e <= 0)
				{
					if (e < -10)
					{
						//
						// E is less than -10.  The absolute value of f is
						// less than HALF_MIN (f may be a small normalized
						// float, a denormalized float or a zero).
						//
						// We convert f to a half zero.
						//

						return 0;
					}

					//
					// E is between -10 and 0.  F is a normalized float,
					// whose magnitude is less than HALF_NRM_MIN.
					//
					// We convert f to a denormalized half.
					// 

					m = (m | 0x00800000) >> (1 - e);

					//
					// Round to nearest, round "0.5" up.
					//
					// Rounding may cause the significand to overflow and make
					// our number normalized.  Because of the way a half's bits
					// are laid out, we don't have to treat this case separately;
					// the code below will handle it correctly.
					// 

					if (m &  0x00001000)
						m += 0x00002000;

					//
					// Assemble the half from s, e (zero) and m.
					//

					return static_cast<int16>(s | (m >> 13));
				}
				else if (e == 0xff - (127 - 15))
				{
					if (m == 0)
					{
						//
						// F is an infinity; convert f to a half
						// infinity with the same sign as f.
						//

						return static_cast<int16>(s | 0x7c00);
					}
					else
					{
						//
						// F is a NAN; we produce a half NAN that preserves
						// the sign bit and the 10 leftmost bits of the
						// significand of f, with one exception: If the 10
						// leftmost bits are all zero, the NAN would turn 
						// into an infinity, so we have to set at least one
						// bit in the significand.
						//

						m >>= 13;
						return static_cast<int16>(s | 0x7c00 | m | (m == 0));
					}
				}
				else
				{
					//
					// E is greater than zero.  F is a normalized float.
					// We try to convert f to a normalized half.
					//

					//
					// Round to nearest, round "0.5" up
					//

					if (m &  0x00001000)
					{
						m += 0x00002000;

						if (m & 0x00800000)
						{
							m =  0;		// overflow in significand,
							e += 1;		// adjust exponent
						}
					}

					//
					// Handle exponent overflow
					//

					if (e > 30)
					{
						overflow ();	// Cause a hardware floating point overflow;
						return static_cast<int16>(s | 0x7c00);	// if this returns, the half becomes an
					}   			// infinity with the same sign as f.

					//
					// Assemble the half from s, e and m.
					//

					return static_cast<int16>(s | (e << 10) | (m >> 13));
				}
			}

			uint16 Actual_FloatToHalf(float32 f)
			{
				if (f == 0)
				{
					//
					// Common special case - zero.
					// For speed, we don't preserve the zero's sign.
					//

					return 0;
				}
				else
				{
					//
					// We extract the combined sign and exponent, e, from our
					// floating-point number, f.  Then we convert e to the sign
					// and exponent of the half number via a table lookup.
					//
					// For the most common case, where a normalized half is produced,
					// the table lookup returns a non-zero value; in this case, all
					// we have to do, is round f's significand to 10 bits and combine
					// the result with e.
					//
					// For all other cases (overflow, zeroes, denormalized numbers
					// resulting from underflow, infinities and NANs), the table
					// lookup returns zero, and we call a longer, non-inline function
					// to do the float-to-half conversion.
					//

					uif x;

					x.f = f;

					register int32 e = (x.i >> 23) & 0x000001ff;

					e = eLut(e);

					if (e)
					{
						//
						// Simple case - round the significand and
						// combine it with the sign and exponent.
						//

						return static_cast<int16>(e + (((x.i & 0x007fffff) + 0x00001000) >> 13));
					}
					else
					{
						//
						// Difficult case - call a function.
						//

						return convert (x.i);
					}
				}
			}
		}


		
		float32 HalfToFloat(uint16 h)
		{
			uif v;
			v.i = Actual_HalfToFloat (h);
			return v.f;
		}

		uint16 FloatToHalf(float32 f)
		{
			// this is mostly done due to function-layout. We could have implemented Actual_ here but that would put OpenEXR code here and I don't want that
			return Actual_FloatToHalf(f);
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

		vec2::vec2()
			: x(0)
			, y(0)
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

		// -----------------------------------------

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

		const vec2 FromTo(const vec2& from, const vec2& to)
		{
			return to - from;
		}

		const vec2 Curve(const vec2& target, const vec2& old, real smoothing)
		{
			return vec2(Curve(target.x, old.x, smoothing), Curve(target.y, old.y, smoothing));
		}

		const vec2 ChangeY(const vec2& v, const real newy)
		{
			return vec2(v.x, newy);
		}

		// ---------------------------------------------------

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// vec3

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// quat

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

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// mat33

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

			const real temp[] = { 1-tYY-tZZ,  tXY-tZW,    tXZ+tYW,
				tXY+tZW,    1-tXX-tZZ,  tYZ-tXW,
				tXZ-tYW,    tYZ+tXW,    1-tXX-tYY};
			return mat33_FromRowMajor(temp);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// mat44

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// point2

		point2::point2(real x, real y)
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

		// -------------------------------------------

		const point2 Origo2()
		{
			return point2(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0));
		}

		const real DistanceBetween(const point2& from, const point2& to)
		{
			return LengthOf(FromTo(from, to));
		}

		const point2 ChangeY(const point2& v, const real newy)
		{
			return point2(v.vec.x, newy);
		}

		// -------------------------------------------

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// direction2

		direction2::direction2(real x, real y)
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

		// ------------------------------------------------------------------------

		const real LengthOf(const direction2& dir)
		{
			return LengthOf(dir.vec);
		}
		const direction2 ToUnit(const direction2& dir)
		{
			return direction2( GetNormalized(dir.vec) );
		}

		const direction2 FromTo(const point2& from, const point2& to)
		{
			return direction2( to.vec-from.vec );
		}

		// ------------------------------------------------------------------------

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// point3

		point3::point3(real x, real y, real z)
			: vec(x, y, z)
		{
		}
		point3::point3(const vec3& data)
			: vec(data)
		{
		}
		const real point3::x() const
		{
			return vec.x;
		}
		const real point3::y() const
		{
			return vec.y;
		}
		void point3::operator+=(const direction3& dir)
		{
			vec += dir.vec;
		}

		// -------------------------------------------

		const point3 Origo3()
		{
			return point3(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0));
		}

		const real DistanceBetween(const point3& from, const point3& to)
		{
			return LengthOf(FromTo(from, to));
		}

		const point3 ChangeY(const point3& v, const real newy)
		{
			return point3(v.vec.x, newy, v.vec.z);
		}

		// -------------------------------------------

		const point3 operator+(const point3& lhs, const direction3& rhs)
		{
			return point3(lhs.vec+rhs.vec);
		}

		const point3 operator+(const direction3& lhs, const point3& rhs)
		{
			return point3(lhs.vec+rhs.vec);
		}

		const point3 operator-(const point3& lhs, const direction3& rhs)
		{
			return point3(lhs.vec-rhs.vec);
		}

		const point3 operator-(const direction3& lhs, const point3& rhs)
		{
			return point3(lhs.vec-rhs.vec);
		}

		const point3 operator*(const point3& lhs, const real rhs)
		{
			return point3(lhs.vec*rhs);
		}

		const point3 operator*(const real lhs, const point3& rhs)
		{
			return point3(lhs*rhs.vec);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// direction3

		direction3::direction3(real x, real y, real z)
			: vec(x, y, z)
		{
		}

		direction3::direction3(const vec3& data)
			: vec(data)
		{
		}

		void direction3::operator+=(const direction3& rhs)
		{
			vec += rhs.vec;
		}

		// ------------------------------------------------------------------------

		const real LengthOf(const direction3& dir)
		{
			return LengthOf(dir.vec);
		}
		const direction3 ToUnit(const direction3& dir)
		{
			return direction3( GetNormalized(dir.vec) );
		}

		const direction3 FromTo(const point3& from, const point3& to)
		{
			return direction3( to.vec-from.vec );
		}

		// ------------------------------------------------------------------------

		const direction3 operator+(const direction3& lhs, const direction3& rhs)
		{
			return direction3(lhs.vec+rhs.vec);
		}

		const direction3 operator-(const direction3& lhs, const direction3& rhs)
		{
			return direction3(lhs.vec-rhs.vec);
		}

		const direction3 operator*(const direction3& lhs, const real rhs)
		{
			return direction3(lhs.vec*rhs);
		}
		const direction3 operator*(const real lhs, const direction3& rhs)
		{
			return direction3(lhs*rhs.vec);
		}
		const direction3 operator/(const direction3& lhs, const real rhs)
		{
			return direction3(lhs.vec / rhs);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// rect

		rect::rect(const point2& ul, const point2& lr)
			: upperLeft(ul)
			, lowerRight(lr)
		{
			Assert(ul.x() < lr.x());
			Assert(ul.y() > lr.y());
		}

		// -----------------------------------------------------------------------------

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

		real WidthOf(const rect& r)
		{
			return RightOf(r) - LeftOf(r);
		}

		real HeightOf(const rect& r)
		{
			return TopOf(r) - BottomOf(r);
		}

		real AspectOf(const rect& r)
		{
			return WidthOf(r) / HeightOf(r);
		}

		const rect FromAspectAndContainingInCenter(const rect& containing, real raspect)
		{
			const real aspect = AspectOf(containing);

			// todo: fix equal test...?
			if( aspect == raspect ) {
				return containing;
			}
			else {
				if( aspect > raspect ) {
					// current width is greater than supported
					// base new on current height
					const real width = HeightOf(containing) * raspect;
					const real theRest = WidthOf(containing) - width;
					const real borderSize = theRest / 2;

					return FromLrud(LeftOf(containing)+borderSize, LeftOf(containing)+borderSize+width, TopOf(containing), BottomOf(containing));
				}
				else {
					// current height is greater than supported
					// base new on current width
					const real height = WidthOf(containing) / raspect;
					const real theRest = HeightOf(containing) - height;
					const real borderSize = theRest / 2;

					return FromLrud(LeftOf(containing), RightOf(containing), TopOf(containing)-borderSize, BottomOf(containing)+borderSize);
				}
			}
		}

		const rect FromUpperLeftAndSize(const point2& point, const direction2& size)
		{
			Assert(size.vec.x >= 0);
			Assert(size.vec.y >= 0);
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
			const real w = size.vec.x;
			const real h = size.vec.y;
			const real cx = center.vec.x;
			const real cy = 1-center.vec.y;
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

		const point2 Remap(const rect& from, const point2& p, const rect& to)
		{
			return point2( Remap(LeftOf(from), RightOf(from), p.vec.x, LeftOf(to), RightOf(to)) ,
				Remap(TopOf(from), BottomOf(from), p.vec.y, TopOf(to), BottomOf(to)) );
		}

		const point2 KeepWithin(const point2& loc, const rect& region)
		{
			return point2( KeepWithin(LeftOf(region), loc.vec.x, RightOf(region)), KeepWithin(BottomOf(region), loc.vec.y, TopOf(region)));
		}

		const point2 KeepWithin(const point2& loc, const rect& region, const rect& object)
		{
			return point2( KeepWithin(LeftOf(region)-LeftOf(object), loc.vec.x, RightOf(region)-RightOf(object)), KeepWithin(BottomOf(region)-BottomOf(object), loc.vec.y, TopOf(region)-TopOf(object)));
		}

		const bool IsWithin(const point2& loc, const rect& region)
		{
			return IsWithin(LeftOf(region), loc.vec.x, RightOf(region))
				&& IsWithin(BottomOf(region), loc.vec.y, TopOf(region));
		}

		// ------------------------------------------------------------------------

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
			Assert(v <= 1 && "v must be smaller than 1, use Limmit or Max on the value to not trigger this Assert");
			Assert(v >= -1 && "v must be greater than -1, use Limmit or Min on the value to not trigger this Assert");
			return Angle::FromRadians(std::asin(v));
		}

		const Angle Acos(const real v)
		{
			Assert(v <= 1 && "v must be smaller than 1, use Limmit or Max on the value to not trigger this Assert");
			Assert(v >= -1 && "v must be greater than -1, use Limmit or Min on the value to not trigger this Assert");
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

		// rewrite to better fit the mathematics instead of this "hack"
		const AxisAngle cAxisAngle(const quat& q)
		{
			Assert( q.w <= 1 ); // if this happes, we should normalize, but this shouldnt happen
			const vec3 axis = cvec3(q);
			AxisAngle aa(axis, Acos(q.w)*PWN_MATH_VALUE(2.0));

			return aa;
		}

		const AxisAngle RightHandAround(const vec3& axis, const Angle& angle)
		{
			return AxisAngle(axis, angle);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////// ArcBall

		/*

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

		*/
	}
}