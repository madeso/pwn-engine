#include <pwn/component/propertystd.h>

namespace pwn
{
	namespace component
	{
		namespace property
		{
			namespace
			{
				std::string InvalidType()
				{
					return "invalid type";
				}

				//////////////////////////////////////////////////////////////////////////////////////////////
				// String property

				class PropertyString
					: public Property
				{
				public:
					string v;
					PropertyString(const string& s)
						: v(s)
					{
					}

					const string getString() const
					{
						return v;
					}
					const real getReal() const
					{
						throw InvalidType();
					}
					const math::vec3 getVec3() const
					{
						throw InvalidType();
					}
					const math::quat getQuat() const
					{
						throw InvalidType();
					}

					string& refString()
					{
						return v;
					}
					real& refReal()
					{
						throw InvalidType();
					}
					math::vec3& refVec3()
					{
						throw InvalidType();
					}
					math::quat& refQuat()
					{
						throw InvalidType();
					}

					void setString(const string& s)
					{
						v = s;
					}
					void setReal(real r)
					{
						throw InvalidType();
					}
					void setVec3(const math::vec3& v)
					{
						throw InvalidType();
					}
					void setQuat(const math::quat& q)
					{
						throw InvalidType();
					}
				};

				//////////////////////////////////////////////////////////////////////////////////////////////
				// String property

				class PropertyReal
					: public Property
				{
				public:
					real v;
					PropertyReal(const real& s)
						: v(s)
					{
					}

					const string getString() const
					{
						throw InvalidType();
					}
					const real getReal() const
					{
						throw v;
					}
					const math::vec3 getVec3() const
					{
						throw InvalidType();
					}
					const math::quat getQuat() const
					{
						throw InvalidType();
					}

					string& refString()
					{
						throw InvalidType();
					}
					real& refReal()
					{
						return v;
					}
					math::vec3& refVec3()
					{
						throw InvalidType();
					}
					math::quat& refQuat()
					{
						throw InvalidType();
					}

					void setString(const string& s)
					{
						throw InvalidType();
					}
					void setReal(real r)
					{
						v = r;
					}
					void setVec3(const math::vec3& v)
					{
						throw InvalidType();
					}
					void setQuat(const math::quat& q)
					{
						throw InvalidType();
					}
				};

				//////////////////////////////////////////////////////////////////////////////////////////////
				// String property

				class PropertyVec3
					: public Property
				{
				public:
					math::vec3 v;
					PropertyVec3(const math::vec3& s)
						: v(s)
					{
					}

					const string getString() const
					{
						throw InvalidType();
					}
					const real getReal() const
					{
						throw InvalidType();
					}
					const math::vec3 getVec3() const
					{
						return v;
					}
					const math::quat getQuat() const
					{
						throw InvalidType();
					}

					string& refString()
					{
						throw InvalidType();
					}
					real& refReal()
					{
						throw InvalidType();
					}
					math::vec3& refVec3()
					{
						return v;
					}
					math::quat& refQuat()
					{
						throw InvalidType();
					}

					void setString(const string& s)
					{
						throw InvalidType();
					}
					void setReal(real r)
					{
						throw InvalidType();
					}
					void setVec3(const math::vec3& a)
					{
						v = a;
					}
					void setQuat(const math::quat& q)
					{
						throw InvalidType();
					}
				};

				//////////////////////////////////////////////////////////////////////////////////////////////
				// Quat property

				class PropertyQuat
					: public Property
				{
				public:
					math::quat v;
					PropertyQuat(const math::quat& s)
						: v(s)
					{
					}

					const string getString() const
					{
						throw InvalidType();
					}
					const real getReal() const
					{
						throw InvalidType();
					}
					const math::vec3 getVec3() const
					{
						throw InvalidType();
					}
					const math::quat getQuat() const
					{
						return v;
					}

					string& refString()
					{
						throw InvalidType();
					}
					real& refReal()
					{
						throw InvalidType();
					}
					math::vec3& refVec3()
					{
						throw InvalidType();
					}
					math::quat& refQuat()
					{
						return v;
					}

					void setString(const string& s)
					{
						throw InvalidType();
					}
					void setReal(real r)
					{
						throw InvalidType();
					}
					void setVec3(const math::vec3& v)
					{
						throw InvalidType();
					}
					void setQuat(const math::quat& q)
					{
						v = q;
					}
				};

				template<typename Prop, typename Base>
				boost::shared_ptr<Property> CreateProperty(const Base& s)
				{
					boost::shared_ptr<Property> p( new Prop(s) );
					return p;
				}
			}

			boost::shared_ptr<Property> CreateString(const string& s)
			{
				return CreateProperty<PropertyString>(s);
			}
			boost::shared_ptr<Property> CreateReal(real r)
			{
				return CreateProperty<PropertyReal>(r);
			}
			boost::shared_ptr<Property> CreateVec3(const math::vec3& v)
			{
				return CreateProperty<PropertyVec3>(v);
			}
			boost::shared_ptr<Property> CreateQuat(const math::quat& q)
			{
				return CreateProperty<PropertyQuat>(q);
			}
		}
	}
}
