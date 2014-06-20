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

#include "chartreuse/src/interface/manager.h"

// Using declarations for tested class
using chartreuse::interface::Manager;
// Using declarations for related classes
using chartreuse::interface::DescriptorId::kCount;
using chartreuse::interface::DescriptorId::Type;
using chartreuse::descriptors::Descriptor_Meta;

/// @brief Compute all descriptors for white noise
TEST(Manager, WhiteNoise) {
  const float kSamplingFreq(48000.0f);

  Manager manager((Manager::Parameters::Parameters(kSamplingFreq)));

  for (unsigned int descriptor_idx(0);
       descriptor_idx < kCount;
       ++descriptor_idx) {
    manager.EnableDescriptor(static_cast<Type>(descriptor_idx), true);
  }

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.ProcessFrame(&frame[0],
                         frame.size());
    for (unsigned int descriptor_idx(0);
         descriptor_idx < kCount;
         ++descriptor_idx) {
      const Type descriptor(static_cast<Type>(descriptor_idx));
      const Descriptor_Meta& desc_meta(manager.GetDescriptorMeta(descriptor));
      const float* out_data(manager.GetDescriptor(descriptor));
      for (unsigned int desc_index(0); desc_index < desc_meta.out_dim; ++desc_index) {
        EXPECT_GE(desc_meta.out_max, out_data[desc_index]);
        EXPECT_LE(desc_meta.out_min, out_data[desc_index]);
      }
    }
    index += frame.size();
  }
}

/// @brief Compute all descriptors for white noise
TEST(Manager, Perf) {
  const float kSamplingFreq(48000.0f);

  Manager manager((Manager::Parameters::Parameters(kSamplingFreq)));

  for (unsigned int descriptor_idx(0);
       descriptor_idx < kCount;
       ++descriptor_idx) {
    manager.EnableDescriptor(static_cast<Type>(descriptor_idx), true);
  }

  std::size_t index(0);
  while (index < kDataPerfSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.ProcessFrame(&frame[0],
                         frame.size());
    index += frame.size();
  }
}
