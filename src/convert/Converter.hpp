#ifndef PWN_CONVERT_CONVERTER_HPP
#define PWN_CONVERT_CONVERTER_HPP

#include <vector>
#include <map>

#include <pwn/math/types.h>
#include <pwn/math/rgba.h>
#include <pwn/string.h>
#include <pwn/mesh/builder.h>

namespace pwn
{
	namespace convert
	{
		class NamedMaterials
		{
		public:
			explicit NamedMaterials(::pwn::mesh::Builder* aBuilder);

			mesh::BTriangle::index addMaterial(const pwn::string& name, mesh::Material m);
			uint32 getMaterial(const pwn::string& name) const;

		private:
			::pwn::mesh::Builder* mBuilder;

			typedef std::map<pwn::string, uint32> MaterialNameIdMap;
			MaterialNameIdMap materialid;
		};
	}
}

#endif
