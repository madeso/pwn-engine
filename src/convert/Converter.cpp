#include "Converter.hpp"

#include <pwn/math/operations.h>
#include <pwn/mesh/mesh.h>
#include <pwn/assert.h>

#include <iostream>

namespace pwn
{
	namespace convert
	{
		NamedMaterials::NamedMaterials(::pwn::mesh::Builder* aBuilder)
			: mBuilder(aBuilder)
		{
		}

		uint32 NamedMaterials::getMaterial(const pwn::string& name) const
		{
			MaterialNameIdMap::const_iterator res = materialid.find(name);
			if( res == materialid.end() ) throw "unable to find material..";
			else return res->second;
		}
		
		mesh::BTriangle::index NamedMaterials::addMaterial(const pwn::string& name, mesh::Material m)
		{
			materialid[name] = mBuilder->materials.size();
			return mBuilder->addMaterial(m);
		}
	}
}
