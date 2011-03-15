#ifndef PWN_MESH_MESH
#define PWN_MESH_MESH

#include <vector>
#include <map>
#include <pwn/math/types.h>
#include <pwn/string.h>
#include <boost/noncopyable.hpp>
#include <pwn/math/rgba.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

namespace pwn
{
	namespace io
	{
		template<class T, typename V>
		class MeshFile;
	}

	namespace mesh
	{
		class Material
		{
		public:
			Material();

			math::Rgba ambient; // -1 - 1
			math::Rgba diffuse; // -1 - 1
			math::Rgba specular; // -1 - 1
			math::Rgba emission; // -1 - 1
			real shininess; // 0-1

			enum
			{
				TextureSize = 200
			};

			pwn::tchar texture_diffuse[TextureSize];

			const string getTexture_Diffuse() const;
			void setTexture_Diffuse(const pwn::string& v);
		};

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
			BoneIndex getParent() const;
			void setParent(BoneIndex bi);

			void setName(const string& n);
			string getName() const;

			tchar name[BoneNameSize::Value];
			math::vec3 pos;
			math::quat rot;
		private:
			BoneIndex parent;
		};

		struct Point
		{
			Point();
			Point(const math::vec3& location, BoneIndex bone);
			Point(const math::vec3& location, math::vec3 normal, math::vec2 textcoord, BoneIndex bone);
			bool hasBone() const;
			BoneIndex getBone() const;

			math::vec3 location;
			math::vec3 normal;
			math::vec2 textcoord;
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

		class FramePosition : public Timed
		{
		public:
			FramePosition();
			FramePosition(real time, const math::vec3& location);
			string toString() const;

			math::vec3 value() const;

			math::vec3 location;
		};

		math::vec3 Interpolate(const FramePosition& from, real current, const FramePosition& to);

		class FrameRotation : public Timed
		{
		public:
			FrameRotation();
			FrameRotation(real time, const math::quat& rotation);
			string toString() const;

			math::quat value() const;

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
		{
		public:
			AnimationPerBone();
			AnimationPerBone(std::vector<FramePosition>& afp, std::vector<FrameRotation>& afr);

			std::vector<FramePosition> fp;
			std::vector<FrameRotation> fr;

			void addPosition(real time, const math::vec3& pos);
			void addRotation(real time, const math::quat& q);

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
			std::vector<PosePerBone> bones;
		};

		class CompiledPose
		{
		public:
			std::vector<math::mat44> transforms;
			CompiledPose();
			CompiledPose(const Pose& pose, const std::vector<Bone>& def);
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
			Animation();
			Animation(const std::vector<AnimationPerBone>& bones);
			void getPose(real time, Pose* out) const;
			real getLength() const;

			std::vector<AnimationPerBone> bones;
			real length; // move to private

			void subanim(int start, int end, Animation* out) const;
			void subanim(const AnimationInformation& info, Animation* out) const;

			void scale(real scale);
		};

		//

		class Triangle
		{
		public:
			typedef pwn::uint32 VertexIndex;
			VertexIndex v0;
			VertexIndex v1;
			VertexIndex v2;

			Triangle(VertexIndex v0, VertexIndex v1, VertexIndex v2);
			Triangle(VertexIndex v[3]);
			Triangle();

			VertexIndex& operator[](pwn::uint32 index);
			const VertexIndex& operator[](pwn::uint32 index) const;
		};

		class VertexData
		{
		public:
			VertexData();
			VertexData(const VertexData& m);
			const VertexData& operator=(const VertexData& m);

			pwn::uint32 getCount() const;
			BoneIndex getBone(pwn::uint32 id) const;
		private:
			void doCopy(const VertexData& m);
		protected:
			template<class T, typename V>
			friend class pwn::io::MeshFile;

			pwn::uint32 count;
			boost::scoped_array<real> locations; // 3*count
			boost::scoped_array<real> normals; // 3*count
			boost::scoped_array<real> textcoords; // 2*count
			boost::scoped_array<BoneIndex> boneindexes; // count
		};

		class Mesh
		{
		public:
			typedef std::vector<Triangle> TriList;
			//typedef boost::shared_ptr<TriList> TriListPtr;
			typedef std::map<pwn::uint32, TriList> TriangleMap;

			pwn::uint32 validate(bool testSortedBones) const;
			const VertexData& data() const;
			const std::vector<Material> getMaterials() const;
		protected:
			template<class T, typename V>
			friend class pwn::io::MeshFile;

			VertexData vertexes;
			std::vector<Bone> bones;

			TriangleMap triangles; // the map key referenses the materials vector below
			std::vector<Material> materials; // this is done so materials could easily be swapped and overriden later on per actor
		};
	}
}

#endif
