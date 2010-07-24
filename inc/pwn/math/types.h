#ifndef PWN_MATH_TYPES
#define PWN_MATH_TYPES

#include <pwn/number.h>

namespace pwn
{
	namespace math
	{
		struct vec2;
		struct vec3;
		struct quat;
		struct point2;
		struct direction2;
		struct point3;
		struct direction3;
		struct rect;
		struct AxisAngle;
		struct Angle;
		struct ArcBall;

		/** A 2 dimensional vector.
		*/
		struct vec2
		{
			/** The x value.
			*/
			real x;

			/** The y value.
			*/
			real y;

			/** constructor.
			*/
			vec2(const real x, const real y);

			/** constructor.
			*/
			vec2();

			/** a data-pointer.
			@returns a 2-dimensional pointer to the member-variables
			*/
			real* data();
			
			/** a const data-pointer.
			@returns a 2-dimensional const pointer to the member-variables
			*/
			const real* data() const;

			/** index operator.
			valid indices are 0 and 1
			*/
			real& operator[](int index);

			/** const index operator.
			valid indices are 0 and 1
			*/
			const real operator[](int index) const;

			/** vector addition.
			*/
			void operator+=(const vec2& rhs);

			/** vector substraction.
			*/
			void operator-=(const vec2& rhs);

			/** uniform scaling.
			*/
			void operator*=(const real rhs);

			/** uniform scaling.
			*/
			void operator/=(const real rhs);
		};

		/** A 3 dimensional vector.
		*/
		struct vec3
		{
			/** The x value.
			*/
			real x;

			/** The y value.
			*/
			real y;

			/** The z value.
			*/
			real z;

			/** constructor.
			*/
			vec3(const real x, const real y, const real z);

			/** constructor.
			*/
			vec3();

			/** a data-pointer.
			@returns a 2-dimensional pointer to the member-variables
			*/
			real* data();
			
			/** a const data-pointer.
			@returns a 2-dimensional const pointer to the member-variables
			*/
			const real* data() const;

			/** index operator.
			valid indices are 0 and 1
			*/
			real& operator[](int index);

			/** const index operator.
			valid indices are 0 and 1
			*/
			const real operator[](int index) const;

			/** vector addition.
			*/
			void operator+=(const vec3& rhs);

			/** vector substraction.
			*/
			void operator-=(const vec3& rhs);

			/** uniform scaling.
			*/
			void operator*=(const real rhs);

			/** uniform scaling.
			*/
			void operator/=(const real rhs);
		};

		/** A quaternion
		*/
		struct quat
		{
			/**
			*/
			real x;

			/** The z value.
			*/
			real y;

			/** The z value.
			*/
			real z;

			/** The z value.
			*/
			real w;

			/** deprecated.
			*/
			quat(const real x, const real y, const real z, const real w);

			/** constructor.
			*/
			quat(const vec3& v, const real w);

			/** constructor.
			*/
			quat(const real w, const vec3& v);

			/** quaternion multiplication.
			*/
			void operator*=(const quat& q);

			/** quaternion addition.
			*/
			void operator+=(const quat& rhs);

			/** quaternion subtraction.
			*/
			void operator-=(const quat& rhs);

			/** quaternion scaling.
			*/
			void operator*=(const real rhs);

			/** quaternion scaling.
			*/
			void operator/=(const real rhs);
		};

		namespace sizes
		{
			/** the size of the columns/rows of the 3x3 matrix
			*/
			const int mat33_size = 3;

			/** the total number of elements of the 3x3 matrix.
			*/
			const int mat33_matrix_size = mat33_size*mat33_size;
		}

		/** A 3x3 matrix.
		@see mat44
		*/
		struct mat33
		{
			/** constructor.
			copies the elements
			*/
			mat33(const real data[sizes::mat33_matrix_size]);
			
			/** A column major layout.
			@code
			    | 0 3 6 |
			M = | 1 4 7 |
			    | 2 5 8 |
			@endcode
			*/
			real columnMajor[sizes::mat33_matrix_size];

			/** accessor function.
			*/
			real at(int row, int column) const;

			/** accessor function.
			might be removed in favor of a set-function instead
			*/
			real& at(int row, int column);
		};

		namespace sizes
		{
			/** the size of the columns/rows of the 4x4 matrix
			*/
			const int mat44_size = 4;

			/** the total number of elements of the 4x4 matrix.
			*/
			const int mat44_matrix_size = mat44_size*mat44_size;
		}

		/** A 4x4 matrix.
		@see mat33
		*/
		struct mat44
		{
			/** constructor.
			copies the elements
			*/
			mat44(const real data[sizes::mat44_matrix_size]);

			/** Column major layout.
			@code
			    | 0 4 8  12 |
			M = | 1 5 9  13 |
			    | 2 5 10 14 |
			    | 3 7 11 15 |
			@endcode
			*/
			real columnMajor[sizes::mat44_matrix_size];

			/** accessor function.
			*/
			real at(int row, int column) const;

			/** accessor function.
			might be removed in favor of a set-function instead
			*/
			real& at(int row, int column);
		};

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