set(CMAKE_CXX_STANDARD "11")
# set(CMAKE_CXXFLAGS "-std=c++11")

include(ExternalProject)
# Set CMake minimum version and CMake policy required by Urho3D-CMake-common module
if (WIN32)
    cmake_minimum_required (VERSION 3.2.3)      # Going forward all platforms will use this as minimum version
else ()
    cmake_minimum_required (VERSION 3.2.3)
endif ()
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 3.2.3 OR CMAKE_VERSION VERSION_EQUAL 3.2.3)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.2.3 OR CMAKE_VERSION VERSION_EQUAL 3.2.3)
        # Disallow use of the LOCATION target property - so we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

# Set project name
project (Scaffolding)

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Include Urho3D CMake common module
include (Urho3D-CMake-common)

# Define target name
set (TARGET_NAME Main)

# Define source files
define_source_files ()

# Setup target with resource copying
setup_main_executable ()

# Setup test cases
if (URHO3D_ANGELSCRIPT)
    setup_test (NAME ExternalLibAS OPTIONS Scripts/12_PhysicsStressTest.as -w)
endif ()
if (URHO3D_LUA)
    setup_test (NAME ExternalLibLua OPTIONS LuaScripts/12_PhysicsStressTest.lua -w)
endif ()

set(LIBPD_ROOT ${CMAKE_BINARY_DIR}/thirdparty/libpd)

ExternalProject_Add(
    libpd_external
    PREFIX ${LIBPD_ROOT}
    GIT_REPOSITORY https://github.com/libpd/libpd.git
    GIT_SUBMODULES
    GIT_TAG 0.8.3
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    BINARY_DIR ${LIBPD_ROOT}/src/libpd
    SOURCE_DIR ${LIBPD_ROOT}/src/libpd
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make cpplib UTIL=true EXTRA=true
    INSTALL_COMMAND ""
)

include_directories(${LIBPD_ROOT}/src/libpd/cpp)
include_directories(${LIBPD_ROOT}/src/libpd/pure-data/src)
include_directories(${LIBPD_ROOT}/src/libpd/libpd_wrapper)
target_link_libraries(${TARGET_NAME} ${LIBPD_ROOT}/src/libpd/libs/libpdcpp.dylib)


set(RTAUDIO_ROOT ${CMAKE_BINARY_DIR}/thirdparty/rtaudio)
ExternalProject_Add(
    rtaudio_external
    PREFIX ${RTAUDIO_ROOT}
    URL http://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-4.1.2.tar.gz
    INSTALL_COMMAND ""
)

include_directories(${RTAUDIO_ROOT}/src/rtaudio_external)
target_link_libraries(${TARGET_NAME} ${RTAUDIO_ROOT}/src/rtaudio_external-build/librtaudio_static.a)
