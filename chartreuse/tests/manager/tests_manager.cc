/// @file tests_manager.cc
/// @brief Chartreuse manager class tests
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

// Using declarations for tested class
using chartreuse::manager::Manager;
using chartreuse::manager::DescriptorId::kAudioPower;
using chartreuse::manager::DescriptorId::kAudioSpectrumCentroid;
using chartreuse::manager::DescriptorId::kAudioSpectrumSpread;
using chartreuse::manager::DescriptorId::kAudioWaveform;

/// @brief Compute all descriptors for white noise
TEST(Manager, WhiteNoise) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  std::vector<float> out_data(kDftLength * 2);

  Manager manager(kSamplingFreq);
  manager.EnableDescriptor(kAudioPower, true);
  manager.EnableDescriptor(kAudioSpectrumCentroid, true);
  manager.EnableDescriptor(kAudioSpectrumSpread, true);
  manager.EnableDescriptor(kAudioWaveform, true);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager(&frame[0],
            frame.size(),
            &out_data[0]);
    index += frame.size();
  }
}
