#include <pwn/mesh/builder>
#include <pwn/mesh/mesh>

namespace pwn
{
	namespace mesh
	{
		Builder::Builder()
		{
		}

		Builder::~Builder()
		{
		}

		namespace // local
		{
			template <typename T>
			void Copy(const std::vector<T>& from, pwn::core::sarray<T>* to)
			{
				to->size = from.size();
				if( to->size == 0 )
				{
					to->data.reset(0);
				}
				else
				{
					to->data.reset( new T[to->size] );
					// todo...
				}
			}
		}

		void Builder::setupMesh(Mesh* mesh) const
		{
			Copy<math::vec3>(positions, &mesh->positions);
			Copy<math::vec3>(normals, &mesh->normals);
			Copy<math::vec2>(texcoords, &mesh->texcoords);
			Copy<Triangle>(triangles, &mesh->triangles);
		}

		Triangle::index Builder::addPosition(const math::vec3& pos)
		{
			positions.push_back(pos);
			return static_cast<Triangle::index>(positions.size()-1);
		}

		Triangle::index Builder::addNormal(const math::vec3& norm)
		{
			normals.push_back(norm);
			return static_cast<Triangle::index>(normals.size()-1);
		}

		Triangle::index Builder::addTextCoord(const math::vec2& tc)
		{
			texcoords.push_back(tc);
			return static_cast<Triangle::index>(texcoords.size()-1);
		}

		void Builder::addTriangle(const Triangle& tri)
		{
			triangles.push_back(tri);
		}
	}
}