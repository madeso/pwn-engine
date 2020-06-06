#ifndef PWN_MATH_TYPES
#define PWN_MATH_TYPES

#include <pwn/number.h>
#include <cml/cml.h>

namespace pwn
{
	namespace math
	{
		typedef cml::vector<real, cml::fixed<2> > vec2;
		typedef cml::vector<real, cml::fixed<3> > vec3;
		typedef cml::vector<real, cml::fixed<4> > vec4;

		typedef cml::vector<real, cml::external<2> > vec2x;
		typedef cml::vector<real, cml::external<3> > vec3x;
		typedef cml::vector<real, cml::external<4> > vec4x;

		typedef cml::quaternion<real, cml::fixed<>, cml::vector_first, cml::positive_cross> quat;

		struct point2;
		struct direction2;
		struct point3;
		struct direction3;
		struct rect;
		struct AxisAngle;
		struct Angle;
		//struct ArcBall;

		real X(const vec2& v);
		real Y(const vec2& v);
		real& X(vec2& v);
		real& Y(vec2& v);

		real X(const vec3& v);
		real Y(const vec3& v);
		real Z(const vec3& v);
		real& X(vec3& v);
		real& Y(vec3& v);
		real& Z(vec3& v);

		real X(const quat& v);
		real Y(const quat& v);
		real Z(const quat& v);
		real W(const quat& v);
		real& X(quat& v);
		real& Y(quat& v);
		real& Z(quat& v);
		real& W(quat& v);

		typedef cml::matrix<real, cml::fixed<3, 3>, cml::col_basis, cml::col_major > mat33;
		typedef cml::matrix<real, cml::fixed<4, 4>, cml::col_basis, cml::col_major > mat44;

		/** a point in 2d.
		a simple wrapper over vec2 for added type-safety
		@see vec2
		@see direction2
		*/
		struct point2
		{
			/** constructor.
			*/
			point2(real x, real y);

			/** constructor.
			*/
			explicit point2(const vec2& data);

			const real x() const;
			const real y() const;

			/** moves the point.
			*/
			void operator+=(const direction2& dir);

			vec2 vec;
		};

		/** a direction and length in 2d.
		a simple wrapper over vec2 for added type-safety.
		@see vec2
		@see point2
		@todo rename to something that also idicates a magnitude
		*/
		struct direction2
		{
			/** constructor.
			*/
			direction2(real x, real y);

			/** constructor.
			*/
			explicit direction2(const vec2& data);

			/** add to the direction.
			*/
			void operator+=(const direction2& rhs);

			/** the direction itself
			*/
			vec2 vec;
		};

		/** a point in 3d.
		a simple wrapper over vec3 for added type-safety
		@see vec3
		@see direction3
		*/
		struct point3
		{
			/** constructor.
			*/
			point3(real x, real y, real z);

			/** constructor.
			*/
			explicit point3(const vec3& data);

			const real x() const;
			const real y() const;
			const real z() const;

			/** moves the point.
			*/
			void operator+=(const direction3& dir);

			vec3 vec;
		};

		/** a direction and length in 3d.
		a simple wrapper over vec3 for added type-safety.
		@see vec3
		@see point3
		@todo rename to something that also idicates a magnitude
		*/
		struct direction3
		{
			/** constructor.
			*/
			direction3(real x, real y, real z);

			/** constructor.
			*/
			explicit direction3(const vec3& data);

			/** add to the direction.
			*/
			void operator+=(const direction3& rhs);

			/** the direction itself
			*/
			vec3 vec;
		};

		/** a 2 dimensional rect.
		Assumes positive means right & up.
		This means that y value of lowerRight must be lower than upperLeft and the x value must be higher.
		@todo: make upperLeft and lowerRight into get/set/accessor functions to add asserts
		*/
		struct rect
		{
			/** constructor.
			*/
			rect(const point2& ul, const point2& lr);

			/** upper left corner.
			*/
			point2 upperLeft;

			/** lower right corner.
			*/
			point2 lowerRight;
		};

		/** a angle in both degrees and radians.
		inernally stored as a radian.
		*/
		struct Angle
		{
		public:
			/** convert the current value to degrees.
			*/
			const real inDegrees() const;

			/** gets the current value in radians.
			*/
			const real inRadians() const;

			/** creates a angle from degrees.
			*/
			static const Angle FromDegrees(real deg);

			/** creates a angle from radians.
			*/
			static const Angle FromRadians(real rad);

			/** wraps the angle around 360 degrees.
			@see GetWrapped(const Angle&)
			*/
			void wrap();

			/** angle addition.
			*/
			void operator+=(const Angle& rhs);

			/** angle subtraction.
			*/
			void operator-=(const Angle& rhs);

			/** angle scaling.
			*/
			void operator*=(const real r);
		private:
			explicit Angle(real rad);
			real mRad;
		};

		/** a axis-angle construct.
		*/
		struct AxisAngle
		{
			/** according to righthand rule, prefer RightHandAround function for documentation purposes
			@see RightHandAround(const vec3&, const Angle%)
			*/
			AxisAngle(const vec3& axis, const Angle& angle);

			/** a unit-vector.
			*/
			vec3 axis;

			/** rotation according to right-hand rule.
			*/
			Angle angle;
		};

		// arc-ball is on ice since it's implementation was/is crude/not accurate
		/*struct ArcBall
		{
			ArcBall(const vec2& center, const real radius);

			vec2 center;
			real radius;
		};*/
	}
}

#endif
