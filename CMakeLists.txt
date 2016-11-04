cmake_minimum_required(VERSION 3.0)

project(EDFlib-qt VERSION 1.0)

list(APPEND CMAKE_PREFIX_PATH C:/Qt/5.7/msvc2013)
find_package(Qt5 COMPONENTS Core)

# Let's do the cmake job for us
set(CMAKE_AUTOMOC OFF) # For meta object compiler
set(CMAKE_AUTORCC ON) # Resource files
set(CMAKE_AUTOUIC ON) # UI files

set(BOOST_COMPILER "vc120")
set(BOOST_VERSION "1_57")
set(BOOST_CONFIG_DEBUG "mt-gd")
set(BOOST_CONFIG_RELEASE "mt")

set(BOOST_DIR C:/boost_${BOOST_VERSION}_0)
set(BOOST_ROOT C:/boost_${BOOST_VERSION}_0)
set(BOOST_LIBRARYDIR ${BOOST_DIR}/stage/lib)
set(BOOST_INCLUDEDIR ${BOOST_DIR}/boost)
list(APPEND CMAKE_PREFIX_PATH ${BOOST_DIR} ${BOOST_LIBRARYDIR})
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost
             COMPONENTS chrono
                        system
                        math_c99
                        container
             REQUIRED)
# --------------------------------------------------------------------------
# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

# Enable Large file support
#include(TestLargeFiles)
#OPJ_TEST_LARGE_FILES(OPJ_HAVE_LARGEFILES)

file( MAKE_DIRECTORY "${BINARY_DIR}/Debug")
file( MAKE_DIRECTORY "${BINARY_DIR}/Release")


set(EDFlib_DIR ${CMAKE_SOURCE_DIR})
set(EDFlib_INCLUDE_DIRS  ${EDFlib_DIR}/src)
set(EDFlib_DEBUG_LIBRARIES  ${EDFlib_DIR}/build/Debug/EDFlibd.lib)
set(EDFlib_RELEASE_LIBRARIES  ${EDFlib_DIR}/build/Release/EDFlib.lib)

add_custom_target(Qt5CopyBinaries

  # todo: check if debug and release folder exist
  # debug version
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll         ${BINARY_DIR}/Debug
  
  # release version

  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll          ${BINARY_DIR}/Release

  # Output Message
  COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${BINARY_DIR}'" VERBATIM
)


add_subdirectory(src ${CMAKE_SOURCE_DIR}/build)
add_subdirectory(unitTests ${CMAKE_SOURCE_DIR}/buildUnitTest)
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/buildUnitTest/EDFFileTests)
add_dependencies(edfFileTests Qt5CopyBinaries)