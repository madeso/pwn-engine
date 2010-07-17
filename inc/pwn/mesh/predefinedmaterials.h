#ifndef PWN_MESH_PREDEFINEDMATERIALS
#define PWN_MESH_PREDEFINEDMATERIALS

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace mesh
	{
		class Material;

		namespace materials
		{
			boost::shared_ptr<Material> Emerald();
			boost::shared_ptr<Material> Jade();
			boost::shared_ptr<Material> Obsidian();
			boost::shared_ptr<Material> Pearl();
			boost::shared_ptr<Material> Ruby();
			boost::shared_ptr<Material> Turquoise();
			boost::shared_ptr<Material> Brass();
			boost::shared_ptr<Material> Bronze();
			boost::shared_ptr<Material> Chrome();
			boost::shared_ptr<Material> Copper();
			boost::shared_ptr<Material> Gold();
			boost::shared_ptr<Material> Silver();
			boost::shared_ptr<Material> Plastic_Black();
			boost::shared_ptr<Material> Plastic_Cyan();
			boost::shared_ptr<Material> Plastic_Green();
			boost::shared_ptr<Material> Plastic_Red();
			boost::shared_ptr<Material> Plastic_White();
			boost::shared_ptr<Material> Plastic_Yellow();
			boost::shared_ptr<Material> Rubber_Black();
			boost::shared_ptr<Material> Rubber_Cyan();
			boost::shared_ptr<Material> Rubber_Green();
			boost::shared_ptr<Material> Rubber_Red();
			boost::shared_ptr<Material> Rubber_White();
			boost::shared_ptr<Material> Rubber_Yellow();

			boost::shared_ptr<Material> White();
		}
	}
}

#endif