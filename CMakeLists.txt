PROJECT(PseudoCountAtari)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "-std=c++14 -O3")

SET(Boost_NO_SYSTEM_PATHS OFF)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.53.0 REQUIRED COMPONENTS system)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# x86_64 Linux cannot use static libs inside of a dynamic lib.
IF (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    SET(Boost_USE_STATIC_LIBS ON)
ENDIF()

FILE(GLOB cppfiles src/*.cpp src/*.hpp src/*.h)

ADD_LIBRARY(pseudocount MODULE ${cppfiles})
TARGET_LINK_LIBRARIES(pseudocount ${Boost_LIBRARIES}) 

install(TARGETS pseudocount
        ARCHIVE DESTINATION ${LUA_CPATH} 
        LIBRARY DESTINATION ${LUA_CPATH} )

install(FILES init.lua DESTINATION ${LUA_PATH}/pseudocount)
