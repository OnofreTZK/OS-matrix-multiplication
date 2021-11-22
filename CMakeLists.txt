cmake_minimum_required(VERSION 3.5)
project( matrix VERSION 0.1 LANGUAGES CXX)


# set the compiling flags
set ( CMAKE_CXX_FLAGS "-Wall -lpthread" )
#CMAKE_CXX_FLAGS = "-Wall";

# set the include path

# -------------------------------
# This is for old cmake versions
# set (CMAKE_CXX_STANDARD 17)
# -------------------------------

#set include path
include_directories ( include )

# define the sources for the project

add_executable ( genmtx
				 src/genmtx.cpp )

add_executable ( serialmtx
				 src/serialmtx.cpp )

add_executable ( threadmtx
				 src/threadmtx.cpp )

add_executable ( procmtx
				 src/procmtx.cpp )

# define C++17 standard
set_property (TARGET genmtx PROPERTY CXX_STANDARD 17)
set_property (TARGET serialmtx PROPERTY CXX_STANDARD 17)
set_property (TARGET threadmtx PROPERTY CXX_STANDARD 17)
set_property (TARGET procmtx PROPERTY CXX_STANDARD 17)

#end
