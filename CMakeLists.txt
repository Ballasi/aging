cmake_minimum_required (VERSION 2.6)

project (aging)

include_directories("src")

# search for location of header gmpxx.h", only at positions given by the user
find_path(GMPXX_INCLUDE_DIR
  NAMES "gmpxx.h"
  PATHS ${GMP_PREFIX} ${GMP_ROOT}
  PATH_SUFFIXES include
  NO_DEFAULT_PATH)
# try default paths now
find_path(GMPXX_INCLUDE_DIR
  NAMES "gmpxx.h")

# check if header is accepted
include(CMakePushCheckState)
cmake_push_check_state()
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${GMPXX_INCLUDE_DIR})
include(CheckIncludeFileCXX)
check_include_file_cxx("gmpxx.h" GMP_HEADER_WORKS)

# look for library gmp, only at positions given by the user
find_library(GMP_LIB gmp
  PATHS ${GMP_PREFIX} ${GMP_ROOT}
  PATH_SUFFIXES lib lib64
  NO_DEFAULT_PATH
  DOC "GNU GMP library")
# try default paths now
find_library(GMP_LIB gmp)

# look for library gmpxx, only at positions given by the user
find_library(GMPXX_LIB gmpxx
  PATHS ${GMP_PREFIX} ${GMP_ROOT}
  PATH_SUFFIXES lib lib64
  NO_DEFAULT_PATH
  DOC "GNU GMPXX library")
# try default paths now
find_library(GMPXX_LIB gmpxx)

# behave like a CMake module is supposed to behave
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  "GMP"
  DEFAULT_MSG
  GMPXX_INCLUDE_DIR GMP_LIB GMPXX_LIB GMP_HEADER_WORKS GMPXX_LIB_WORKS
)

mark_as_advanced(GMP_LIB GMPXX_LIB GMPXX_INCLUDE_DIR)

# if GMPxx headers, GMP library, and GMPxx library are found, store results
if(GMP_FOUND)
  set(GMP_INCLUDE_DIRS ${GMPXX_INCLUDE_DIR})
  set(GMP_LIBRARIES ${GMP_LIB} ${GMPXX_LIB})
  set(GMP_COMPILE_FLAGS "-DENABLE_GMP=1")
  # log result
  file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
    "Determining location of GMP, GMPxx succeeded:\n"
    "Include directory: ${GMP_INCLUDE_DIRS}\n"
    "Library directory: ${GMP_LIBRARIES}\n\n")
else()
  # log errornous result
  file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
    "Determining location of GMP, GMPxx failed:\n"
    "Include directory: ${GMPXX_INCLUDE_DIR}\n"
    "gmp library directory: ${GMP_LIB}\n"
    "gmpxx library directory: ${GMPXX_LIB}\n\n")
endif()


find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender 3DInput Charts)

set(CMAKE_AUTOMOC ON)

file(
    GLOB_RECURSE

    sources_files

    src/*
    )

add_executable(aging ${sources_files})

target_link_libraries(aging ${GMP_LIB} ${GMPXX_LIB})

target_link_libraries(aging PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
    Qt5::Charts
)
