#ifndef PWN_MESH_MESH
#define PWN_MESH_MESH

#include <vector>
#include <map>
#include <pwn/math/types.h>
#include <pwn/mesh/triangle.h>
#include <pwn/string.h>
#include <pwn/core/vector.h>

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace mesh
	{
		class Material;

		typedef pwn::uint32 BoneIndex;

		class Bone
		{
		public:
			Bone();
			const string toString();
			bool hasParent() const;
			BoneIndex parent;
			string name;
			math::vec3 pos;
			math::quat rot;

			BoneIndex index;
			Bone* parentBone;

			std::vector<Bone*> childs;
		};

		struct Point
		{
			Point();
			Point(const math::vec3& location, BoneIndex bone);
			math::vec3 location;
			BoneIndex bone;
		};

		//

		class Timed
		{
		public:
			float getTime() const;
		protected:
			explicit Timed(float t);
		private:
			float time;
		};

		template<typename T>
		int Get(const core::Vector<T>& da, float current)
		{
			for (int i = 1; i < da.Count; ++i)
			{
				if (math1.IsWithin(da[i-1].time, current, da[i].time))
				{
					return i;
				}
			}

			return -1;
		};

		class FramePosition : public Timed
		{
		public:
			FramePosition(float time, math::vec3 location);
			string toString() const;
			math::vec3 getLocation() const;

		private:
			math::vec3 location;
		};

		math::vec3 Interpolate(const FramePosition& from, float current, const FramePosition& to);

		class FrameRotation : public Timed
		{
			FrameRotation(float time, math::quat rotation);
			string toString() const;
			math::quat getRotation() const;

		private:
			math::quat rotation;
		};

		math::quat Interpolate(const FrameRotation& from, float current, const FrameRotation& to);

		class PosePerBone
		{
		public:
			PosePerBone(math::vec3 l, math::quat r);

			math::vec3 location;
			math::quat rotation;

			string toString() const;
		};

		class AnimationPerBone
		{
		public:
			core::Vector<FramePosition> fp;
			core::Vector<FrameRotation> fr;

			string ToString() const;

			float getLength() const;

			void addPositon(float time, math::vec3 vec);
			void addPositon(FramePosition fp);

			void addRotation(float time, math::quat rotation);
			void addRotation(FrameRotation fr);

			PosePerBone getBonePose(float time) const;

			AnimationPerBone sub(int start, int end) const;
			void scale(float scale);

		private:
			static math::quat Interpolate(float time, core::Vector<FrameRotation> fr);
			static math::vec3 Interpolate(float time, core::Vector<FramePosition> fp);
		};

		class Pose
		{
		public:
			Pose(const core::Vector<PosePerBone>& pose);
			const core::Vector<PosePerBone> bones;
		};

		class MeshDef;

		class CompiledPose
		{
		public:
			core::Vector<math::mat44> transforms;
			CompiledPose(Pose pose, const MeshDef& def);
		};

		class AnimationInformation
		{
		public:
			AnimationInformation(int start, int end, const string& name);
			int start;
			int end;
			string name;
		};

		class Animation
		{
		public:
			Animation(core::Vector<AnimationPerBone> bones);
			Pose getPose(float time);

			core::Vector<AnimationPerBone> bones;
			const float length;

			Animation subanim(int start, int end) const;
			Animation subanim(const AnimationInformation& info) const;

			void scale(float scale);
		};

		//

		class Mesh
		{
		public:
			typedef std::vector<Triangle> TriangleList;
			typedef boost::shared_ptr<Material> MaterialPtr;

			Mesh();

			void clear();

			Triangle::index addPosition(const Point& pos);
			Triangle::index addPosition(const math::vec3& pos, BoneIndex bone); // syntasx sugar
			Triangle::index addNormal(const math::vec3& norm);
			Triangle::index addTextCoord(const math::vec2& tc);

			void addTriangle(const Triangle& tri);
			Triangle::index addMaterial(MaterialPtr m);

			// todo: make private and add accessors instead...
			std::vector<Point> positions;
			std::vector<math::vec3> normals;
			std::vector<math::vec2> texcoords;
			std::vector<Bone> bones;

			TriangleList triangles; // the map key referenses the materials vector below
			std::vector<MaterialPtr> materials; // this is done so materials could easily be swapped and overriden later on per actor
		};
	}
}

#endif