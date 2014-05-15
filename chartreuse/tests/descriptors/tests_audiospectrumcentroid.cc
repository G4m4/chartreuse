/// @file tests_audiospectrumcentroid.cc
/// @brief Chartreuse AudioSpectrumCentroid descriptor tests
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

#include "chartreuse/src/descriptors/audiospectrumcentroid.h"

// Using declarations for tested class
using chartreuse::descriptors::AudioSpectrumCentroid;

// TODO(gm): move this in a common file
static const float kSamplingFreq(48000.0f);

/// @brief Compute the descriptor for an uniform white noise,
/// check that its range lies within [-1.0f ; 1.0f]
TEST(AudioSpectrumCentroid, Range) {
  AudioSpectrumCentroid descriptor(kSamplingFreq);
  std::vector<float> desc_data(AudioSpectrumCentroid::Meta().out_dim);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    descriptor(&frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GT(AudioSpectrumCentroid::Meta().out_max, desc_data[desc_index]);
      EXPECT_LT(AudioSpectrumCentroid::Meta().out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid of very low frequency,
/// check the descriptor output lower bound
TEST(AudioSpectrumCentroid, LowerBound) {
  const float kFrequency(1.0f);
  AudioSpectrumCentroid descriptor(kSamplingFreq);
  std::vector<float> desc_data(AudioSpectrumCentroid::Meta().out_dim);

  std::size_t index(0);
  SinusGenerator generator(kFrequency, kSamplingFreq);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  generator);
    descriptor(&frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GT(AudioSpectrumCentroid::Meta().out_max, desc_data[desc_index]);
      EXPECT_LT(AudioSpectrumCentroid::Meta().out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid of very high frequency,
/// check the descriptor output higher bound
TEST(AudioSpectrumCentroid, HigherBound) {
  const float kFrequency((kSamplingFreq - 10.f) / 2.0f);
  AudioSpectrumCentroid descriptor(kSamplingFreq);
  std::vector<float> desc_data(AudioSpectrumCentroid::Meta().out_dim);

  std::size_t index(0);
  SinusGenerator generator(kFrequency, kSamplingFreq);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  generator);
    descriptor(&frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GT(AudioSpectrumCentroid::Meta().out_max, desc_data[desc_index]);
      EXPECT_LT(AudioSpectrumCentroid::Meta().out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid,
/// check that its range lies within [-1.0f ; 1.0f]
TEST(AudioSpectrumCentroid, Sin) {
  AudioSpectrumCentroid descriptor(kSamplingFreq);
  std::vector<float> desc_data(AudioSpectrumCentroid::Meta().out_dim);

  std::size_t index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    const std::size_t kRightIndex(
      std::min(index + frame.size(),
      static_cast<std::size_t>(kDataInSinLength - 1)));
    std::copy(&kInSin[index],
              &kInSin[kRightIndex],
              frame.begin());
    descriptor(&frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GT(AudioSpectrumCentroid::Meta().out_max, desc_data[desc_index]);
      EXPECT_LT(AudioSpectrumCentroid::Meta().out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a constant value,
/// check that its range lies within [-1.0f ; 1.0f]
TEST(AudioSpectrumCentroid, Constant) {
  AudioSpectrumCentroid descriptor(kSamplingFreq);
  std::vector<float> desc_data(AudioSpectrumCentroid::Meta().out_dim);
  const float kConstant(1.0f);

  std::size_t index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::fill(frame.begin(),
              frame.end(),
              kConstant);
    descriptor(&frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GT(AudioSpectrumCentroid::Meta().out_max, desc_data[desc_index]);
      EXPECT_LT(AudioSpectrumCentroid::Meta().out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Performance test for computing a fixed length signal
TEST(AudioSpectrumCentroid, Perf) {
  AudioSpectrumCentroid descriptor(kSamplingFreq);
  std::vector<float> desc_data(AudioSpectrumCentroid::Meta().out_dim);

  std::size_t index(0);
  // Computing the mean output prevents the compiler from optimizing out things
  float mean(0.0f);
  while (index < kDataPerfSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    descriptor(&frame[0], frame.size(), &desc_data[0]);
    mean += desc_data[0] * desc_data[0];
    index += frame.size();
  }
  EXPECT_LT(-1.0f, mean);
}
