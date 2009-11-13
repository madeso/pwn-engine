#pragma warning(disable:4512) // boost unable to generate assignment operator

#include <pwn/mesh/builder>
#include <pwn/mesh/mesh>
#include <boost/foreach.hpp>
#include <pwn/math/operations>

namespace pwn
{
	namespace mesh
	{
		void Move(Mesh* mesh, const math::vec3& dir)
		{
			BOOST_FOREACH(math::vec3& v, mesh->positions)
			{
				v += dir;
			}
		}

		void SetCube(Mesh* mesh, boost::shared_ptr<Material> material, real w, real h, real d, bool faceOut)
		{
			using math::vec3;

			mesh->clear();

			const Triangle::Vertex v0(mesh->addPosition(vec3(0, 0, 0)), 0, 0);
			const Triangle::Vertex v1(mesh->addPosition(vec3(w, 0, 0)), 0, 0);
			const Triangle::Vertex v2(mesh->addPosition(vec3(0, h, 0)), 0, 0);
			const Triangle::Vertex v3(mesh->addPosition(vec3(w, h, 0)), 0, 0);

			const Triangle::Vertex v4(mesh->addPosition(vec3(0, 0, d)), 0, 0);
			const Triangle::Vertex v5(mesh->addPosition(vec3(w, 0, d)), 0, 0);
			const Triangle::Vertex v6(mesh->addPosition(vec3(0, h, d)), 0, 0);
			const Triangle::Vertex v7(mesh->addPosition(vec3(w, h, d)), 0, 0);

			AddQuad(mesh, !faceOut, 0, v0, v2, v3, v1); // front
			AddQuad(mesh, !faceOut, 0, v1, v3, v7, v5); // right
			AddQuad(mesh, !faceOut, 0, v4, v6, v2, v0); // left
			AddQuad(mesh, !faceOut, 0, v5, v7, v6, v4); // back
			AddQuad(mesh, !faceOut, 0, v3, v2, v6, v7); // up
			AddQuad(mesh, !faceOut, 0, v4, v0, v7, v5); // bottom

			mesh->materials.push_back(material);
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

		void BuildNormals(Mesh* mesh)
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
		}

		void InvertNormals(Mesh* mesh)
		{
			BOOST_FOREACH(math::vec3& n, mesh->normals)
			{
				n = -n;
			}
		}
	}
}