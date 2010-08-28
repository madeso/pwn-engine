#pragma warning(disable: 4244) // conversion, possible loss of data

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include <pwn/core/stdutil.h>
#include <boost/foreach.hpp>

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
			if (math::IsWithin(0, scale, 1) == false) throw "invalid scale";
			return math::Lerp(from.location, scale, to.location);
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
			if (math::IsWithin(0, scale, 1) == false) throw "invalid scale";
			return math::SlerpShortway(from.rotation, scale, to.rotation);
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

		math::quat Interpolate(real time, const core::Vector<FrameRotation>& fr)
		{
			const int fri = Get(fr, time);
			if (fri == -1) return math::qIdentity();
			const math::quat r( Interpolate(fr[fri - 1], time, fr[fri]) );
			return r;
		}
		
		math::vec3 Interpolate(real time, const core::Vector<FramePosition>& fp)
		{
			const int fpi = Get(fp, time);
			if (fpi == -1) return math::Origo3().vec;
			const math::vec3 res( Interpolate(fp[fpi - 1], time, fp[fpi]) );
			return res;
		}

		AnimationPerBone::AnimationPerBone(core::Vector<FramePosition>& afp, core::Vector<FrameRotation>& afr)
		{
			fp.swap(afp);
			fr.swap(afr);
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

		AnimationPerBone AnimationPerBone::sub(int start, int end) const
		{
			std::vector<FramePosition> abfp;
			std::vector<FrameRotation> abfr;
			real length = end - start;
			bool first = true;
			real last = 0;

			BOOST_FOREACH(const FramePosition& fp, this->fp)
			{
				if (math::IsWithin(start, fp.getTime(), end))
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

			BOOST_FOREACH(const FrameRotation& fr, this->fr)
			{
				if (math::IsWithin(start, fr.getTime(), end))
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
			return AnimationPerBone(core::Vector<FramePosition>(abfp), core::Vector<FrameRotation>(abfr));
		}

		void AnimationPerBone::scale(real scale)
		{
			BOOST_FOREACH(FramePosition& f, fp)
			{
				f.location *= scale;
			}
		}

		Pose::Pose(core::Vector<PosePerBone>& apose)
			: bones(apose)
		{
		}

		void UpdateMatrix(core::Vector<math::mat44>& result, const Bone& bone, const Pose& pose, const std::vector<Bone>& list)
		;/*{
			mat44 parent;
			if (bone.hasParent() == false) parent = mat44.Identity;
			else parent = result[bone.parent];
			vec3 loc = pose.bones[bone.index].location;
			quat rot = pose.bones[bone.index].rotation;
			// bone.pos Rotate(-bone.rot).
			result[bone.index] = new MatrixHelper(parent).Rotate(bone.rot).Translate(bone.pos).Translate(loc).Rotate(-rot).mat44;
			BOOST_FOREACH(Bone b, bone.childs)
			{
				updateMatrix(ref result, b, pose, list);
			}
		}*/

		CompiledPose::CompiledPose(const Pose& pose, const Mesh& def)
		{
			if (pose.bones.size() != def.bones.size()) throw "Invalid animation/mesh, bone count differs";
			core::Vector<math::mat44> result( pose.bones.size() );
			for (int i = 0; i < pose.bones.size(); ++i) result[i] = math::mat44Identity();
			BOOST_FOREACH(const Bone& b, def.bones)
			{
				UpdateMatrix(result, b, pose, def.bones);
			}
			transforms.swap(result);
		}

		AnimationInformation::AnimationInformation(int s, int e, const string& n)
			: start(s)
			, end(e)
			, name(n)
		{
		}

		real CalculateLength(const core::Vector<AnimationPerBone>& bones)
		{
			real length = 0;
			BOOST_FOREACH(const AnimationPerBone& ab, bones)
			{
				length = math::Max(length, ab.getLength());
			}
			return length;
		}

		Animation::Animation(core::Vector<AnimationPerBone>& abones)
			: length( CalculateLength(abones) )
		{
			bones.swap(abones);
		}

		Pose Animation::getPose(real time) const
		{
			core::Vector<PosePerBone> bd;
			bd.reset(bones.size());
			int i=0;
			BOOST_FOREACH(const AnimationPerBone& ab, bones)
			{
				bd[i] = ab.getBonePose(time);
				++i;
			}
			return Pose(bd);
		}

		Animation Animation::subanim(int start, int end) const
		{
			core::Vector<AnimationPerBone> bd(bones.size());
			int index = 0;
			BOOST_FOREACH(const AnimationPerBone& ab, bones)
			{
				bd[index] = ab.sub(start, end);
				++index;
			}
			return Animation(bd);
		}

		Animation Animation::subanim(const AnimationInformation& info) const
		{
			return subanim(info.start, info.end);
		}

		void Animation::scale(real scale)
		{
			BOOST_FOREACH(AnimationPerBone& afb, bones)
			{
				afb.scale(scale);
			}
		}
	}
}
