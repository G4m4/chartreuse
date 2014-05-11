/// @file tests_dftraw.cc
/// @brief Chartreuse "DftRaw" algorithm tests
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

#include "chartreuse/tests/tests.h"

#include "chartreuse/src/algorithms/dftraw.h"

// Using declarations for tested class
using chartreuse::algorithms::DftRaw;

static const unsigned int kFFTDataSize(1024);
static const unsigned int kDefaultSamplingRate = 48000;
static const float kDefaultTestFreq = 3520.0f;  // 8 * 440

/// @brief Compute the DFT of an uniform white noise
TEST(DftRaw, WhiteNoise) {
  std::vector<float> data(kFFTDataSize);
  std::vector<float> out_data(kLargeDFTLength, 0.0f);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  const float kExpectedMean = 0.0f;
  const float kEpsilonMean = 1e-3f * out_data.size();

  DftRaw dft;
  dft(&data[0],
      &data[data.size() - 1],
      false,
      out_data.size(),
      &out_data[0]);

  float mean(0.0f);
  for (std::vector<float>::const_iterator iter(out_data.begin());
       iter != out_data.end();
       iter += 2) {
    mean += *iter;
  }
  mean /= out_data.size() / 2.0f;

  EXPECT_NEAR(kExpectedMean, mean, kEpsilonMean);
}

/// @brief Check normalization factor by retrieving the DFT
/// for an unitary vector
TEST(DftRaw, Normalization) {
  std::vector<float> data(kDataInSinLength, 1.0f);
  std::vector<float> out_data(kMediumDFTLength, 0.0f);

  // Input buffer normalized
  // Note that whatever the input data size is,
  // the output data is limited by the size of the DFT
  const float kExpected = static_cast<float>(out_data.size());
  const float kEpsilon = 1e-5f;

  DftRaw dft;
  dft(&data[0],
      &data[data.size() - 1],
      false,
      out_data.size(),
      &out_data[0]);

  const float kActual = out_data[0];

  EXPECT_NEAR(kExpected, kActual, kEpsilon);
}

/// @brief Check properties of a medium-length DFT for a pure sinusoid
TEST(DftRaw, MagSinMedLengthProperties) {
  std::vector<float> out_data(kMediumDFTLength, 0.0f);

  const float kExpected = (kMediumDFTLength * kDefaultTestFreq)
                          / static_cast<float>(kDefaultSamplingRate);
  const float kEpsilon = 1.0f;  // Due to resolution issues

  DftRaw dft;
  dft(&kInSin[0],
      &kInSin[kInSin.size() - 1],
      false,
      out_data.size(),
      &out_data[0]);

  const unsigned int kActual(
    std::distance(out_data.begin(),
                  std::min_element(out_data.begin(), out_data.end()))
    / 2);

  EXPECT_NEAR(kExpected, kActual, kEpsilon);
}

/// @brief Compare the result of a small-length DFT of a pure sinusoid
/// with externally precomputed reference data
TEST(DftRaw, SinSmallLength) {
  std::vector<float> out_data(kSmallDFTLength, 0.0f);
  const float kEpsilon = 1e-3f * kSmallDFTLength;

  DftRaw dft;
  dft(&kInSin[0],
      &kInSin[kInSin.size() - 1],
      false,
      out_data.size(),
      &out_data[0]);

  for (unsigned int i = 0; i < kSmallDFTLength; i += 2) {
    EXPECT_NEAR(kDataSmallSinDFT[i / 2], out_data[i], kEpsilon);
  }
}

/// @brief Compute the magnitude of a medium-length DFT of a pure sinusoid
/// with externally precomputed reference data
TEST(DftRaw, SinMedLength) {
  std::vector<float> out_data(kMediumDFTLength, 0.0f);
  const float kEpsilon = 1e-3f * kMediumDFTLength;

  DftRaw dft;
  dft(&kInSin[0],
      &kInSin[kInSin.size() - 1],
      false,
      out_data.size(),
      &out_data[0]);

  for (unsigned int i = 0; i < kMediumDFTLength; i += 2) {
    EXPECT_NEAR(kDataMediumSinDFT[i / 2], out_data[i], kEpsilon);
  }
}

/// @brief Compute the magnitude of a large-length DFT of a pure sinusoid
/// with externally precomputed reference data
TEST(DftRaw, SinLargeLength) {
  std::vector<float> out_data(kLargeDFTLength, 0.0f);
  // Greater error for this DFT length
  // TODO: find out why?
  const float kEpsilon = 1e-2f * kLargeDFTLength;

  DftRaw dft;
  dft(&kInSin[0],
      &kInSin[kInSin.size() - 1],
      false,
      out_data.size(),
      &out_data[0]);

  for (unsigned int i = 0; i < kLargeDFTLength; i += 2) {
    EXPECT_NEAR(kDataLargeSinDFT[i / 2], out_data[i], kEpsilon);
  }
}