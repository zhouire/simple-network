#pragma once

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive & ar, glm::quat & g, const unsigned int version)
		{
			ar & g.x;
			ar & g.y;
			ar & g.z;
			ar & g.w;
		}


		template<class Archive>
		void serialize(Archive & ar, glm::vec3 & vec, const unsigned int version)
		{
			ar & boost::serialization::make_nvp("x", vec.x);
			ar & boost::serialization::make_nvp("y", vec.y);
			ar & boost::serialization::make_nvp("z", vec.z);
		}

		template<class Archive>
		void serialize(Archive & ar, OVR::Vector3f & v, const unsigned int version)
		{
			ar & v.x;
			ar & v.y;
			ar & v.z;
		}

		template<class Archive>
		void serialize(Archive & ar, OVR::Matrix4f & m, const unsigned int version)
		{
			ar & m.M;
		}


		template<class Archive>
		void serialize(Archive & ar, Part & p, const unsigned int version)
		{
			ar & p.N;
		}


	} // namespace serialization
} // namespace boost