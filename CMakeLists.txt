PROJECT(PseudoCountAtari)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)


# x86_64 Linux cannot use static libs inside of a dynamic lib.
IF (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    SET(Boost_USE_STATIC_LIBS ON)
ENDIF()

set(CMAKE_CXX_FLAGS "-std=c++14 -O4 -g")

SET(Boost_NO_SYSTEM_PATHS OFF)
SET(Boost_USE_STATIC_RUNTIME OFF)

FIND_PACKAGE(Boost 1.55.0 REQUIRED COMPONENTS system)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FILE(GLOB cppfiles src/*.cpp src/*.hpp src/*.h)
ADD_LIBRARY(pseudocount SHARED ${cppfiles})

TARGET_LINK_LIBRARIES(pseudocount ${Boost_LIBRARIES}) 

ADD_EXECUTABLE(atari-count test/main.cpp)

TARGET_LINK_LIBRARIES(atari-count pseudocount) 

