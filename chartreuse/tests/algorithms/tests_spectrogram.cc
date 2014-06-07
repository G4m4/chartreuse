/// @file tests_spectrogram.cc
/// @brief Chartreuse spectrogram algorithm tests
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

// Useful using declarations
using chartreuse::manager::Manager;
using chartreuse::manager::DescriptorId::kSpectrogram;

/// @brief Compute the spectrogram for white noise
TEST(Spectrogram, WhiteNoise) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  chartreuse::manager::DescriptorId::Type descriptor(kSpectrogram);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  manager.EnableDescriptor(descriptor, true);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager(&frame[0], frame.size(), &frame[0]);
    const float* out_data(manager.GetDescriptor(descriptor, &frame[0], frame.size()));
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
TEST(Spectrogram, Sin) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  chartreuse::manager::DescriptorId::Type descriptor(kSpectrogram);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  manager.EnableDescriptor(descriptor, true);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    const std::size_t kRightIndex(
      std::min(index + frame.size(),
      static_cast<std::size_t>(kDataInSinLength - 1)));
    std::copy(&kInSin[index],
              &kInSin[kRightIndex],
              frame.begin());
    manager(&frame[0], frame.size(), &frame[0]);
    const float* out_data(manager.GetDescriptor(descriptor, &frame[0], frame.size()));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}
