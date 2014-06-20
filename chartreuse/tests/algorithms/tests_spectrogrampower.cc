/// @file tests_spectrogrampower.cc
/// @brief Chartreuse spectrogram power computation algorithm tests
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

#include "chartreuse/src/interface/manager.h"

// Useful using declarations
using chartreuse::interface::Manager;
using chartreuse::interface::DescriptorId::kSpectrogramPower;

/// @brief Compute the spectrogram power for white noise
TEST(SpectrogramPower, WhiteNoise) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  chartreuse::interface::DescriptorId::Type descriptor(kSpectrogramPower);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  manager.EnableDescriptor(descriptor, true);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the spectrogram for a sinus
TEST(SpectrogramPower, Sin) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  chartreuse::interface::DescriptorId::Type descriptor(kSpectrogramPower);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  manager.EnableDescriptor(descriptor, true);

  std::size_t index(0);
  const unsigned int kFrameLength(manager.AnalysisParameters().hop_size_sample);
  while (index < kDataTestSetSize) {
    manager.ProcessFrame(&kInSin[index], kFrameLength);
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += kFrameLength;
  }
}

/// @brief Algorithm performance evaluation
TEST(SpectrogramPower, Perf) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  chartreuse::interface::DescriptorId::Type descriptor(kSpectrogramPower);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  manager.EnableDescriptor(descriptor, true);

  std::size_t index(0);
  while (index < kDataPerfSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}
