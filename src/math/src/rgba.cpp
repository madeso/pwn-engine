#include <pwn/math/rgba.h>

namespace pwn
{
	namespace math
	{
		Rgba::Rgba(const real ar, const real ag, const real ab, const real aa)
			: r(ar), g(ag), b(ab), a(aa)
		{
		}

		Rgba::Rgba(const real ar, const real ag, const real ab)
			: r(ar), g(ag), b(ab), a(1)
		{
		}

		Rgba::Rgba(const real gray)
			: r(gray), g(gray), b(gray), a(1)
		{
		}

		const real* Rgba::data() const
		{
			return &r;
		}

		const real Rgba::red() const
		{
			return r;
		}
		const real Rgba::green() const
		{
			return g;
		}
		const real Rgba::blue() const
		{
			return b;
		}
		const real Rgba::alpha() const
		{
			return a;
		}

		void Rgba::red(const real v)
		{
			r = v;
		}
		void Rgba::green(const real v)
		{
			g = v;
		}
		void Rgba::blue(const real v)
		{
			b = v;
		}
		void Rgba::alpha(const real v)
		{
			a = v;
		}

		void Rgba::rgb(const real ar, const real ag, const real ab)
		{
			red(ar);
			green(ag);
			blue(ab);
			alpha(1);
		}
	}
}
