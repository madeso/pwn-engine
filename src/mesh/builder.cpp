#pragma warning(disable:4512) // boost unable to generate assignment operator

#include <pwn/mesh/builder>
#include <pwn/mesh/mesh>
#include <boost/foreach.hpp>
#include <pwn/math/operations>

namespace pwn
{
	namespace mesh
	{
		Mesh* Move(Mesh* mesh, const math::vec3& dir)
		{
			BOOST_FOREACH(math::vec3& v, mesh->positions)
			{
				v += dir;
			}

			return mesh;
		}

		namespace // local
		{
			Triangle::Vertex v(Triangle::index p, Triangle::index t)
			{
				return Triangle::Vertex(p, 0, t);
			}
		}

		Mesh* SetBox(Mesh* mesh, boost::shared_ptr<Material> material, real w, real h, real d, bool faceOut)
		{
			using math::vec2;
			using math::vec3;

			mesh->clear();

			const Triangle::index t0 = mesh->addTextCoord(vec2(0,1));
			const Triangle::index t1 = mesh->addTextCoord(vec2(1,1));
			const Triangle::index t2 = mesh->addTextCoord(vec2(0,0));
			const Triangle::index t3 = mesh->addTextCoord(vec2(1,0));

			// front side
			const Triangle::index v0 = mesh->addPosition(vec3(0, 0, 0));
			const Triangle::index v1 = mesh->addPosition(vec3(w, 0, 0));
			const Triangle::index v2 = mesh->addPosition(vec3(0, h, 0));
			const Triangle::index v3 = mesh->addPosition(vec3(w, h, 0));

			// back side
			const Triangle::index v4 = mesh->addPosition(vec3(0, 0, d));
			const Triangle::index v5 = mesh->addPosition(vec3(w, 0, d));
			const Triangle::index v6 = mesh->addPosition(vec3(0, h, d));
			const Triangle::index v7 = mesh->addPosition(vec3(w, h, d));

			AddQuad(mesh, !faceOut, 0, v(v0,t2), v(v2,t0), v(v3,t1), v(v1,t3)); // front
			AddQuad(mesh, !faceOut, 0, v(v1,t3), v(v3,t1), v(v7,t0), v(v5,t2)); // right
			AddQuad(mesh, !faceOut, 0, v(v4,t3), v(v6,t1), v(v2,t0), v(v0,t2)); // left
			AddQuad(mesh, !faceOut, 0, v(v5,t2), v(v7,t0), v(v6,t1), v(v4,t3)); // back
			AddQuad(mesh, !faceOut, 0, v(v3,t1), v(v2,t3), v(v6,t2), v(v7,t0)); // up
			AddQuad(mesh, !faceOut, 0, v(v4,t0), v(v0,t1), v(v1,t3), v(v5,t2)); // bottom

			mesh->materials.push_back(material);

			return mesh;
		}

		void AddQuad(Mesh* mesh, bool reverse, pwn::uint32 material, const Triangle::Vertex& v0, const Triangle::Vertex& v1, const Triangle::Vertex& v2, const Triangle::Vertex& v3)
		{
			if( reverse )
			{
				mesh->addTriangle(Triangle(material, v2, v1, v0) );
				mesh->addTriangle(Triangle(material, v3, v2, v0) );
			}
			else
			{
				mesh->addTriangle(Triangle(material, v0, v1, v2) );
				mesh->addTriangle(Triangle(material, v0, v2, v3) );
			}
		}

		Mesh* BuildNormals(Mesh* mesh)
		{
			using math::vec3;
			std::vector<vec3> vertexNormalsSum(mesh->positions.size(), vec3(0,0,0));
			BOOST_FOREACH(Triangle& t, mesh->triangles)
			{
				const vec3 p0 = mesh->positions[t[0].location];
				const vec3 p1 = mesh->positions[t[1].location];
				const vec3 p2 = mesh->positions[t[2].location];

				const vec3 d0 = math::FromTo(p1, p0);
				const vec3 d1 = math::FromTo(p1, p2);

				const vec3 faceNormal = math::crossNorm(d0, d1);

				vertexNormalsSum[t[0].location] += faceNormal;
				vertexNormalsSum[t[1].location] += faceNormal;
				vertexNormalsSum[t[2].location] += faceNormal;

				for(int i=0; i<3; ++i) t[i].normal = t[i].location;
			}

			mesh->normals.clear();
			BOOST_FOREACH(const vec3& normalSum, vertexNormalsSum)
			{
				mesh->normals.push_back( math::GetNormalized(normalSum) );
			}

			return mesh;
		}

		Mesh* InvertNormals(Mesh* mesh)
		{
			BOOST_FOREACH(math::vec3& n, mesh->normals)
			{
				n = -n;
			}

			return mesh;
		}
	}
}