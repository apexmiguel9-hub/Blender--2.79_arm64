#!/bin/bash
# apply_android_patches.sh
# Modifies Blender 2.79b source for Android build
set -euo pipefail

BLENDER_DIR="$1"
SOURCE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

echo "=== Applying Android modifications to Blender ==="

# 1. Copy GHOST Android backend
echo "[1/6] Copying GHOST Android backend..."
cp "$SOURCE_DIR/android/ghost/GHOST_SystemAndroid.h" "$BLENDER_DIR/intern/ghost/intern/"
cp "$SOURCE_DIR/android/ghost/GHOST_SystemAndroid.cpp" "$BLENDER_DIR/intern/ghost/intern/"
cp "$SOURCE_DIR/android/ghost/GHOST_WindowAndroid.h" "$BLENDER_DIR/intern/ghost/intern/"
cp "$SOURCE_DIR/android/ghost/GHOST_WindowAndroid.cpp" "$BLENDER_DIR/intern/ghost/intern/"

# 2. Add Android case to GHOST_ISystem.cpp
echo "[2/6] Patching GHOST_ISystem.cpp..."
sed -i 's/#ifdef WITH_X11$/#ifdef WITH_X11\n#elif defined(WITH_GHOST_ANDROID)\n\t\tm_system = new GHOST_SystemAndroid();/' \
    "$BLENDER_DIR/intern/ghost/intern/GHOST_ISystem.cpp"

# 3. Add Android files to GHOST CMakeLists.txt
echo "[3/6] Patching intern/ghost/CMakeLists.txt..."
sed -i '/^if(WITH_HEADLESS)$/i if(WITH_ANDROID_MODULE)\n\tadd_definitions(-DWITH_GHOST_ANDROID)\n\tadd_definitions(-DWITH_GL_EGL)\nendif()\n' \
    "$BLENDER_DIR/intern/ghost/CMakeLists.txt"

# Find the end of WIN32 section in the SRC list and add Android files
sed -i '/^if(WIN32)$/,/^endif()$/{
/^endif()$/a\
\
if(WITH_ANDROID_MODULE)\
\tlist(APPEND SRC\
\t\tintern\/GHOST_SystemAndroid.cpp\
\t\tintern\/GHOST_SystemAndroid.h\
\t\tintern\/GHOST_WindowAndroid.cpp\
\t\tintern\/GHOST_WindowAndroid.h\
\t)\
endif()
}' "$BLENDER_DIR/intern/ghost/CMakeLists.txt"

# 4. Add WITH_ANDROID_MODULE to root CMakeLists.txt
echo "[4/6] Patching root CMakeLists.txt..."
# Add option definition after WITH_PYTHON_MODULE
sed -i '/^option(WITH_PYTHON_MODULE/,/^)/a\
option(WITH_ANDROID_MODULE "Build Blender as Android shared library (.so)" OFF)' \
    "$BLENDER_DIR/CMakeLists.txt"

# Add GL4ES support block (skip desktop OpenGL for Android)
sed -i '/^# Configure OpenGL\.$/a\
\
if(WITH_ANDROID_MODULE)\
\tset(GL4ES_LIBRARY "" CACHE FILEPATH "Path to GL4ES libGL.a")\
\tset(GL4ES_INCLUDE_DIR "" CACHE PATH "Path to GL4ES include dir")\
\tmark_as_advanced(GL4ES_LIBRARY GL4ES_INCLUDE_DIR)\
endif()' "$BLENDER_DIR/CMakeLists.txt"

# 5. Build as shared library in source/creator/CMakeLists.txt
echo "[5/6] Patching source/creator/CMakeLists.txt..."
sed -i '/^if(WITH_PYTHON_MODULE)$/a\
if(WITH_ANDROID_MODULE)\
\tadd_definitions(-DWITH_ANDROID_MODULE)\
endif()' "$BLENDER_DIR/source/creator/CMakeLists.txt"

# Change add_executable to add_library for Android
sed -i 's/^\(	else()$\|	else()$\)\n\tadd_executable(blender ${EXETYPE} ${SRC})/\
	elseif(WITH_ANDROID_MODULE)\
		add_library(blender SHARED ${SRC})\
		set_target_properties(blender PROPERTIES OUTPUT_NAME blender PREFIX "lib")\
	else()\
		add_executable(blender ${EXETYPE} ${SRC})/' \
    "$BLENDER_DIR/source/creator/CMakeLists.txt"

# 6. Make the blend target depend on our new files
echo "[6/6] Patching creator.c for Android entry..."
sed -i 's/^#ifdef WITH_PYTHON_MODULE$/#if defined(WITH_PYTHON_MODULE) || defined(WITH_ANDROID_MODULE)/' \
    "$BLENDER_DIR/source/creator/creator.c"

echo "=== Android modifications applied ==="
