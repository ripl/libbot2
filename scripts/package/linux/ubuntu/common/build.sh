#!/bin/bash

# This script is not designed to be run manually. It is used when creating a
# libbot2 debian package with the script called `package` which should be in
# the same directory.
# This script clones LCM's git repository, compiles and packages LCM, and
# compiles and packages libbot2.

set -euxo pipefail

if [ -z "$1" ]; then
  readonly timestamp=$(date -u +%Y%m%d)
else
  readonly timestamp=$1
fi

pushd /tmp
# LCM is built and package as part of this process as there is no
# official LCM package.
git clone --branch v1.4.0 --config advice.detachedHead=false --depth 1 https://github.com/lcm-proj/lcm.git

pushd lcm
cat << 'EOF' > lcm-cmake.patch
diff --git a/lcm-cmake/cpack.cmake b/lcm-cmake/cpack.cmake
index 253ab64..52ec648 100644
--- a/lcm-cmake/cpack.cmake
+++ b/lcm-cmake/cpack.cmake
@@ -63,5 +63,5 @@ set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
 set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/lcm-proj/lcm")
 set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
-set(CPACK_DEBIAN_PACKAGE_DEPENDS "libglib2.0-0, libpcre3")
+set(CPACK_DEBIAN_PACKAGE_DEPENDS "default-jre | java8-runtime, libc6, libgcc1, libglib2.0-0, libpcre3, libstdc++6, python")

 message(STATUS "CPack: Packages will be placed under ${CPACK_PACKAGE_DIRECTORY}")
EOF
git apply lcm-cmake.patch
rm -f lcm-cmake.patch
popd

mkdir lcm-build
pushd lcm-build

# Forces the package to be installed in /opt/lcm/<version>
# to be consistent with the fact these packages are not "official".
cmake -DBUILD_SHARED_LIBS:BOOL=ON \
      -DCMAKE_BUILD_TYPE:STRING=Release \
      -DCMAKE_CXX_FLAGS:STRING="$(dpkg-buildflags --get CXXFLAGS) $(dpkg-buildflags --get CPPFLAGS)" \
      -DCMAKE_C_FLAGS:STRING="$(dpkg-buildflags --get CFLAGS) $(dpkg-buildflags --get CPPFLAGS)" \
      -DCMAKE_SHARED_LINKER_FLAGS:STRING="$(dpkg-buildflags --get LDFLAGS)" \
      -DCPACK_DEBIAN_PACKAGE_VERSION:STRING=1.4.0 \
      -DCPACK_DEBIAN_PACKAGE_MAINTAINER:STRING="Kitware <kitware@kitware.com>" \
      -DCPACK_PACKAGING_INSTALL_PREFIX:PATH=/opt/lcm/1.4.0 \
      -DCMAKE_C_FLAGS:STRING=-Wl,-rpath,\$ORIGIN/../lib \
      -DCMAKE_CXX_FLAGS:STRING=-Wl,-rpath,\$ORIGIN/../lib \
      -DCMAKE_INSTALL_PREFIX:PATH=/opt/lcm/1.4.0 \
      -DLCM_ENABLE_EXAMPLES:BOOL=OFF \
      -DLCM_ENABLE_TESTS:BOOL=OFF \
      ../lcm
make
cpack -G DEB
popd
mv lcm-build/packages/lcm_1.4.0-1_amd64.deb lcm_1.4.0-1_amd64.deb
rm -rf lcm-build

# Install the package instead of running `make install`. This
# ensures that LCM is installed in the same location when libbot2
# is built and its package is installed. This is necessary because
# some scripts such as `bot-spy` rely on `lcm-spy` and its path
# is hardcoded at compile time.
dpkg -i lcm_1.4.0-1_amd64.deb

# Check if package versions are specified in environment variable
LIBPNG_VERSION=""
if [ `echo "$CPACK_LIBPNG_VERSION"` != "" ]; then
  LIBPNG_VERSION="-DCPACK_LIBPNG_VERSION:STRING=${CPACK_LIBPNG_VERSION}"
fi
CMAKE_PACKAGE_OPENGL=""
if [ `echo "$PACKAGE_OPENGL"` == TRUE ]; then
  CMAKE_PACKAGE_OPENGL="-DPACKAGE_OPENGL:BOOL=TRUE"
fi
# Configure, compile, and package libbot2
mkdir libbot2-build
pushd libbot2-build
cmake -DPACKAGE_LIBBOT2:BOOL=ON \
      -DCMAKE_BUILD_TYPE:STRING=Release \
      -DCMAKE_CXX_FLAGS:STRING="$(dpkg-buildflags --get CXXFLAGS) $(dpkg-buildflags --get CPPFLAGS)" \
      -DCMAKE_C_FLAGS:STRING="$(dpkg-buildflags --get CFLAGS) $(dpkg-buildflags --get CPPFLAGS)" \
      -DCMAKE_PREFIX_PATH:PATH=/opt/lcm/1.4.0 \
      -DCMAKE_SHARED_LINKER_FLAGS:STRING="$(dpkg-buildflags --get LDFLAGS)" \
      $LIBPNG_VERSION \
      $CMAKE_PACKAGE_OPENGL \
      ../libbot2
make
cpack -G DEB
popd
mv libbot2-build/packages/libbot2_0.0.1.${timestamp}-1_amd64.deb libbot2_0.0.1.${timestamp}-1_amd64.deb
rm -rf libbot2-build
