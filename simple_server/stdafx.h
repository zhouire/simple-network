// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>

#include "targetver.h"


//boost files
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
//boost files over



//#include "GlobalStructs.h"

#include <stdio.h>
#include <tchar.h>


//#include "GL/CAPI_GLE.h"
#include "Extras/OVR_Math.h"
//#include "OVR_CAPI_GL.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp>

#include "GlobalStructs.h"

// TODO: reference additional headers your program requires here
