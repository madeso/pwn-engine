#ifndef PWN_RENDER_RGBA
#define PWN_RENDER_RGBA

#include <pwn/math/rgb.h>

namespace pwn
{
	namespace math
	{
		class Rgb;

		class Rgba
		{
		public:
			Rgba(const Rgb& rgb, const real a);
			Rgba(const real r, const real g, const real b, const real a);
			Rgba(const real r, const real g, const real b);
			explicit Rgba(const real gray);
			const real* data() const;

			const real red() const;
			const real green() const;
			const real blue() const;
			const real alpha() const;

			void red(const real v);
			void green(const real v);
			void blue(const real v);
			void alpha(const real v);

			void rgb(const real r, const real g, const real b);
		public:
			real r;
			real g;
			real b;
			real a;
		};
	}
}

#endif
