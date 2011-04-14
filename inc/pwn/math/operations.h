#ifndef PWN_MATH_FUNCTIONS
#define PWN_MATH_FUNCTIONS

#include <pwn/math/types.h>

namespace pwn
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup math1 1-D math operations
		/// @{

		bool IsEqual(real lhs, real rhs);
		bool IsZero(real r);
		real ZeroOrValue(real r);

		/** Calculates the sign as a positive or a negative int.
		@returns 1 if r is greater than 0, -1 if not.
		@see Abs()
		*/
		const int Sign(const real r);

		const real Lerp(const real f, real scale, const real t);

		/** Performs a single interpolating step to transform a old value to a new value.
		Larger smoothing values result in a smoother but less resposive path

		Example:
		@code mousex = Curve( ( MousePos.X - OldMousePos.X ) * Sensitivity(), mousex, 6 ); @endcode

		@param target is the target value
		@param current is the current value
		@param smoothing is the smoothing value > 0, the lower, the less smooth

		@returns The smoothed result.
		*/
		const real Curve(const real target, const real current, const real smoothing);

		/** Calculates the square of the argument.
		@see Sqrt()
		*/
		const real Square(const real r);

		/** Calculates the square-root of the argument.
		@see Square()
		*/
		const real Sqrt(const real r);

		/** Calculates the absolute value of the argument.
		@see Sign()
		*/
		const real Abs(const real r);

		/** Calculates the minimum of two values
		@see Max()
		@see KeepWithin()
		*/
		const real Min(const real lhs, const real rhs);

		/** Calculates the maximum of two values.
		@see Min()
		@see KeepWithin()
		*/
		const real Max(const real lhs, const real rhs);

		/** Transforms a value from one range to the 0-1 range.
		This function does not limmit the value, so if it's below the lower bound it will be below 0.
		@param L the lower bound of the range
		@param v the value
		@param U the upper bound of the range
		@returns The tranformed value.
		@see From01()
		@see Remap()
		@see KeepWithin()
		*/
		const real To01(const real L, const real v, const real U);

		/** Transforms a value from the 0-1 range to another range.
		This function does not limmit he value, so if it's below 0 the result will be below the lower bound.
		@param L the lower bound of the range
		@param v the value
		@param U the upper bound of the range
		@returns The tranformed value
		@see To01()
		@see KeepWithin()
		@see Remap()
		*/
		const real From01(const real L, const real v, const real U);

		/** Remaps/tranforms from one range to another.
		This function does not limmit it's input, so if the value is outside the original range, it will be outside the new range.
		@param ol old lower range
		@param ou old upper range
		@param v the value
		@param nl new lower range
		@param nu new upper range
		@returns The transformed value
		@see From01()
		@see To01()
		@see KeepWithin()
		*/
		const real Remap(const real ol, const real ou, const real v, const real nl, const real nu);

		/** Calculate smooth value suitable for infinite looping.
		@param min is the minimum output value
		@param value is a real that indicates what to return
		             0 means minimum, 0.5 means maximum and 1 is mimimum again, the values inbetween follow a sinus/cosinus curve doing a full 360.
		@param max is the maximum output value
		@returns a value that lies between min and max
		*/
		const real Get360Angular(const real min, const real value, const real max);

		/** Keeps a value within a minimum and a maximum.
		Limmits it to the range if it get's outside.
		@param min is the minimum value
		@param v is the value to limmit
		@param max is the maximum value
		@returns the limmited value
		@see Min()
		@see Max()
		@see Wrap()
		@see IsWithin()
		*/
		const real KeepWithin(const real min, const real v, const real max);

		/** Checks wheter a value is within a range.
		@see KeepWithin()
		*/
		const bool IsWithin(const real min, const real c, const real max);

		/** Checks wheter a value is within a range, beeing at the border is ok.
		@see KeepWithin()
		*/
		const bool IsWithinInclusive(const real min, const real c, const real max);

		/** Keeps a value within a range.
		Wraps it to the others side if it gets outside.
		@param min is the lower range
		@param v is the value to be wrapped
		@param max is the upper range
		@returns the wrapped value
		@see KeepWithin()
		@see Wrapi()
		@see IncrementAndWrap()
		*/
		const real Wrap(const real min, const real v, const real max);

		/** Keeps a value within a range.
		Wraps it to the others side if it gets outside.
		@param min is the lower range
		@param v is the value to be wrapped
		@param max is the upper range
		@returns the wrapped value
		@see KeepWithin()
		@see Wrap()
		@see IncrementAndWrapi()
		*/
		const int Wrapi(const int min, const int v, const int max);

		/** rounds a value to the nearest nice value.
		If the granularity is 1 the function rounds to the closest integer, at .5 the closest half integer, at 2 even integers etc...
		@param num the number to round
		@param gran the granularity
		*/
		const real Round(const real num, const real gran);

		/** Increment and wrap a value.
		@returns return number of wraps (with sign)
		@see Wrap()
		*/
		const int IncrementAndWrap(const real min, real* current, const real change, const real max);

		/** Increment and wrap a value.
		@returns return number of wraps (with sign)
		@see Wrapi()
		*/
		const int IncrementAndWrapi(const int min, int* current, const int change, const int max);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup constants Constants
		/// @{

		/** The matematical constant pi.
		@see HalfPi()
		*/
		const real Pi();

		/** The matematical constant pi divided by 2.
		@see Pi()
		*/
		const real HalfPi();

		/** The boolean value true.
		@see kFalse()
		*/
		const bool kTrue();

		/** The boolean value false.
		@see kTrue()
		*/
		const bool kFalse();

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup vec2 vec2 operations
		/// @{
		const vec2 GetNormalized(const vec2& vec);

		/** Generates a direction given two positions.
		@returns a vector pointing from to to.
		*/
		const vec2 FromTo(const vec2& from, const vec2& to);

		/** Performs a single interpolating step to transform a old vector to a new vector.
		Larger smoothing values result in a smoother but less resposive path

		Example:
		@code mousex = Curve( ( MousePos - OldMousePos ) * Sensitivity(), mousex, 6 ); @endcode

		@param target is the target value
		@param current is the current value
		@param smoothing is the smoothing value > 0, the lower, the less smooth

		@returns The smoothed result.
		*/
		const vec2 Curve(const vec2& target, const vec2& current, real smoothing);

		/** util chain-function for changing the y value of a vector.
		Useful for flattening the In-vector
		@returns the changed vector
		*/
		const vec2 ChangeY(const vec2& v, const real newy);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup vec3 vec3 operations
		/// @{

		/** simple ignore-w constructor.
		drops the w value from a quaternion.
		*/
		const vec3 cvec3(const quat& q);

		/** gets the transformation of the matrix
		*/
		const vec3 cvec3(const mat44& m);

		/** normalizes a vector and returns it.
		*/
		const vec3 GetNormalized(const vec3& vec);

		/** computes the cross-product and then normalizes the result.
		@see GetNormalized(const vec3&)
		*/
		const vec3 crossNorm(const vec3& lhs, const vec3& rhs);

		/** Performs a single interpolating step to transform a old vector to a new vector.
		Larger smoothing values result in a smoother but less resposive path

		@param target is the target value
		@param current is the current value
		@param smoothing is the smoothing value > 0, the lower, the less smooth

		@returns The smoothed result.
		*/
		vec3 Curve(const vec3& target, const vec3& current, real smoothing);

		/** Generates a direction given two positions.
		@returns a vector pointing from to to.
		*/
		const vec3 FromTo(const vec3& from, const vec3& to);

		/** linear interpolation between two vectors.
		*/
		const vec3 Lerp(const vec3& f, real scale, const vec3& t);

		/** quadratic interpolation between two vectors.
		*/
		const vec3 Qlerp(const vec3& f, real scale, const vec3& t);

		/** gets the angle between two vectors
		only able to get 0-180 degrees
		@see AngleBetween(const vec3&, const vec3&, const vec3&)
		*/
		const Angle AngleBetween(const vec3& a, const vec3& b);

		/** gets the angle between two vectors
		This function is able to get a full 0-360 estimate.
		For the result of this function to be valid, a 3 vectors should be on the same plane - this is not something that is checked (atm).
		Assuming all parameters are correct, if b=perpa the return-value is 90, and if b=-perpa the return-value is 270
		@param a is the first vector
		@param b is the second vector
		@param perpa is a vector perpendicular to a, this is not checked
		@see AngleBetween(const vec3&, const vec3&)
		*/
		const Angle AngleBetween(const vec3& a, const vec3& b, const vec3& perpa);

		/** Unit-vector pointing in the direction of negative Z, (0,0,-1).
		*/
		const vec3 In();

		/** Unit-vector pointing in the direction of positive X, (1,0,0).
		*/
		const vec3 Right();

		/** Unit-vector pointing in the direction of positive Y, (0,1,0)
		*/
		const vec3 Up();

		/** Unit-vector pointing in the direction of positive Z, (0,0,1).
		*/
		const vec3 Out();

		/** Unit-vector pointing in the direction of negaitve X, (-1,0,0)
		*/
		const vec3 Left();

		/** Unit-vector pointing in the direction of negative Y, (0,-1,0)
		*/
		const vec3 Down();

		/** Compresses a unit-vector.
		@see CompressedToUnitVector(uing16)
		*/
		const uint16 UnitVectorToCompressed(const vec3& vec);

		/** Decrompress a compressed unit-vector.
		@see UnitVectorToCompressed(vec3)
		*/
		const vec3 CompressedToUnitVector(uint16 compressed);

		const vec3 cvec3(const vec4& v);
		const vec4 cvec4(const vec3& v);
		const vec4 cvec4(const vec3& v, real w);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup mat33 mat33 operations
		/// @{

		/** Unit-vector pointing in the direction of negative Z, (0,0,-1) in matrix-space.
		*/
		const vec3 In(const mat33& m);

		/** Unit-vector pointing in the direction of positive X, (1,0,0) in matrix-space.
		*/
		const vec3 Right(const mat33& m);

		/** Unit-vector pointing in the direction of positive Y, (0,1,0) in matrix-space.
		*/
		const vec3 Up(const mat33& m);

		/** Unit-vector pointing in the direction of positive Z, (0,0,1) in matrix-space.
		*/
		const vec3 Out(const mat33& m);

		/** Unit-vector pointing in the direction of negaitve X, (-1,0,0) in matrix-space.
		*/
		const vec3 Left(const mat33& m);

		/** Unit-vector pointing in the direction of negative Y, (0,-1,0) in matrix-space.
		*/
		const vec3 Down(const mat33& m);

		/** creates a matrix from the col-major argument.
		*/
		const mat33 mat33_FromColMajor(const real data0, const real data1, const real data2,
										const real data3, const real data4, const real data5,
										const real data6, const real data7, const real data8);

		/** Scaling matrix.
		*/
		const mat33 Scale(const vec3& scale);

		/** The identity matrix.
		@see mat44Identity()
		@see qIdentity()
		*/
		const mat33 mat33Identity();

		/** takes a 4x4 matrix and ignores the 4th row and the 4th column.
		*/
		const mat33 cmat33(const mat44& m);
		const mat33 cmat33(const quat& q);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup mat44 mat44 operations
		/// @{

		/** Unit-vector pointing in the direction of negative Z, (0,0,-1) in matrix-space.
		*/
		const vec3 In(const mat44& m);

		/** Unit-vector pointing in the direction of positive X, (1,0,0) in matrix-space.
		*/
		const vec3 Right(const mat44& m);

		/** Unit-vector pointing in the direction of positive Y, (0,1,0) in matrix-space.
		*/
		const vec3 Up(const mat44& m);

		/** Unit-vector pointing in the direction of positive Z, (0,0,1) in matrix-space.
		*/
		const vec3 Out(const mat44& m);

		/** Unit-vector pointing in the direction of negaitve X, (-1,0,0) in matrix-space.
		*/
		const vec3 Left(const mat44& m);

		/** Unit-vector pointing in the direction of negative Y, (0,-1,0) in matrix-space.
		*/
		const vec3 Down(const mat44& m);

		/** creates a matrix from the row-major argument.
		*/
		const mat44 mat44_FromColMajor(
									const real data0,  const real data1,  const real data2,  const real data3,
									const real data4,  const real data5,  const real data6,  const real data7,
									const real data8,  const real data9,  const real data10, const real data11,
									const real data12, const real data13, const real data14, const real data15 );

		/** matrix-multiplication.
		*/
		const mat44 operator* (const mat44& a, const mat44& b);

		/** matrix-vector multiplication.
		the "4th" element on the vector is ignored.
		*/
		const vec3 operator *(const mat44& m, const vec3& v);

		/** axis-angle rotation.
		*/
		const mat44 cmat44(const AxisAngle& aa);

		/** quaternion rotation.
		*/
		const mat44 cmat44(const quat& q);

		const mat44 SetTransform(const mat44& m, const vec3& t);

		/** identity matrix
		*/
		const mat44 mat44Identity();

		/** translation-matrix.
		*/
		const mat44 cmat44(const vec3& v);

		/** fills out the missing items with values from the identy-matrix
		*/
		const mat44 cmat44(const mat33& m);

		vec3 RotateWithInverseMatrix(const vec3 vec, const mat44& mat);
		vec3 TranslateWithInverseMatrix(const vec3 vec, const mat44& mat);

		/** Syntax-sugar function-chainer helper-class for most common mat44 operations.
		Example:
		@code newMatrix = mat44helper(mat44Identity()).rotate(roation).translate(translation).mat; @endcode
		*/
		struct mat44helper
		{
			/** copy and store the starting matrix.
			*/
			mat44helper(const mat44& mat);

			/** multiply with a custom matrix.
			*/
			mat44helper& mult(const mat44& m);

			/** multiply with a rotation matrix.
			*/
			mat44helper& rotate(const AxisAngle& aa);

			/** multiply with a rotation matrix.
			*/
			mat44helper& rotate(const quat& q);

			/** multiply with a translation matrix
			*/
			mat44helper& translate(const vec3& t);

			/** transform a vector with the current matrix.
			*/
			const vec3 transform(const vec3& v) const;

			/** the current matrix
			*/
			mat44 mat;
		};

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup quat quat operations
		/// @{

		const quat quatw(const real aw, const real ax, const real ay, const real az);

		/** construct from a axis and a angle.
		*/
		const quat cquat(const AxisAngle& aa);

		const quat GetConjugate(const quat& q);

		/** Linear interpolation.
		*/
		const quat Lerp(const quat& f, const real scale, const quat& t);

		/** Quadratic interpolation.
		*/
		const quat Qlerp(const quat& f, const real scale, const quat& t);

		/** Spherical-linear interpolation, may not go the shortest way.
		@see SlerpShortway(const quat&, const real, const quat&)
		*/
		const quat Slerp(const quat& f, const real scale, const quat& t);

		/** Spherical linear interpolation that is guarantued to go the shortest way.
		@see Slerp(const quat&, const real, const quat&)
		*/
		const quat SlerpShortway(const quat& f, const real scale, const quat& t);

		/** returns a normalized version of the quaternion.
		@see Normalize(quat*)
		*/
		const quat GetNormalized(const quat& q);

		/** Unit-vector pointing in the direction of negative Z, (0,0,-1) in quaternion-space.
		*/
		const vec3 In(const quat& q);

		/** Unit-vector pointing in the direction of positive X, (1,0,0) in quaternion-space.
		*/
		const vec3 Right(const quat& q);

		/** Unit-vector pointing in the direction of positive Y, (0,1,0) in quaternion-space.
		*/
		const vec3 Up(const quat& q);

		/** Unit-vector pointing in the direction of positive Z, (0,0,1) in quaternion-space.
		*/
		const vec3 Out(const quat& q);

		/** Unit-vector pointing in the direction of negaitve X, (-1,0,0) in quaternion-space.
		*/
		const vec3 Left(const quat& q);

		/** Unit-vector pointing in the direction of negative Y, (0,-1,0) in quaternion-space.
		*/
		const vec3 Down(const quat& q);

		/** Util function form muliplying a vector with In, Right and Up.
		@see In(const quat&)
		@see Right(const quat&)
		@see Up(const quat&)
		@returns In*Z + Right*X + Up*Y
		*/
		const vec3 RightUpIn(const quat& q, const vec3& v);

		/** Rotates a vector around origo.
		*/
		const vec3 RotateAroundOrigo(const quat& q, const vec3& v);

		/** returns the combined rotations
		*/
		const quat Combine(const quat& current, const quat& extra);

		//const quat cquat(const Euler& e); // todo: add construct from angle function?

		/** The identity quaternion.
		*/
		const quat qIdentity();

		/** create a look-at quaternion.
		@see qLookInDirection(const vec3&, const vec3&)
		*/
		const quat qLookAt(const vec3& from, const vec3& to, const vec3& up);

		/** create a look-at quaternion or qIdentity if no directoion could be generated.
		@see qLookInDirection(const vec3&, const vec3&)
		@see qIdentity()
		*/
		const quat qLookAtOrNot(const vec3& from, const vec3& to, const vec3& up);

		/** creates a look-at quaternion.
		@see qLookAt(const vec3&, const vec3&, const vec3&)
		*/
		const quat qLookInDirection(const vec3& adir, const vec3& up);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup point2 point2 operations
		/// @{

		/** origo.
		*/
		const point2 Origo2();

		/** gets the distance between 2 points.
		*/
		const real DistanceBetween(const point2& from, const point2& to);

		const point2 ChangeY(const point2& v, const real newy);

		/** point/direction addition.
		*/
		const point2 operator+(const point2& lhs, const direction2& rhs);

		/** point/direction addition.
		*/
		const point2 operator+(const direction2& lhs, const point2& rhs);

		/** point/direction subtraction.
		*/
		const point2 operator-(const point2& lhs, const direction2& rhs);

		/** point/direction subtraction.
		*/
		const point2 operator-(const direction2& lhs, const point2& rhs);

		/** uniform scaling.
		*/
		const point2 operator*(const point2& lhs, const real rhs);

		/** uniform scaling.
		*/
		const point2 operator*(const real lhs, const point2& rhs);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup direction2 direction2 operations
		/// @{

		/** The length.
		*/
		const real LengthOf(const direction2& dir);

		/** convert the direction to a unit vector
		*/
		const direction2 ToUnit(const direction2& dir);

		/** create a direction from one point to another.
		*/
		const direction2 FromTo(const point2& from, const point2& to);

		/** direction addition.
		*/
		const direction2 operator+(const direction2& lhs, const direction2& rhs);

		/** direction subtraction.
		*/
		const direction2 operator-(const direction2& lhs, const direction2& rhs);

		/** direction scaling.
		*/
		const direction2 operator*(const direction2& lhs, const real rhs);

		/** direction scaling.
		*/
		const direction2 operator*(const real lhs, const direction2& rhs);

		/** direction scaling.
		*/
		const direction2 operator/(const direction2& lhs, const real rhs);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup point3 point3 operations
		/// @{

		/** origo.
		*/
		const point3 Origo3();

		/** gets the distance between 2 points.
		*/
		const real DistanceBetween(const point3& from, const point3& to);

		/** point/direction addition.
		*/
		const point3 operator+(const point3& lhs, const direction3& rhs);

		/** point/direction addition.
		*/
		const point3 operator+(const direction3& lhs, const point3& rhs);

		/** point/direction subtraction.
		*/
		const point3 operator-(const point3& lhs, const direction3& rhs);

		/** point/direction subtraction.
		*/
		const point3 operator-(const direction3& lhs, const point3& rhs);

		/** uniform scaling.
		*/
		const point3 operator*(const point3& lhs, const real rhs);

		/** uniform scaling.
		*/
		const point3 operator*(const real lhs, const point3& rhs);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup direction3 direction3 operations
		/// @{

		/** The length.
		*/
		const real LengthOf(const direction3& dir);

		/** convert the direction to a unit vector
		*/
		const direction3 ToUnit(const direction3& dir);

		/** create a direction from one point to another.
		*/
		const direction3 FromTo(const point3& from, const point3& to);

		/** direction addition.
		*/
		const direction3 operator+(const direction3& lhs, const direction3& rhs);

		/** direction subtraction.
		*/
		const direction3 operator-(const direction3& lhs, const direction3& rhs);

		/** direction scaling.
		*/
		const direction3 operator*(const direction3& lhs, const real rhs);

		/** direction scaling.
		*/
		const direction3 operator*(const real lhs, const direction3& rhs);

		/** direction scaling.
		*/
		const direction3 operator/(const direction3& lhs, const real rhs);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup rect rect operations
		/// @{

		/** the top of the rectangle.
		*/
		real TopOf(const rect& r);

		/** The bottom of the rectangle.
		*/
		real BottomOf(const rect& r);

		/** The left side of the rectangle.
		*/
		real LeftOf(const rect& r);

		/** The right side of the rectangle.
		*/
		real RightOf(const rect& r);

		/** The width of the rectangle.
		*/
		real WidthOf(const rect& r);

		/** The height of the rectangle.
		*/
		real HeightOf(const rect& r);

		/** The aspect ratio of the rectangle.
		i.e width/height
		*/
		real AspectOf(const rect& r);

		/** creates a centered rect filling the whole containing-rect, with a aspect
		*/
		const rect FromAspectAndContainingInCenter(const rect& containing, real aspect);

		/** creates a rect from upper-left-corner and the size.
		*/
		const rect FromUpperLeftAndSize(const point2& point, const direction2& size);

		/** creates a rect from upper-left-corner and the size.
		*/
		const rect FromUpperLeftAndSize(const point2& point, const real width, const real height);

		/** creates a rect from upper-left-corner and the size.
		*/
		const rect FromUpperLeftAndSize(const point2& point, const real size);

		/** creates a rect from upper-left-corner and the lower right corner
		*/
		const rect FromUpperLeftAndLowerRight(const point2& ul, const point2& lr);

		/** from left, right, up and down.
		*/
		const rect FromLrud(const real left, const real right, const real up, const real down);

		/** constructs a rectangle from the size and it's center.
		@param size is the size of the rect
		@param center is the percentage expressed as 0-1. (0,0) is bottom left and (1,1) is top right.
		*/
		const rect FromSizeAndCenter(const direction2& size, const point2& center);

		/** constructs a rectangle from the size and it's center.
		@param size is the size of the rect
		@param center is the percentage expressed as 0-1. (0,0) is bottom left and (1,1) is top right.
		*/
		const rect FromSizeAndCenter(const real size, const point2& center);


		/** moves a rectangle to a new position, withour altering the source.
		*/
		const rect MoveCopyTo(const rect& r, const point2& point);

		/** flips the rectangle upside-down, without altering the source
		*/
		const rect TurnCopyUpsideDown(const rect& r, const real scale);

		/** transforms a point from one source to another.
		*/
		const point2 Remap(const rect& from, const point2& p, const rect& to);

		// todo: WrapWithin?

		/** keeps a point within a rectangle.
		*/
		const point2 KeepWithin(const point2& loc, const rect& region);

		/** keeps a point within a rectangle, also including a rextent (such as a player or a cursor).
		*/
		const point2 KeepWithin(const point2& loc, const rect& region, const rect& extent);

		/** determines if a point is within a rectangle.
		*/
		const bool IsWithin(const point2& loc, const rect& region);

		/** move a rectangle.
		*/
		const rect operator+(const rect& lhs, const direction2& rhs);

		/** move a rectangle.
		*/
		const rect operator+(const direction2& lhs, const rect& rhs);

		/** scale a rectangle.
		*/
		const rect operator*(const rect& r, const real scale);

		/** scale a rectangle.
		*/
		const rect operator*(const real scale, const rect& r);

		// todo: IsWithin/DoesOverlap operations for rect-rect operations?

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup Angle Angle operations
		/// @{

		/** The mathemathical function Sine.
		*/
		const real Sin(const Angle& ang);

		/** The mathemathical function Cosine.
		*/
		const real Cos(const Angle& ang);

		/** The mathemathical function Tangent.
		*/
		const real Tan(const Angle& ang);

		/** The mathemathical function Arc-sine.
		*/
		const Angle Asin(const real v);

		/** The mathemathical function Arc-cosine.
		*/
		const Angle Acos(const real v);

		/** The mathemathical function Arc-tangent.
		*/
		const Angle Atan(const real v);

		/** wraps the angle around 360 degrees.
		@see Angle::wrap()
		*/
		const Angle GetWrapped(const Angle& a);

		/** angle addition.
		*/
		const Angle operator+(const Angle& lhs, const Angle& rhs);

		/** angle subtraction.
		*/
		const Angle operator-(const Angle& lhs, const Angle& rhs);

		/** angle multiplication.
		*/
		const Angle operator*(const Angle& lhs, const real rhs);

		/** angle multiplication.
		*/
		const Angle operator*(const real rhs, const Angle& lhs);

		/** cretes a angle where 1 is 360 degrees and 0 is 0 degrees.
		*/
		const Angle FromPercentOf360(const real percent);

		/** zero degrees.
		*/
		const Angle Zero();

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup AxisAngle AxisAngle operations
		/// @{

		/** creates a axis-angle according to the right-hand rule.
		*/
		const AxisAngle RightHandAround(const vec3& axis, const Angle& angle);

		/** creates a axis-angle from a quaternion.
		*/
		const AxisAngle cAxisAngle(const quat& q);

		/// @}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \defgroup ArcBall ArcBall operations
		/// @{

		/** get the rotation that is done, by moving from one point to another.
		*/
		// quat GetRotation(const ArcBall& arc, const vec2 from, const vec2 to);

		/// @}

		template<class Stream>
		Stream& operator<<(Stream& s, const Angle& a)
		{
			s << a.inDegrees() << "°";
			return s;
		}

		template<class Stream>
		Stream& operator<<(Stream& s, const AxisAngle& aa)
		{
			s << "(" << aa.angle << " around " << aa.axis << ")";
			return s;
		}
	}
}

#endif
