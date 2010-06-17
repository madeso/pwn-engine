#include <pwn/math/rgba>

namespace pwn
{
	namespace math
	{
		Rgba::Rgba(const real r, const real g, const real b, const real a)
			: Rgb(r, g, b)
		{
			alpha(a);
		}

		Rgba::Rgba(const real r, const real g, const real b)
			: Rgb(r, g ,b)
		{
			alpha(1);
		}

		Rgba::Rgba(const real gray)
			: Rgb(gray)
		{
			alpha(1);
		}

		Rgba::Rgba(const int rgb)
			: Rgb(rgb)
		{
			alpha(1);
		}

		const real Rgba::alpha() const
		{
			return a;
		}

		/*const real* Rgba::data() const
		{
			return mdata.array;
		}*/
		
		void Rgba::alpha(const real v)
		{
			a = v;
		}

		/*real& Rgba::operator[](int index)
		{
			return mdata.array[index];
		}

		const real& Rgba::operator[](int index) const
		{
			return mdata.array[index];
		}*/

		void Rgba::rgba(const real r, const real g, const real b, const real a)
		{
			Rgb::rgb(r, g, b);
			alpha(a);
		}

		void Rgba::rgb(const real r, const real g, const real b)
		{
			rgba(r, g, b, 1);
		}

		void Rgba::rgb(int rgb)
		{
			Rgb::rgb(rgb);
			alpha(1);
		}
	}
}