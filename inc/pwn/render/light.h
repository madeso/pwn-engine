#ifndef PWN_RENDER_LIGHT
#define PWN_RENDER_LIGHT

#include <pwn/math/types.h>
#include <pwn/math/rgba.h>

namespace pwn
{
	namespace render
	{
		class LightProperties
		{
		public:
			LightProperties();
			void apply(int light);

			math::Rgba ambient; // -1 to 1
			math::Rgba diffuse; // -1 to 1
			math::Rgba specular; // -1 to 1
		};

		class Attenuation
		{
		public:
			Attenuation();
			void apply(int light);

			real constant;
			real linear;
			real quadratic;
		};

		class Light
		{
		public:
			virtual ~Light();
			virtual void apply(int light) = 0;

			LightProperties properties;
		};

		class PointLight
			: public Light
		{
		public:
			PointLight();
			void apply(int light);

			Attenuation attenuation;
			math::vec3 position;
		};

		class DirectionalLight
			: public Light
		{
		public:
			DirectionalLight();
			void apply(int light);

			Attenuation attenuation;
			math::quat direction;
		};

		class SpotLight
			: public Light
		{
		public:
			SpotLight();
			void apply(int light);

			Attenuation attenuation;
			math::vec3 position;
			math::quat direction;
			real exponent; // 0-128
			real cutoff; // 0-90
		};
	}
}

#endif
