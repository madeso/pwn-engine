#include <pwn/math/rgba>

namespace pwn
{
	namespace math
	{
		Rgba::Rgba(const real r, const real g, const real b, const real a)
		{
			rgba(r, g, b, a);
		}

		Rgba::Rgba(const real r, const real g, const real b)
		{
			rgb(r, g, b);
		}

		Rgba::Rgba(const real gray)
		{
			rgb(gray, gray, gray);
		}

		const real Rgba::red() const
		{
			return mdata.component.red;
		}
		const real Rgba::green() const
		{
			return mdata.component.green;
		}
		const real Rgba::blue() const
		{
			return mdata.component.blue;
		}
		const real Rgba::alpha() const
		{
			return mdata.component.alpha;
		}

		const real* Rgba::data() const
		{
			return mdata.array;
		}

		void Rgba::red(const real v)
		{
			mdata.component.red = v;
		}

		void Rgba::green(const real v)
		{
			mdata.component.green = v;
		}
		void Rgba::blue(const real v)
		{
			mdata.component.blue = v;
		}
		void Rgba::alpha(const real v)
		{
			mdata.component.alpha = v;
		}

		real& Rgba::operator[](int index)
		{
			return mdata.array[index];
		}

		const real& Rgba::operator[](int index) const
		{
			return mdata.array[index];
		}

		void Rgba::rgba(const real r, const real g, const real b, const real a)
		{
			mdata.component.red = r;
			mdata.component.green = g;
			mdata.component.blue = b;
			mdata.component.alpha = a;
		}

		void Rgba::rgb(const real r, const real g, const real b)
		{
			rgba(r, g, b, 1);
		}

		namespace // local
		{
			const real GetComponent(int i, int steps)
			{
				const int value = ((i >> 4*steps) & 0xff);
				return value / 255.0f;
			}
		}

		void Rgba::rgb(int rgb)
		{
			const real b = GetComponent(rgb, 0);
			const real g = GetComponent(rgb, 1);
			const real r = GetComponent(rgb, 2);
			this->rgb(r, g, b);
		}

		Rgba::Rgba(Color::Type color)
		{
			using namespace Color;

			switch(color)
			{
			case      White:                rgb(0xffffff);              break;
			case      Silver:               rgb(0xc0c0c0);              break;
			case      Gray:                 rgb(0x808080);              break;
			case      Black:                rgb(0x000000);              break;
			case      Navy:                 rgb(0x000080);              break;
			case      Blue:                 rgb(0x0000ff);              break;
			case      Aqua:                 rgb(0x00ffff);              break;
			case      Teal:                 rgb(0x008080);              break;
			case      Green:                rgb(0x008000);              break;
			case      Olive:                rgb(0x808000);              break;
			case      Lime:                 rgb(0x00ff00);              break;
			case      Maroon:               rgb(0x800000);              break;
			case      Red:                  rgb(0xff0000);              break;
			case      Orange:               rgb(0xffa500);              break;
			case      Yellow:               rgb(0xffff00);              break;
			case      Purple:               rgb(0x800080);              break;
			case      Fuchsia:              rgb(0xff00ff);              break;
			case      AliceBlue:            rgb(0xf0f8ff);              break;
			case      AntiqueWhite:         rgb(0xfaebd7);              break;
			case      AquaMarine:           rgb(0x7fffd4);              break;
			case      Azure:                rgb(0xf0ffff);              break;
			case      Beige:                rgb(0xf5f5dc);              break;
			case      Bisque:               rgb(0xffe4c4);              break;
			case      BlanchedAlmond:       rgb(0xffebcd);              break;
			case      BlueViolet:           rgb(0x8a2be2);              break;
			case      Brown:                rgb(0xa52a2a);              break;
			case      BurlyWood:            rgb(0xdeb887);              break;
			case      CadetBlue:            rgb(0x5f9ea0);              break;
			case      Chartreuse:           rgb(0x7fff00);              break;
			case      Chocolate:            rgb(0xd2691e);              break;
			case      Coral:                rgb(0xff7f50);              break;
			case      CornflowerBlue:       rgb(0x6495ed);              break;
			case      Cornsilk:             rgb(0xfff8dc);              break;
			case      Crimson:              rgb(0xdc143c);              break;
			case      Cyan:                 rgb(0x00ffff);              break;
			case      Darkblue:             rgb(0x00008b);              break;
			case      Darkcyan:             rgb(0x008b8b);              break;
			case      DarkGoldenRod:        rgb(0xb8860b);              break;
			case      Darkgray:             rgb(0xa9a9a9);              break;
			case      Darkgreen:            rgb(0x006400);              break;
			case      DarKkhaki:            rgb(0xbdb76b);              break;
			case      DarkMagenta:          rgb(0x8b008b);              break;
			case      DarkOliveGreen:       rgb(0x556b2f);              break;
			case      DarkOrange:           rgb(0xff8c00);              break;
			case      DarkOrchid:           rgb(0x9932cc);              break;
			case      DarkRed:              rgb(0x8b0000);              break;
			case      DarkSalmon:           rgb(0xe9967a);              break;
			case      DarkSeaGreen:         rgb(0x8fbc8f);              break;
			case      DarkslateBlue:        rgb(0x483d8b);              break;
			case      DarkslateGray:        rgb(0x2f4f4f);              break;
			case      DarkTurquoise:        rgb(0x00ced1);              break;
			case      DarkBiolet:           rgb(0x9400d3);              break;
			case      DeeppInk:             rgb(0xff1493);              break;
			case      DeepskyBlue:          rgb(0x00bfff);              break;
			case      DimGray:              rgb(0x696969);              break;
			case      DodgerBlue:           rgb(0x1e90ff);              break;
			case      FireBrick:            rgb(0xb22222);              break;
			case      FloralWhite:          rgb(0xfffaf0);              break;
			case      ForestGreen:          rgb(0x228b22);              break;
			case      Gainsboro:            rgb(0xdcdcdc);              break;
			case      GhostWhite:           rgb(0xf8f8ff);              break;
			case      Gold:                 rgb(0xffd700);              break;
			case      GoldenRod:            rgb(0xdaa520);              break;
			case      GreenYellow:          rgb(0xadff2f);              break;
			case      Honeydew:             rgb(0xf0fff0);              break;
			case      Hotpink:              rgb(0xff69b4);              break;
			case      IndianRed:            rgb(0xcd5c5c);              break;
			case      Indigo:               rgb(0x4b0082);              break;
			case      Ivory:                rgb(0xfffff0);              break;
			case      Khaki:                rgb(0xf0e68c);              break;
			case      Lavender:             rgb(0xe6e6fa);              break;
			case      LavenderBlush:        rgb(0xfff0f5);              break;
			case      Lawngreen:            rgb(0x7cfc00);              break;
			case      LemonChiffon:         rgb(0xfffacd);              break;
			case      LightBlue:            rgb(0xadd8e6);              break;
			case      LightCoral:           rgb(0xf08080);              break;
			case      LightCyan:            rgb(0xe0ffff);              break;
			case      LightGoldenRodYellow: rgb(0xfafad2);              break;
			case      LightGray:            rgb(0xd3d3d3);              break;
			case      LightGreen:           rgb(0x90ee90);              break;
			case      LightPink:            rgb(0xffb6c1);              break;
			case      LightSalmon:          rgb(0xffa07a);              break;
			case      LightSeaGreen:        rgb(0x20b2aa);              break;
			case      LightskyBlue:         rgb(0x87cefa);              break;
			case      LightslateGray:       rgb(0x778899);              break;
			case      LightSteelBlue:       rgb(0xb0c4de);              break;
			case      LightYellow:          rgb(0xffffe0);              break;
			case      LimeGreen:            rgb(0x32cd32);              break;
			case      Linen:                rgb(0xfaf0e6);              break;
			case      Magenta:              rgb(0xff00ff);              break;
			case      MediumAquaMarine:     rgb(0x66cdaa);              break;
			case      MediumBlue:           rgb(0x0000cd);              break;
			case      MediumOrchid:         rgb(0xba55d3);              break;
			case      MediumPurple:         rgb(0x9370db);              break;
			case      MediumSeaGreen:       rgb(0x3cb371);              break;
			case      MediumslateBlue:      rgb(0x7b68ee);              break;
			case      MediumSpringGreen:    rgb(0x00fa9a);              break;
			case      MediumTurquoise:      rgb(0x48d1cc);              break;
			case      MediumVioletRed:      rgb(0xc71585);              break;
			case      MidnightBlue:         rgb(0x191970);              break;
			case      Mintcream:            rgb(0xf5fffa);              break;
			case      Mistyrose:            rgb(0xffe4e1);              break;
			case      Moccasin:             rgb(0xffe4b5);              break;
			case      NavajoWhite:          rgb(0xffdead);              break;
			case      Oldlace:              rgb(0xfdf5e6);              break;
			case      OliveDrab:            rgb(0x6b8e23);              break;
			case      OrangeRed:            rgb(0xff4500);              break;
			case      Orchid:               rgb(0xda70d6);              break;
			case      PaleGoldenRod:        rgb(0xeee8aa);              break;
			case      PaleGreen:            rgb(0x98fb98);              break;
			case      PaleTurquoise:        rgb(0xafeeee);              break;
			case      PaleVioletRed:        rgb(0xdb7093);              break;
			case      Papayawhip:           rgb(0xffefd5);              break;
			case      Peachpuff:            rgb(0xffdab9);              break;
			case      Peru:                 rgb(0xcd853f);              break;
			case      Pink:                 rgb(0xffc0cb);              break;
			case      Plum:                 rgb(0xdda0dd);              break;
			case      PowderBlue:           rgb(0xb0e0e6);              break;
			case      Rosybrown:            rgb(0xbc8f8f);              break;
			case      Royalblue:            rgb(0x4169e1);              break;
			case      SaddleBrown:          rgb(0x8b4513);              break;
			case      Salmon:               rgb(0xfa8072);              break;
			case      SandyBrown:           rgb(0xf4a460);              break;
			case      Seagreen:             rgb(0x2e8b57);              break;
			case      Seashell:             rgb(0xfff5ee);              break;
			case      Sienna:               rgb(0xa0522d);              break;
			case      SkyBlue:              rgb(0x87ceeb);              break;
			case      SlateBlue:            rgb(0x6a5acd);              break;
			case      SlateGray:            rgb(0x708090);              break;
			case      Snow:                 rgb(0xfffafa);              break;
			case      SpringGreen:          rgb(0x00ff7f);              break;
			case      SteelBlue:            rgb(0x4682b4);              break;
			case      Tan:                  rgb(0xd2b48c);              break;
			case      Thistle:              rgb(0xd8bfd8);              break;
			case      Tomato:               rgb(0xff6347);              break;
			case      Turquoise:            rgb(0x40e0d0);              break;
			case      Violet:               rgb(0xee82ee);              break;
			case      Wheat:                rgb(0xf5deb3);              break;
			case      WhiteSmoke:           rgb(0xf5f5f5);              break;
			case      YellowGreen:          rgb(0x9acd32);              break;
			default:                       throw "invalid color name"; break;
			}
		}
	}
}