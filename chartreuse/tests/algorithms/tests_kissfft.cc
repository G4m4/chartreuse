/// @file tests_kissfft.cc
/// @brief Chartreuse kissfft wrapper tests
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

#include "chartreuse/src/manager/manager.h"
#include "chartreuse/src/algorithms/kissfft.h"

// Useful using declarations
using chartreuse::algorithms::KissFFT;
using chartreuse::manager::Manager;
using chartreuse::manager::DescriptorId::kDft;

static const unsigned int kFFTDataSize(1024);
static const unsigned int kDefaultSamplingRate = 48000;
static const float kDefaultTestFreq = 3520.0f;  // 8 * 440

/// @brief Compute the DFT of a constant odd length short vector
/// Useful basic sanity check for development purpose
TEST(KissFFT, BasicOddSize) {
  const float kValue(1.0f);
  std::vector<float> data(5, kValue);
  const unsigned int kDftLength(8);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength), false);
  KissFFT descriptor(&manager);

  descriptor.Process(&data[0],
                     data.size(),
                     kDftLength,
                     &out_data[0]);

  const float kExpected(kValue * data.size());
  EXPECT_EQ(kExpected, out_data[0]);
}

/// @brief Compute the DFT of an alternating even length short vector
/// Useful basic sanity check for development purpose
TEST(KissFFT, BasicEvenSize) {
  const float kValue(1.0f);
  std::vector<float> data(4, kValue);
  // Invert every other value
  data[1] *= -1.0f;
  data[3] *= -1.0f;
  const unsigned int kDftLength(8);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength), false);
  KissFFT descriptor(&manager);

  descriptor.Process(&data[0],
                     data.size(),
                     kDftLength,
                     &out_data[0]);

  const float kExpected(kValue * data.size());
  EXPECT_EQ(kExpected, out_data[kDftLength]);
}

/// @brief Compute the DFT of an uniform white noise
TEST(KissFFT, WhiteNoise) {
  std::vector<float> data(kFFTDataSize);
  std::generate(data.begin(),
                data.end(),
                [&] {return kNormDistribution(kRandomGenerator);});

  const unsigned int kDftLength(kLargeDFTLength);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength), false);
  KissFFT descriptor(&manager);

  const float kExpectedMean = 0.0f;
  const float kEpsilonMean = 1e-3f * kDftLength;

  descriptor.Process(&data[0],
                     data.size(),
                     kDftLength,
                     &out_data[0]);

  float mean(0.0f);
  for (unsigned int idx(0);
       idx < data.size();
       idx += 1) {
    mean += out_data[idx];
  }
  mean /= kDftLength;

  EXPECT_NEAR(kExpectedMean, mean, kEpsilonMean);
}

/// @brief Check normalization factor by retrieving the DFT
/// for an unitary vector
TEST(KissFFT, Normalization) {
  const unsigned int kDftLength(kMediumDFTLength);
  std::vector<float> data(kDataInSinLength, 1.0f);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq,
                                      kDftLength,
                                      62.5f,
                                      1500.0f,
                                      data.size(),
                                      1), false);
  KissFFT descriptor(&manager);

  // Input buffer normalized
  // Note that whatever the input data size is,
  // the output data is limited by the size of the DFT
  const float kExpected = static_cast<float>(kDftLength);
  const float kEpsilon = 1e-5f;

  descriptor.Process(&data[0],
                     data.size(),
                     kDftLength,
                     &out_data[0]);

  const float kActual = out_data[0];

  EXPECT_NEAR(kExpected, kActual, kEpsilon);
}

/// @brief Check properties of a medium-length DFT for a pure sinusoid
TEST(KissFFT, MagSinMedLengthProperties) {
  const unsigned int kDftLength(kMediumDFTLength);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq,
                                      kDftLength,
                                      62.5f,
                                      1500.0f,
                                      kInSin.size(),
                                      1), false);
  KissFFT descriptor(&manager);

  const float kExpected = (kDftLength * kDefaultTestFreq)
                          / static_cast<float>(kDefaultSamplingRate);
  const float kEpsilon = 1.0f;  // Due to resolution issues

  descriptor.Process(&kInSin[0],
                     kInSin.size(),
                     kDftLength,
                     &out_data[0]);

  const unsigned int kActual(
    std::distance(&out_data[0],
                  std::min_element(&out_data[0],
                                   // Only searching on half the data,
                                   // since this output is complex!
                                   &out_data[0] + kDftLength))
    / 2);

  EXPECT_NEAR(kExpected, kActual, kEpsilon);
}

/// @brief Compare the result of a small-length DFT of a pure sinusoid
/// with externally precomputed reference data
TEST(KissFFT, SinSmallLength) {
  const unsigned int kDftLength(kSmallDFTLength);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq,
                                      kDftLength,
                                      62.5f,
                                      1500.0f,
                                      kInSin.size(),
                                      1), false);
  KissFFT descriptor(&manager);

  const float kEpsilon = 1e-3f * kDftLength;

  descriptor.Process(&kInSin[0],
                     kInSin.size(),
                     kDftLength,
                     &out_data[0]);

  for (unsigned int i = 0; i < kDftLength; i += 2) {
    EXPECT_NEAR(kDataSmallSinDFT[i / 2], out_data[i], kEpsilon);
  }
}

/// @brief Compute the magnitude of a medium-length DFT of a pure sinusoid
/// with externally precomputed reference data
TEST(KissFFT, SinMedLength) {
  const unsigned int kDftLength(kMediumDFTLength);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq,
                                      kDftLength,
                                      62.5f,
                                      1500.0f,
                                      kInSin.size(),
                                      1), false);
  KissFFT descriptor(&manager);

  const float kEpsilon = 1e-3f * kDftLength;

  descriptor.Process(&kInSin[0],
                     kInSin.size(),
                     kDftLength,
                     &out_data[0]);

  for (unsigned int i = 0; i < kDftLength; i += 2) {
    EXPECT_NEAR(kDataMediumSinDFT[i / 2], out_data[i], kEpsilon);
  }
}

/// @brief Compute the magnitude of a large-length DFT of a pure sinusoid
/// with externally precomputed reference data
TEST(KissFFT, SinLargeLength) {
  const unsigned int kDftLength(kLargeDFTLength);
  std::vector<float> out_data(kDftLength + 2);

  Manager manager(Manager::Parameters(kSamplingFreq,
                                      kDftLength,
                                      62.5f,
                                      1500.0f,
                                      kInSin.size(),
                                      1), false);
  KissFFT descriptor(&manager);

  // Greater error for this DFT length
  // TODO: find out why?
  const float kEpsilon = 1e-2f * kDftLength;

  descriptor.Process(&kInSin[0],
                     kInSin.size(),
                     kDftLength,
                     &out_data[0]);

  for (unsigned int i = 0; i < kDftLength; i += 2) {
    EXPECT_NEAR(kDataLargeSinDFT[i / 2], out_data[i], kEpsilon);
  }
}
