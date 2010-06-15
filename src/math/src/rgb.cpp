#include <pwn/math/rgb>

namespace pwn
{
	namespace math
	{
		Rgb::Rgb(const real red, const real green, const real blue)
		{
			rgb(red, green, blue);
		}

		Rgb::Rgb(const real gray)
		{
			rgb(gray, gray, gray);
		}
		Rgb::Rgb(const int all)
		{
			rgb(all);
		}

		const real Rgb::red() const
		{
			return r;
		}
		const real Rgb::green() const
		{
			return g;
		}
		const real Rgb::blue() const
		{
			return b;
		}

		/*const real* Rgb::data() const
		{
			return mdata.array;
		}*/

		void Rgb::red(const real v)
		{
			r = v;
		}
		void Rgb::green(const real v)
		{
			g = v;
		}
		void Rgb::blue(const real v)
		{
			b = v;
		}

		/*real& Rgb::operator[](int index)
		{
			return mdata.array[index];
		}
		const real& Rgb::operator[](int index) const
		{
			return mdata.array[index];
		}*/

		void Rgb::rgb(const real red, const real green, const real blue)
		{
			r = red;
			g = green;
			b = blue;
		}
		namespace // local
		{
			const real GetComponent(int i, int steps)
			{
				const int value = ((i >> 4*steps) & 0xff);
				return value / 255.0f;
			}
		}
		void Rgb::rgb(int rgb)
		{
			const real b = GetComponent(rgb, 0);
			const real g = GetComponent(rgb, 1);
			const real r = GetComponent(rgb, 2);
			this->rgb(r, g, b);
		}

		const Rgb Rgb::From(Color::Type color)
		{
			using namespace Color;

			switch(color)
			{
			case      White:                return Rgb(0xffffff);
			case      Silver:               return Rgb(0xc0c0c0);
			case      Gray:                 return Rgb(0x808080);
			case      Black:                return Rgb(0x000000);
			case      Navy:                 return Rgb(0x000080);
			case      Blue:                 return Rgb(0x0000ff);
			case      Aqua:                 return Rgb(0x00ffff);
			case      Teal:                 return Rgb(0x008080);
			case      Green:                return Rgb(0x008000);
			case      Olive:                return Rgb(0x808000);
			case      Lime:                 return Rgb(0x00ff00);
			case      Maroon:               return Rgb(0x800000);
			case      Red:                  return Rgb(0xff0000);
			case      Orange:               return Rgb(0xffa500);
			case      Yellow:               return Rgb(0xffff00);
			case      Purple:               return Rgb(0x800080);
			case      Fuchsia:              return Rgb(0xff00ff);
			case      AliceBlue:            return Rgb(0xf0f8ff);
			case      AntiqueWhite:         return Rgb(0xfaebd7);
			case      AquaMarine:           return Rgb(0x7fffd4);
			case      Azure:                return Rgb(0xf0ffff);
			case      Beige:                return Rgb(0xf5f5dc);
			case      Bisque:               return Rgb(0xffe4c4);
			case      BlanchedAlmond:       return Rgb(0xffebcd);
			case      BlueViolet:           return Rgb(0x8a2be2);
			case      Brown:                return Rgb(0xa52a2a);
			case      BurlyWood:            return Rgb(0xdeb887);
			case      CadetBlue:            return Rgb(0x5f9ea0);
			case      Chartreuse:           return Rgb(0x7fff00);
			case      Chocolate:            return Rgb(0xd2691e);
			case      Coral:                return Rgb(0xff7f50);
			case      CornflowerBlue:       return Rgb(0x6495ed);
			case      Cornsilk:             return Rgb(0xfff8dc);
			case      Crimson:              return Rgb(0xdc143c);
			case      Cyan:                 return Rgb(0x00ffff);
			case      Darkblue:             return Rgb(0x00008b);
			case      Darkcyan:             return Rgb(0x008b8b);
			case      DarkGoldenRod:        return Rgb(0xb8860b);
			case      Darkgray:             return Rgb(0xa9a9a9);
			case      Darkgreen:            return Rgb(0x006400);
			case      DarKkhaki:            return Rgb(0xbdb76b);
			case      DarkMagenta:          return Rgb(0x8b008b);
			case      DarkOliveGreen:       return Rgb(0x556b2f);
			case      DarkOrange:           return Rgb(0xff8c00);
			case      DarkOrchid:           return Rgb(0x9932cc);
			case      DarkRed:              return Rgb(0x8b0000);
			case      DarkSalmon:           return Rgb(0xe9967a);
			case      DarkSeaGreen:         return Rgb(0x8fbc8f);
			case      DarkslateBlue:        return Rgb(0x483d8b);
			case      DarkslateGray:        return Rgb(0x2f4f4f);
			case      DarkTurquoise:        return Rgb(0x00ced1);
			case      DarkBiolet:           return Rgb(0x9400d3);
			case      DeeppInk:             return Rgb(0xff1493);
			case      DeepskyBlue:          return Rgb(0x00bfff);
			case      DimGray:              return Rgb(0x696969);
			case      DodgerBlue:           return Rgb(0x1e90ff);
			case      FireBrick:            return Rgb(0xb22222);
			case      FloralWhite:          return Rgb(0xfffaf0);
			case      ForestGreen:          return Rgb(0x228b22);
			case      Gainsboro:            return Rgb(0xdcdcdc);
			case      GhostWhite:           return Rgb(0xf8f8ff);
			case      Gold:                 return Rgb(0xffd700);
			case      GoldenRod:            return Rgb(0xdaa520);
			case      GreenYellow:          return Rgb(0xadff2f);
			case      Honeydew:             return Rgb(0xf0fff0);
			case      Hotpink:              return Rgb(0xff69b4);
			case      IndianRed:            return Rgb(0xcd5c5c);
			case      Indigo:               return Rgb(0x4b0082);
			case      Ivory:                return Rgb(0xfffff0);
			case      Khaki:                return Rgb(0xf0e68c);
			case      Lavender:             return Rgb(0xe6e6fa);
			case      LavenderBlush:        return Rgb(0xfff0f5);
			case      Lawngreen:            return Rgb(0x7cfc00);
			case      LemonChiffon:         return Rgb(0xfffacd);
			case      LightBlue:            return Rgb(0xadd8e6);
			case      LightCoral:           return Rgb(0xf08080);
			case      LightCyan:            return Rgb(0xe0ffff);
			case      LightGoldenRodYellow: return Rgb(0xfafad2);
			case      LightGray:            return Rgb(0xd3d3d3);
			case      LightGreen:           return Rgb(0x90ee90);
			case      LightPink:            return Rgb(0xffb6c1);
			case      LightSalmon:          return Rgb(0xffa07a);
			case      LightSeaGreen:        return Rgb(0x20b2aa);
			case      LightskyBlue:         return Rgb(0x87cefa);
			case      LightslateGray:       return Rgb(0x778899);
			case      LightSteelBlue:       return Rgb(0xb0c4de);
			case      LightYellow:          return Rgb(0xffffe0);
			case      LimeGreen:            return Rgb(0x32cd32);
			case      Linen:                return Rgb(0xfaf0e6);
			case      Magenta:              return Rgb(0xff00ff);
			case      MediumAquaMarine:     return Rgb(0x66cdaa);
			case      MediumBlue:           return Rgb(0x0000cd);
			case      MediumOrchid:         return Rgb(0xba55d3);
			case      MediumPurple:         return Rgb(0x9370db);
			case      MediumSeaGreen:       return Rgb(0x3cb371);
			case      MediumslateBlue:      return Rgb(0x7b68ee);
			case      MediumSpringGreen:    return Rgb(0x00fa9a);
			case      MediumTurquoise:      return Rgb(0x48d1cc);
			case      MediumVioletRed:      return Rgb(0xc71585);
			case      MidnightBlue:         return Rgb(0x191970);
			case      Mintcream:            return Rgb(0xf5fffa);
			case      Mistyrose:            return Rgb(0xffe4e1);
			case      Moccasin:             return Rgb(0xffe4b5);
			case      NavajoWhite:          return Rgb(0xffdead);
			case      Oldlace:              return Rgb(0xfdf5e6);
			case      OliveDrab:            return Rgb(0x6b8e23);
			case      OrangeRed:            return Rgb(0xff4500);
			case      Orchid:               return Rgb(0xda70d6);
			case      PaleGoldenRod:        return Rgb(0xeee8aa);
			case      PaleGreen:            return Rgb(0x98fb98);
			case      PaleTurquoise:        return Rgb(0xafeeee);
			case      PaleVioletRed:        return Rgb(0xdb7093);
			case      Papayawhip:           return Rgb(0xffefd5);
			case      Peachpuff:            return Rgb(0xffdab9);
			case      Peru:                 return Rgb(0xcd853f);
			case      Pink:                 return Rgb(0xffc0cb);
			case      Plum:                 return Rgb(0xdda0dd);
			case      PowderBlue:           return Rgb(0xb0e0e6);
			case      Rosybrown:            return Rgb(0xbc8f8f);
			case      Royalblue:            return Rgb(0x4169e1);
			case      SaddleBrown:          return Rgb(0x8b4513);
			case      Salmon:               return Rgb(0xfa8072);
			case      SandyBrown:           return Rgb(0xf4a460);
			case      Seagreen:             return Rgb(0x2e8b57);
			case      Seashell:             return Rgb(0xfff5ee);
			case      Sienna:               return Rgb(0xa0522d);
			case      SkyBlue:              return Rgb(0x87ceeb);
			case      SlateBlue:            return Rgb(0x6a5acd);
			case      SlateGray:            return Rgb(0x708090);
			case      Snow:                 return Rgb(0xfffafa);
			case      SpringGreen:          return Rgb(0x00ff7f);
			case      SteelBlue:            return Rgb(0x4682b4);
			case      Tan:                  return Rgb(0xd2b48c);
			case      Thistle:              return Rgb(0xd8bfd8);
			case      Tomato:               return Rgb(0xff6347);
			case      Turquoise:            return Rgb(0x40e0d0);
			case      Violet:               return Rgb(0xee82ee);
			case      Wheat:                return Rgb(0xf5deb3);
			case      WhiteSmoke:           return Rgb(0xf5f5f5);
			case      YellowGreen:          return Rgb(0x9acd32);
			default:                       throw "invalid color name"; break;
			}
		}
	}
}