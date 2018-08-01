#pragma once

using namespace std;

template<typename _Archive, typename _T1, typename _T2, typename _Alloc>
inline void save(_Archive &ar, boost::interprocess::basic_string<_T1, _T2, _Alloc> const str, unsigned int const version)
{
	boost::serialization::stl::save_collection<
		_Archive,
		boost::interprocess::basic_string<_T1, _T2, _Alloc>>(ar, str);
}


template<typename _Archive, typename _T1, typename _T2, typename _Alloc>
inline void load(_Archive &ar, boost::interprocess::basic_string<_T1, _T2, _Alloc> &str, unsigned int const version)
{
	boost::archive::library_version_type const library_version{
		ar.get_library_version()
	};

	boost::serialization::item_version_type item_version{ 0 };
	boost::serialization::collection_size_type count;
	ar >> BOOST_SERIALIZATION_NVP(count);

	if (boost::archive::library_version_type(3) < library_version)
	{
		ar >> BOOST_SERIALIZATION_NVP(item_version);
	}

	str.reserve(count);
	boost::serialization::stl::collection_load_impl(ar, str, count, item_version);
}


template<typename _Archive, typename _T1, typename _T2, typename _Alloc>
inline void serialize(_Archive &ar, boost::interprocess::basic_string<_T1, _T2, _Alloc> &str, unsigned int const version)
{
	boost::serialization::split_member(ar, str, version);
}