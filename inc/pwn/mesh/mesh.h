#ifndef PWN_MESH_MESH
#define PWN_MESH_MESH

#include <vector>
#include <map>
#include <pwn/math/types.h>
#include <pwn/string.h>
#include <pwn/core/vector.h>

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace mesh
	{
		class Material;

		/**
		0 = no bone
		>0 = one more than index in bone arrray
		*/
		typedef pwn::uint32 BoneIndex;

		namespace BoneNameSize
		{
			enum
			{
				Value = 100
			};
		}

		class Bone
		{
		public:
			Bone();
			bool hasParent() const;

			void setName(const string& n);
			string getName() const;

			BoneIndex parent;
			tchar name[BoneNameSize::Value];
			math::vec3 pos;
			math::quat rot;
		};

		struct Point
		{
			Point();
			Point(const math::vec3& location, BoneIndex bone);
			bool hasBone() const;
			BoneIndex getBone() const;
			math::vec3 location;
			BoneIndex bone;
		};

		//

		class Timed
		{
		public:
			real getTime() const;
		protected:
			explicit Timed(real t);
		private:
			real time;
		};

		template<typename T>
		int Get(const core::Vector<T>& da, real current)
		{
			for (std::size_t i = 1; i < da.size(); ++i)
			{
				if (math::IsWithin(da[i-1].getTime(), current, da[i].getTime()))
				{
					return i;
				}
			}

			return -1;
		};

		class FramePosition : public Timed
		{
		public:
			FramePosition();
			FramePosition(real time, const math::vec3& location);
			string toString() const;

			math::vec3 location;
		};

		math::vec3 Interpolate(const FramePosition& from, real current, const FramePosition& to);

		class FrameRotation : public Timed
		{
		public:
			FrameRotation();
			FrameRotation(real time, const math::quat& rotation);
			string toString() const;

			math::quat rotation;
		};

		math::quat Interpolate(const FrameRotation& from, real current, const FrameRotation& to);

		class PosePerBone
		{
		public:
			PosePerBone();
			PosePerBone(math::vec3 l, math::quat r);

			math::vec3 location;
			math::quat rotation;

			string toString() const;
		};

		class AnimationPerBone
			: boost::noncopyable
		{
		public:
			AnimationPerBone();
			AnimationPerBone(core::Vector<FramePosition>& afp, core::Vector<FrameRotation>& afr);

			core::Vector<FramePosition> fp;
			core::Vector<FrameRotation> fr;

			string toString() const;

			real getLength() const;

			PosePerBone getBonePose(real time) const;

			void sub(int start, int end, AnimationPerBone* out) const;
			void scale(real scale);
		};

		class Pose
			: boost::noncopyable
		{
		public:
			explicit Pose(core::Vector<PosePerBone>& pose);
			core::Vector<PosePerBone> bones;
		};

		class CompiledPose
			: boost::noncopyable
		{
		public:
			core::Vector<math::mat44> transforms;
			CompiledPose();
			CompiledPose(const Pose& pose, const core::Vector<Bone>& def);
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
			: boost::noncopyable
		{
		public:
			Animation(core::Vector<AnimationPerBone>& bones);
			void getPose(real time, Pose* out) const;

			core::Vector<AnimationPerBone> bones;
			const real length;

			void subanim(int start, int end, Animation* out) const;
			void subanim(const AnimationInformation& info, Animation* out) const;

			void scale(real scale);
		};

		//

		class Triangle
		{
		public:
			typedef pwn::uint32 index;
			struct Vertex
			{
				index location;
				index normal;
				index texcoord;
				index bone;

				Vertex(index location, index normal, index texcoord);
				Vertex(index location_textcoord, index normal);
				Vertex();
			};
			Vertex v0;
			Vertex v1;
			Vertex v2;

			Triangle(Vertex v0, Vertex v1, Vertex v2);
			Triangle(Vertex v[3]);
			Triangle();

			Vertex& operator[](pwn::uint32 index);
			const Vertex& operator[](pwn::uint32 index) const;
		};

		class Mesh
		{
		public:
			typedef core::Vector<Triangle> TriList;
			typedef boost::shared_ptr<TriList> TriListPtr;
			typedef std::map<pwn::uint32, TriListPtr> TriangleMap;
			typedef boost::shared_ptr<Material> MaterialPtr;

			Mesh();

			void clear();
			pwn::uint32 validate() const;

			// todo: make private and add accessors instead...
			core::Vector<Point> positions;
			core::Vector<math::vec3> normals;
			core::Vector<math::vec2> texcoords;
			core::Vector<Bone> bones;

			TriangleMap triangles; // the map key referenses the materials vector below
			core::Vector<MaterialPtr> materials; // this is done so materials could easily be swapped and overriden later on per actor
		};
	}
}

#endif