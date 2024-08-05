# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("gnunet")

# configure import libs
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../distribution)

add_library(libgnunetstatistics SHARED IMPORTED)
set_target_properties(libgnunetstatistics PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunetstatistics/lib/${ANDROID_ABI}/libgnunetstatistics.so)
add_library(libgnunettransportcore SHARED IMPORTED)
set_target_properties(libgnunettransportcore PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunettransportcore/lib/${ANDROID_ABI}/libgnunettransportcore.so)
add_library(libgnunettransportcommunicator SHARED IMPORTED)
set_target_properties(libgnunettransportcommunicator PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunettransportcommunicator/lib/${ANDROID_ABI}/libgnunettransportcommunicator.so)
add_library(libgnunettransportapplication SHARED IMPORTED)
set_target_properties(libgnunettransportapplication PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunettransportapplication/lib/${ANDROID_ABI}/libgnunettransportapplication.so)
add_library(libgnunettransportmonitor SHARED IMPORTED)
set_target_properties(libgnunettransportmonitor PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunettransportmonitor/lib/${ANDROID_ABI}/libgnunettransportmonitor.so)
add_library(libgnunetpeerstore SHARED IMPORTED)
set_target_properties(libgnunetpeerstore PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunetpeerstore/lib/${ANDROID_ABI}/libgnunetpeerstore.so)
add_library(libgnunetarm SHARED IMPORTED)
set_target_properties(libgnunetarm PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunetarm/lib/${ANDROID_ABI}/libgnunetarm.so)
add_library(libgnunethello SHARED IMPORTED)
set_target_properties(libgnunethello PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunethello/lib/${ANDROID_ABI}/libgnunethello.so)
add_library(libgnunetsq SHARED IMPORTED)
set_target_properties(libgnunetsq PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunetsq/lib/${ANDROID_ABI}/libgnunetsq.so)
add_library(libgpg-error SHARED IMPORTED)
set_target_properties(libgpg-error PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgpg-error/lib/${ANDROID_ABI}/libgpg-error.so)
add_library(libgcrypt SHARED IMPORTED)
set_target_properties(libgcrypt PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgcrypt/lib/${ANDROID_ABI}/libgcrypt.so)
add_library(libsodium SHARED IMPORTED)
set_target_properties(libsodium PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libsodium/lib/${ANDROID_ABI}/libsodium.so)
add_library(libtool SHARED IMPORTED)
set_target_properties(libtool PROPERTIES IMPORTED_LOCATION
${distribution_DIR}/libtool/lib/${ANDROID_ABI}/libltdl.so)
add_library(gmp SHARED IMPORTED)
set_target_properties(gmp PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/gmp/lib/${ANDROID_ABI}/libgmp.so)
add_library(libunistring SHARED IMPORTED)
set_target_properties(libunistring PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libunistring/lib/${ANDROID_ABI}/libunistring.so)
add_library(libz SHARED IMPORTED)
set_target_properties(libz PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libz/lib/${ANDROID_ABI}/libz.so)
add_library(libiconv SHARED IMPORTED)
set_target_properties(libiconv PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libiconv/lib/${ANDROID_ABI}/libiconv.so)
add_library(libintl SHARED IMPORTED)
set_target_properties(libintl PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libintl/lib/${ANDROID_ABI}/libintl.so)
add_library(libgettext SHARED IMPORTED)
set_target_properties(libgettext PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/gettext/lib/${ANDROID_ABI}/libgettextlib-0.22.5.so)
add_library(libgnunetutil SHARED IMPORTED)
set_target_properties(libgnunetutil PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libgnunetutil/lib/${ANDROID_ABI}/libgnunetutil.so)

# build application's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(gnunet SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
)

target_include_directories(gnunet PRIVATE
        ${distribution_DIR}/libsodium/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgcrypt/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgpg-error/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/gmp/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libunistring/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libz/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libtool/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libiconv/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libintl/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/gettext/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunetutil/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunetstatistics/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunettransportcore/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunettransportcommunicator/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunettransportapplication/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunettransportmonitor/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunetpeerstore/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunetarm/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunethello/lib/${ANDROID_ABI}/include
        ${distribution_DIR}/libgnunetsq/lib/${ANDROID_ABI}/include)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(gnunet
        # List libraries link to the target library
        android
        libgpg-error
        libgcrypt
        libsodium
        libtool
        gmp
        libunistring
        libz
        libiconv
        libintl
        libgettext
        libgnunetutil
        libgnunetstatistics
        libgnunettransportcore
        libgnunettransportcommunicator
        libgnunettransportapplication
        libgnunettransportmonitor
        libgnunetpeerstore
        libgnunetarm
        libgnunethello
        libgnunetsq
        log)