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
#include <array>
#include <cmath>
#include <functional>
#include <iterator>
#include <random>
#include <vector>

#include "gtest/gtest.h"

#include "chartreuse/src/configuration.h"
#include "chartreuse/src/common.h"

// Smaller performance test sets in debug
#if (_BUILD_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize = 64 * 1024;
#else  // (_BUILD_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize = 64 * 1024 * 1024;
#endif  // (_BUILD_CONFIGURATION_DEBUG)

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
#if (_BUILD_CONFIGURATION_DEBUG)
/// @brief Sinusoid data length
static const size_t kDataInSinLength(sizeof(kDataInSin)
                                       / (4 * sizeof(kDataInSin[0])));
#else  // (_BUILD_CONFIGURATION_DEBUG)
/// @brief Sinusoid data length
static const size_t kDataInSinLength(sizeof(kDataInSin)
                                     / sizeof(kDataInSin[0]));
#endif  // (_BUILD_CONFIGURATION_DEBUG)

/// @brief Helper wrapper around the sinusoid data
const std::vector<float> kInSin(kDataInSin, kDataInSin + kDataInSinLength);

/// @brief Standard length for tests data sets
static const unsigned int kDataTestSetSize(kDataInSinLength);

/// @brief Uniform distribution of normalized float values
static std::uniform_real_distribution<float> kNormDistribution(-1.0f, 1.0f);
/// @brief Random generator
static std::default_random_engine kRandomGenerator;

/// @brief Basic sinus generator
///
/// No special care has been taken performance or quality-wise
class SinusGenerator {
 public:
  /// @brief Default constructor, the generated and sampling frequencies
  /// have to be provided
  ///
  /// @param[in]  freq    Generated sinus frequency
  /// @param[in]  sampling_freq    Generated signal sampling frequency
  SinusGenerator(const float freq, const float sampling_freq);
  ~SinusGenerator();

  /// @brief Actual processing method
  ///
  /// @return the next sample in order to create a sinus
  float operator()(void);

 private:
  // No assignment operator for this functor
  SinusGenerator& operator=(const SinusGenerator& right);

  const double increment_;
  double current_phase_;
};

#endif  // CHARTREUSE_TESTS_TESTS_H_
