Chartreuse
==================================

Chartreuse is an audio descriptors library, (for now) based on [MPEG-7](https://en.wikipedia.org/wiki/MPEG-7).

Tests
-----

Tests rely on [Google Test Framework](http://code.google.com/p/googletest/).

All required Cmake variables are set up, and the GTest library is used as a submodule. In order to retrieve the library files, the following must be done from Chartreuse root folder:

    git submodule init
    git submodule update

Build
-----

The build system is based on Cmake.
It comes with two boolean (ON/OFF) options:
- CHARTREUSE_HAS_GTEST to indicate that GTest framework can be used (see above)

Building is done with:

    mkdir build && cd build
    cmake ..
    cmake --build .

Builds are continuously tested on gcc and Clang with [Travis CI](https://travis-ci.org/).
[![Build Status](https://travis-ci.org/G4m4/chartreuse.svg?branch=master)](https://travis-ci.org/G4m4/chartreuse)

License
==================================
Chartreuse is under GPLv3.

See the COPYING file for the full license text.
