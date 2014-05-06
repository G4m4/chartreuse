Chartreuse
==================================

Chartreuse is an audio descriptor library

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

A Travis configuration file is provided in order to be used with [Travis CI](https://travis-ci.org/).

Building is done with:

    mkdir build && cd build
    cmake ..
    cmake --build .

License
==================================
Chartreuse is under GPLv3.

See the COPYING file for the full license text.
