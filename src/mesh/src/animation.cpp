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

		math::vec3 FramePosition::value() const
		{
			return location;
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

		math::quat FrameRotation::value() const
		{
			return rotation;
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

		

		template<typename T, typename R>
		R Get(const std::vector<T>& da, real current)
		{
			Assert( false == da.empty() );

			if( current < da[0].getTime() )
			{
				return da[0].value();
			}

			for (std::size_t i = 1; i < da.size(); ++i)
			{
				if (math::IsWithinInclusive(da[i-1].getTime(), current, da[i].getTime()))
				{
					//return da[i-1].value();
					return Interpolate(da[i-1], current, da[i]);
				}
			}

			return da.rbegin()->value();
		};

		math::quat Interpolate(real time, const std::vector<FrameRotation>& fr)
		{
			return Get<FrameRotation, math::quat>(fr, time);
		}
		
		math::vec3 Interpolate(real time, const std::vector<FramePosition>& fp)
		{
			return Get<FramePosition, math::vec3>(fp, time);
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

		template<class T>
		void FillInterpolate(const std::vector<T>& data, std::vector<T>* v, int start, int end)
		{
			real length = end - start;
			bool first = true;
			real last = -1;

			for(std::size_t i=0; i<data.size(); ++i)
			{
				const T& fp = data[i];
				if (math::IsWithinInclusive(start, fp.getTime(), end))
				{
					real mark = fp.getTime()-start;
					if (first && math::IsZero(mark)==false)
					{
						v->push_back(T(0, Interpolate(start, data)));
					}
					mark = math::ZeroOrValue(mark);
					first = false;
					v->push_back(T(mark, fp.value()));
					last = mark;
				}
			}

			if (first)
			{
				v->push_back(T(0, Interpolate(start, data)));
			}

			if (math::IsEqual(length, last)==false )
			{
				v->push_back(T(length, Interpolate(end, data)));
			}

			if (v->size() < 2 ) throw "Data error, need atleast 2 keyframes per animation";
		}

		void AnimationPerBone::sub(int start, int end, AnimationPerBone* out) const
		{
			std::vector<FramePosition> abfp;
			std::vector<FrameRotation> abfr;

			FillInterpolate(fp, &abfp, start, end);
			FillInterpolate(fr, &abfr, start, end);

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
		
		CompiledPose::CompiledPose(const Pose& pose, const std::vector<Bone>& bones)
		{
			if (pose.bones.size() != bones.size()) throw "Invalid animation/mesh, bone count differs";
			std::vector<math::mat44> result( pose.bones.size() );
			for (std::size_t boneIndex = 0; boneIndex < pose.bones.size(); ++boneIndex)
			{
				const Bone& bone = bones[boneIndex];
				// either it has no parents, or it's parent has already been precoessed
				Assert( bone.hasParent()==false || boneIndex > bone.getParent() );
				math::mat44 parent = bone.hasParent() ? result[bone.getParent()] : math::mat44Identity();
				const math::vec3 poseloc = pose.bones[boneIndex].location;
				const math::quat poserot = pose.bones[boneIndex].rotation;

				const math::mat44 anim = math::mat44helper(math::mat44Identity()).translate(poseloc).rotate(GetConjugate(poserot)).mat;
				const math::mat44 skel = math::mat44helper(math::mat44Identity()).translate(bone.pos).rotate(bone.rot).mat;

				const math::mat44 local = math::mat44helper(math::mat44Identity()).mult(skel).mult(anim).mat;

				result[boneIndex] = math::mat44helper(parent).mult(skel).mult(anim).mat;
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
