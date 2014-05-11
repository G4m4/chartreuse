/// @file tests.h
/// @brief Tests common include file
/// @author gm
/// @copyright gm 2014
///
/// This file is part of Chartreuse
///
/// Chartreuse is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// Chartreuse is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with Chartreuse.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CHARTREUSE_TESTS_TESTS_H_
#define CHARTREUSE_TESTS_TESTS_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <random>
#include <vector>

#include "gtest/gtest.h"

#include "chartreuse/src/configuration.h"

static const unsigned int kDataTestSetSize = 64 * 1024;
static const unsigned int kIterationTestSetLength = 1024;

// Smaller performance test sets in debug
#if (_BUILD_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize = 64 * 1024;
#else  // (_BUILD_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize = 64 * 1024 * 1024;
#endif  // (_BUILD_CONFIGURATION_DEBUG)

static const unsigned int kDefaultSamplingRate = 48000;
static const float kDefaultTestFreq = 3520.0f;  // 8 * 440

static const unsigned int kSmallDFTLength = 64;
static const unsigned int kMediumDFTLength = 512;
static const unsigned int kLargeDFTLength = 4096;

static const float kDataInSin[] = {
#include "chartreuse/tests/data/data_in_sin.dat"
};

static const float kDataSmallSinDFT[] = {
#include "chartreuse/tests/data/dft_small.dat"
};
static const float kDataMediumSinDFT[] = {
#include "chartreuse/tests/data/dft_medium.dat"
};
static const float kDataLargeSinDFT[] = {
#include "chartreuse/tests/data/dft_large.dat"
};

// Smaller input data set in debug
#if (CHTZ_CONFIGURATION_DEBUG)
static const size_t kDataInSinLength = sizeof(kDataInSin)
                                        / (8 * sizeof(kDataInSin[0]));
#else  // (CHTZ_CONFIGURATION_DEBUG)
static const size_t kDataInSinLength = sizeof(kDataInSin)
                                        / sizeof(kDataInSin[0]);
#endif  // (CHTZ_CONFIGURATION_DEBUG)

const std::vector<float> kInSin(kDataInSin, kDataInSin + kDataInSinLength);

/// @brief Uniform distribution of normalized float values
static std::uniform_real_distribution<float> kNormDistribution(-1.0f, 1.0f);
static std::default_random_engine kRandomGenerator;

#endif  // CHARTREUSE_TESTS_TESTS_H_
