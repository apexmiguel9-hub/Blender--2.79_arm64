# Platform configuration for Android cross-compilation

# Zlib - required
find_package(ZLIB REQUIRED)
add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE)

# Platform link libraries for Android NDK
list(APPEND PLATFORM_LINKLIBS -lc -lm -ldl -llog)
find_package(Threads REQUIRED)
list(APPEND PLATFORM_LINKLIBS ${CMAKE_THREAD_LIBS_INIT})
set(PTHREADS_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# JPEG (optional)
find_package(JPEG QUIET)
if(NOT JPEG_FOUND)
    set(JPEG_LIBRARIES "")
    set(WITH_IMAGE_JPEG OFF)
endif()

# PNG (optional)
find_package(PNG QUIET)
if(NOT PNG_FOUND)
    set(PNG_LIBRARIES "")
    set(WITH_IMAGE_PNG OFF)
endif()

# Freetype (optional)
find_package(Freetype QUIET)
if(NOT FREETYPE_FOUND)
    set(FREETYPE_LIBRARY "")
endif()
