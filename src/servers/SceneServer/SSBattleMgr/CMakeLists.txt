SET(ProjectName SSBattleMgr)
PROJECT(${ProjectName})
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE_DIRECTORIES($ENV{ServerRoot}/CommonCPlus)
INCLUDE_DIRECTORIES($ENV{ServerRoot}/CommonCPlus/Include)
INCLUDE_DIRECTORIES($ENV{ServerRoot}/CommonCPlus/CommonCPlus)
INCLUDE_DIRECTORIES($ENV{ServerRoot}/CommonCPlus/CommonCPlus/boost)
INCLUDE_DIRECTORIES($ENV{ServerRoot}/CommonCPlus/CommonCPlus/LuaPlus)
INCLUDE_DIRECTORIES($ENV{ServerRoot}/CommonCPlus/CommonCPlus/LuaPlus/src)
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(..)
LINK_DIRECTORIES( $ENV{ServerRoot}/Bin/$ENV{Platform}/$ENV{Configuration})
LINK_DIRECTORIES( $ENV{ServerRoot}/Lib/$ENV{Platform}/$ENV{Configuration})
SET(LIBRARY_OUT_PATH $ENV{ServerRoot}/Bin/$ENV{Platform}/$ENV{Configuration})
SET(EXECUTABLE_OUT_PATH $ENV{ServerRoot}/Bin/$ENV{Platform}/$ENV{Configuration})
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
AUX_SOURCE_DIRECTORY(../../../ThirdFunc/LuaPlus LuaPlusDir1)
AUX_SOURCE_DIRECTORY(../../../ThirdFunc/LuaPlus/src LuaPlusDir2)

SET(CMAKE_CXX_COMPILER ${CompilerSetting})
add_definitions(-std=c++11)
IF ($ENV{Configuration} STREQUAL Debug)
	SET(CMAKE_BUILD_TYPE Debug)
ELSE()
	SET(CMAKE_BUILD_TYPE Release)
ENDIF()

IF ($ENV{Configuration} STREQUAL Debug)
	#add_definitions(-g -lpthread)	
	SET(CMAKE_CXX_FLAGS_DEBU "$ENV{CXXFLAGS} -O0 -Wall -fPIC")
ELSE()
	SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
ENDIF()

SET(newDir  ${DIR_SRCS} ${LuaPlusDir1} ${LuaPlusDir2})
ADD_LIBRARY(${ProjectName} SHARED ${newDir})