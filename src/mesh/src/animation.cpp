#pragma warning(disable: 4244) // conversion, possible loss of data

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include <pwn/core/stdutil.h>
#include <boost/foreach.hpp>
#include <pwn/assert.h>

#include <pwn/core/stdutil.h>
#include <fstream>

namespace pwn
{
	namespace mesh
	{
		real Timed::getTime() const
		{
			return time;
		}

		Timed::Timed(real t)
			: time(t)
		{
		}

		FramePosition::FramePosition()
			: Timed(0)
			, location(math::Origo3().vec)
		{
		}
	
		FramePosition::FramePosition(real time, const math::vec3& loc)
			: Timed(time)
			, location(loc)
		{
		}

		string FramePosition::toString() const
		{
			return core::Str() << getTime() << " " << location;
		}

		math::vec3 Interpolate(const FramePosition& from, real current, const FramePosition& to)
		{
			real scale = math::To01(from.getTime(), current, to.getTime());
			if (math::IsWithinInclusive(0, scale, 1) == false) throw "invalid scale";
			return math::Lerp(from.location, scale, to.location);
		}

		FrameRotation::FrameRotation()
			: Timed(0)
			, rotation(math::qIdentity())
		{
		}
		
		FrameRotation::FrameRotation(real time, const math::quat& rot)
			: Timed(time)
			, rotation(rot)
		{
		}
		
		string FrameRotation::toString() const
		{
			return core::Str() << getTime() << " " << math::cAxisAngle(rotation);
		}

		math::quat Interpolate(const FrameRotation& from, real current, const FrameRotation& to)
		{
			real scale = math::To01(from.getTime(), current, to.getTime());
			if (math::IsWithinInclusive(0, scale, 1) == false) throw "invalid scale";
			return math::SlerpShortway(from.rotation, scale, to.rotation);
		}

		PosePerBone::PosePerBone()
			: location(math::Origo3().vec)
			, rotation(math::qIdentity())
		{
		}

		PosePerBone::PosePerBone(math::vec3 l, math::quat r)
			: location(l)
			, rotation(r)
		{
		}

		string PosePerBone::toString() const
		{
			return core::Str() << location << ": " << math::cAxisAngle(rotation);
		}

		math::quat Interpolate(real time, const std::vector<FrameRotation>& fr)
		{
			const int fri = Get(fr, time);
			if (fri == -1) return math::qIdentity();
			const math::quat r( Interpolate(fr[fri - 1], time, fr[fri]) );
			return r;
		}
		
		math::vec3 Interpolate(real time, const std::vector<FramePosition>& fp)
		{
			const int fpi = Get(fp, time);
			if (fpi == -1) return math::Origo3().vec;
			const math::vec3 res( Interpolate(fp[fpi - 1], time, fp[fpi]) );
			return res;
		}

		AnimationPerBone::AnimationPerBone()
		{
		}

		AnimationPerBone::AnimationPerBone(std::vector<FramePosition>& afp, std::vector<FrameRotation>& afr)
		{
			fp = afp;
			fr = afr;
		}

		void AnimationPerBone::addPosition(real time, const math::vec3& pos)
		{
			fp.push_back(FramePosition(time, pos));
		}

		void AnimationPerBone::addRotation(real time, const math::quat& q)
		{
			fr.push_back(FrameRotation(time, q));
		}

		string AnimationPerBone::toString() const
		{
			return core::Str() << "<" << fp.size() << " " << fr.size() << ">";
		}

		real AnimationPerBone::getLength() const
		{
			// get the time of the last frame
			return fp[fp.size() - 1].getTime();
		}

		PosePerBone AnimationPerBone::getBonePose(real time) const
		{
			const PosePerBone p(
				Interpolate(time, fp),
				Interpolate(time, fr) );
			return p;
		}

		void AnimationPerBone::sub(int start, int end, AnimationPerBone* out) const
		{
			std::vector<FramePosition> abfp;
			std::vector<FrameRotation> abfr;
			real length = end - start;
			bool first = true;
			real last = 0;

			for(std::size_t i=0; i<this->fp.size(); ++i)
			{
				const FramePosition& fp = this->fp[i];
				if (math::IsWithinInclusive(start, fp.getTime(), end))
				{
					real mark = fp.getTime()-start;
					if (first && math::IsZero(mark)==false)
					{
						abfp.push_back(FramePosition(0, Interpolate(start, this->fp)));
					}
					mark = math::ZeroOrValue(mark);
					first = false;
					abfp.push_back(FramePosition(mark, fp.location));
					last = mark;
				}
			}
			if (math::IsEqual(length, last)==false )
			{
				abfp.push_back(FramePosition(length, Interpolate(end, this->fp)));
			}

			first = true;
			last = 0;

			for(std::size_t i=0; i<this->fr.size(); ++i)
			{
				const FrameRotation& fr = this->fr[i];
				if (math::IsWithinInclusive(start, fr.getTime(), end))
				{
					real mark = fr.getTime() - start;
					if (first && math::IsZero(mark) == false)
					{
						abfr.push_back(FrameRotation(0, Interpolate(start, this->fr)));
					}
					mark = math::ZeroOrValue(mark);
					first = false;
					abfr.push_back(FrameRotation(mark, fr.rotation));
					last = mark;
				}
			}
			if (math::IsEqual(length, last) == false)
			{
				abfr.push_back(FrameRotation(length, Interpolate(end, this->fr)));
			}

			if (abfp.size() < 2 || abfr.size() < 2) throw "Data error, need atleast 2 keyframes per animation";
			out->fp = abfp;
			out->fr = abfr;
		}

		void AnimationPerBone::scale(real scale)
		{
			for(std::size_t i=0; i<fp.size(); ++i)
			{
				FramePosition& f = fp[i];
				f.location *= scale;
			}
		}

		CompiledPose::CompiledPose()
		{
		}

		namespace 
		{
			void dump(const pwn::string& file, const math::mat44& p)
			{
				std::ofstream of(file.c_str());
				for(int r=0; r<4; ++r)
				{
					for(int c=0; c<4; ++c)
					{
						of << p.at(r, c) << " ";
					}
					of << std::endl;
				}
			}

			void dump(const pwn::string& file, const pwn::mesh::Bone& b)
			{
				std::ofstream of(file.c_str());

				of << b.rot.x << " "
					 << b.rot.y << " "
					 << b.rot.z << " "
					 << b.rot.w << std::endl;

				of << b.pos.x << " "
					 << b.pos.y << " "
					 << b.pos.z << std::endl;
			}
		}

		CompiledPose::CompiledPose(const Pose& pose, const std::vector<Bone>& bones)
		{
			if (pose.bones.size() != bones.size()) throw "Invalid animation/mesh, bone count differs";
			std::vector<math::mat44> result( pose.bones.size() );
			for (std::size_t boneIndex = 0; boneIndex < pose.bones.size(); ++boneIndex)
			{
				const Bone& bone = bones[boneIndex];
				// either it is a parent, or it's parent has already been precoessed
				Assert( bone.hasParent()==false || boneIndex > bone.getParent() );
				math::mat44 parent = bone.hasParent() ? result[bone.getParent()] : math::mat44Identity();
				const math::vec3 poseloc = pose.bones[boneIndex].location;
				const math::quat poserot = pose.bones[boneIndex].rotation;

				const math::mat44 anim = math::mat44helper(math::mat44Identity()).translate(poseloc).rotate(-poserot).mat;
				const math::mat44 skel = math::mat44helper(math::mat44Identity()).translate(bone.pos).rotate(bone.rot).mat;
				const math::mat44 local = math::mat44helper(math::mat44Identity()).mult(skel).mult(anim).mat;
				
				{
					using namespace pwn::math;
					using namespace pwn::mesh;
					using namespace pwn::core;
					dump(Str() << "C:\\Users\\Gustav\\dev\\pwn-engine\\dist\\temp\\pwn\\a0 bone " << boneIndex << ".txt", bone);
					dump(Str() << "C:\\Users\\Gustav\\dev\\pwn-engine\\dist\\temp\\pwn\\b anim " << boneIndex << ".txt", anim);
					dump(Str() << "C:\\Users\\Gustav\\dev\\pwn-engine\\dist\\temp\\pwn\\c skel " << boneIndex << ".txt", skel);
					dump(Str() << "C:\\Users\\Gustav\\dev\\pwn-engine\\dist\\temp\\pwn\\d local " << boneIndex << ".txt", local);
				}

				//result[boneIndex] = math::mat44helper(parent).rotate(bone.rot).translate(bone.pos).translate(poseloc).rotate(-poserot).mat;
				result[boneIndex] = math::mat44helper(parent).mult(local).mat;
			}
			transforms = result;
		}

		AnimationInformation::AnimationInformation(int s, int e, const string& n)
			: start(s)
			, end(e)
			, name(n)
		{
		}

		real CalculateLength(const std::vector<AnimationPerBone>& bones)
		{
			real length = 0;
			for(std::size_t i=0; i<bones.size(); ++i)
			{
				const AnimationPerBone& ab = bones[i];
				length = math::Max(length, ab.getLength());
			}
			return length;
		}

		Animation::Animation()
			: length(0)
		{
		}

		Animation::Animation(const std::vector<AnimationPerBone>& abones)
			: length( CalculateLength(abones) )
			, bones(abones)
		{
		}

		void Animation::getPose(real time, Pose* out) const
		{
			std::vector<PosePerBone> bd;
			bd.resize(bones.size());
			for(std::size_t i=0; i<bones.size(); ++i)
			{
				const AnimationPerBone& ab = bones[i];
				bd[i] = ab.getBonePose(time);
			}
			out->bones = bd;
		}

		real Animation::getLength() const
		{
			return length;
		}

		void Animation::subanim(int start, int end, Animation* out) const
		{
			std::vector<AnimationPerBone> bd(bones.size());
			for(std::size_t i=0; i<bones.size(); ++i)
			{
				const AnimationPerBone& ab = bones[i];
				ab.sub(start, end, &bd[i]);
			}
			out->bones = bd;
			out->length = CalculateLength(bd);
		}

		void Animation::subanim(const AnimationInformation& info, Animation* out) const
		{
			subanim(info.start, info.end, out);
		}

		void Animation::scale(real scale)
		{
			for(std::size_t i=0; i<bones.size(); ++i)
			{
				AnimationPerBone& ab = bones[i];
				ab.scale(scale);
			}
		}
	}
}
