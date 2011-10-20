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
			Point(const math::vec3& position, math::vec4 bone);
			Point(const math::vec3& position, math::vec3 normal, math::vec2 textcoord, math::vec4 bone);
			bool hasBone() const;
			math::vec4 getBone() const;

			math::vec3 position;
			math::vec3 normal;
			math::vec2 textcoord;
			math::vec4 bone;
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
			FramePosition(real time, const math::vec3& position);
			string toString() const;

			math::vec3 value() const;

			math::vec3 position;
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

			math::vec3 position;
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

		typedef std::pair<math::mat44, int> CompiledPoseData;

		class CompiledPose
		{
		public:
			std::vector<CompiledPoseData> transforms;
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

		void SetBoneIndex(real* val, BoneIndex newIndex);
		BoneIndex GetBoneIndex(real val);
		real GetBoneInfluence(real val);

		class VertexData
		{
		public:
			VertexData();
			VertexData(const std::vector<math::vec3>& posv,
			           const std::vector<math::vec3>& normv,
			           const std::vector<math::vec2>&textv,
			           const std::vector<math::vec4>& bonev);
			VertexData(const VertexData& m);
			const VertexData& operator=(const VertexData& m);

			pwn::uint32 getCount() const;
			math::vec4 getBone(pwn::uint32 id) const;
			const Point getPoint(uint32 i) const;
			void setLocationNormal(const uint32 i, const math::vec3& pos, const math::vec3& norm);
		private:
			void doCopy(const VertexData& m);
		protected:
			template<class T, typename V>
			friend class pwn::io::MeshFile;

			const math::vec3x pos(const uint32 i) const;
			const math::vec3x norm(const uint32 i) const;
			const math::vec2x tex(const uint32 i) const;
			const math::vec4x bone(const uint32 i) const;
			math::vec3x pos(const uint32 i);
			math::vec3x norm(const uint32 i);
			math::vec2x tex(const uint32 i);
			math::vec4x bone(const uint32 i);

			pwn::uint32 count;
			boost::scoped_array<real> locations; // 3*count
			boost::scoped_array<real> normals; // 3*count
			boost::scoped_array<real> textcoords; // 2*count
			boost::scoped_array<real> boneindexes; // 4*count
		};

		class Mesh
		{
		public:
			typedef std::vector<Triangle> TriList;
			//typedef boost::shared_ptr<TriList> TriListPtr;
			typedef std::map<pwn::uint32, TriList> TriangleMap;

			Mesh();
			Mesh(const std::vector<math::vec3>& posv,
			     const std::vector<math::vec3>& normv,
			     const std::vector<math::vec2>&textv,
			     const std::vector<math::vec4>& bonev,
			     const TriangleMap& trim,
			     const std::vector<Bone>& bones,
			     const std::vector<Material>& materials);

			pwn::uint32 validate(bool testSortedBones) const;
			const VertexData& data() const;
			const std::vector<Material>& getMaterials() const;
			const std::vector<Bone>& getBones() const;
			const TriangleMap& getTriangles() const;
			void setLocationNormal(const uint32 i, const math::vec3& pos, const math::vec3& norm);

			pwn::uint32 getNumberOfTriangles() const;
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
