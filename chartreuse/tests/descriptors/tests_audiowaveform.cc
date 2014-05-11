/// @file tests_audiowaveform.cc
/// @brief Chartreuse AudioWaveform descriptor tests
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

#include "chartreuse/src/descriptors/audiowaveform.h"

// Using declarations for tested class
using chartreuse::descriptors::AudioWaveform;

/// @brief Compute the descriptor for an uniform white noise,
/// check that its range lies within [-1.0f ; 1.0f]
TEST(AudioWaveform, Range) {
  AudioWaveform descriptor;
  std::vector<float> desc_data(descriptor.DataLength());

  unsigned int index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  std::bind(kNormDistribution, kRandomGenerator));
    descriptor(frame, &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GT(1.0f, desc_data[desc_index]);
      EXPECT_LT(-1.0f, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid,
/// check that its range lies within [-1.0f ; 1.0f]
TEST(AudioWaveform, Sin) {
  AudioWaveform descriptor;
  std::vector<float> desc_data(descriptor.DataLength());

  unsigned int index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::copy(&kInSin[index],
              &kInSin[std::min(index + frame.size(), kDataInSinLength - 1)],
              frame.begin());
    descriptor(frame, &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(1.0f, desc_data[desc_index]);
      EXPECT_LE(-1.0f, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a constant value,
/// check that its range lies within [-1.0f ; 1.0f]
TEST(AudioWaveform, Constant) {
  AudioWaveform descriptor;
  std::vector<float> desc_data(descriptor.DataLength());
  const float kConstant(1.0f);

  unsigned int index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::fill(frame.begin(),
              frame.end(),
              kConstant);
    descriptor(frame, &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(1.0f, desc_data[desc_index]);
      EXPECT_LE(-1.0f, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Performance test for computing a fixed length signal
TEST(AudioWaveform, Perf) {
  AudioWaveform descriptor;
  std::vector<float> desc_data(descriptor.DataLength());

  unsigned int index(0);
  // Computing the mean output prevents the compiler from optimizing out things
  float mean(0.0f);
  while (index < kDataPerfSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  std::bind(kNormDistribution, kRandomGenerator));
    descriptor(frame, &desc_data[0]);
    mean += desc_data[0] * desc_data[0];
    index += frame.size();
  }
  EXPECT_LT(-1.0f, mean);
}
