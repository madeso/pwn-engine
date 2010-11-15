#ifndef PWN_RENDER_RGB
#define PWN_RENDER_RGB

#include <pwn/number.h>

namespace pwn
{
	namespace math
	{
		namespace Color
		{
			// from http://www.uize.com/examples/sortable-color-table.html
			enum Type
			{
				White,
				Silver,
				Gray,
				Black,
				Navy,
				Blue,
				Aqua,
				Teal,
				Green,
				Olive,
				Lime,
				Maroon,
				Red,
				Orange,
				Yellow,
				Purple,
				Fuchsia,
				AliceBlue,
				AntiqueWhite,
				AquaMarine,
				Azure,
				Beige,
				Bisque,
				BlanchedAlmond,
				BlueViolet,
				Brown,
				BurlyWood,
				CadetBlue,
				Chartreuse,
				Chocolate,
				Coral,
				CornflowerBlue,
				Cornsilk,
				Crimson,
				Cyan,
				Darkblue,
				Darkcyan,
				DarkGoldenRod,
				Darkgray,
				Darkgreen,
				DarKkhaki,
				DarkMagenta,
				DarkOliveGreen,
				DarkOrange,
				DarkOrchid,
				DarkRed,
				DarkSalmon,
				DarkSeaGreen,
				DarkslateBlue,
				DarkslateGray,
				DarkTurquoise,
				DarkBiolet,
				DeeppInk,
				DeepskyBlue,
				DimGray,
				DodgerBlue,
				FireBrick,
				FloralWhite,
				ForestGreen,
				Gainsboro,
				GhostWhite,
				Gold,
				GoldenRod,
				GreenYellow,
				Honeydew,
				Hotpink,
				IndianRed,
				Indigo,
				Ivory,
				Khaki,
				Lavender,
				LavenderBlush,
				Lawngreen,
				LemonChiffon,
				LightBlue,
				LightCoral,
				LightCyan,
				LightGoldenRodYellow,
				LightGray,
				LightGreen,
				LightPink,
				LightSalmon,
				LightSeaGreen,
				LightskyBlue,
				LightslateGray,
				LightSteelBlue,
				LightYellow,
				LimeGreen,
				Linen,
				Magenta,
				MediumAquaMarine,
				MediumBlue,
				MediumOrchid,
				MediumPurple,
				MediumSeaGreen,
				MediumslateBlue,
				MediumSpringGreen,
				MediumTurquoise,
				MediumVioletRed,
				MidnightBlue,
				Mintcream,
				Mistyrose,
				Moccasin,
				NavajoWhite,
				Oldlace,
				OliveDrab,
				OrangeRed,
				Orchid,
				PaleGoldenRod,
				PaleGreen,
				PaleTurquoise,
				PaleVioletRed,
				Papayawhip,
				Peachpuff,
				Peru,
				Pink,
				Plum,
				PowderBlue,
				Rosybrown,
				Royalblue,
				SaddleBrown,
				Salmon,
				SandyBrown,
				Seagreen,
				Seashell,
				Sienna,
				SkyBlue,
				SlateBlue,
				SlateGray,
				Snow,
				SpringGreen,
				SteelBlue,
				Tan,
				Thistle,
				Tomato,
				Turquoise,
				Violet,
				Wheat,
				WhiteSmoke,
				YellowGreen
			};
		}

		class Rgb
		{
		public:
			Rgb(const real r, const real g, const real b);
			explicit Rgb(const real gray);
			explicit Rgb(const int rgb);
			static const Rgb From(Color::Type color);

			const real red() const;
			const real green() const;
			const real blue() const;

			//const real* data() const;

			void red(const real v);
			void green(const real v);
			void blue(const real v);

			//real& operator[](int index);
			//const real& operator[](int index) const;

			void rgb(const real r, const real g, const real b);
			void rgb(int rgb);
		public:
			real r;
			real g;
			real b;
		};
	}
}

#endif