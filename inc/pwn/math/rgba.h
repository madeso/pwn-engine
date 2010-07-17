#ifndef PWN_RENDER_RGBA
#define PWN_RENDER_RGBA

#include <pwn/math/rgb>

namespace pwn
{
	namespace math
	{
		class Rgba
			: public Rgb
		{
		public:
			Rgba(const real r, const real g, const real b, const real a);
			Rgba(const real r, const real g, const real b);
			explicit Rgba(const real gray);
			explicit Rgba(const int rgb);
			Rgba(Color::Type color);

			const real alpha() const;
			void alpha(const real v);

			//real& operator[](int index);
			//const real& operator[](int index) const;

			void rgba(const real r, const real g, const real b, const real a);
			void rgb(const real r, const real g, const real b);
			void rgb(int rgb);
			void rgb(Color::Type color);
		public:
			real a;
		};
	}
}

#endif