/// @file dummyclass.cc
/// @brief Dummy class definition
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

#include "chartreuse/src/interface/analyzer.h"

// std::copy_n
#include <algorithm>
#include <array>

#include "chartreuse/src/common.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"
#include "chartreuse/src/interface/manager.h"

namespace chartreuse {
namespace interface {

Analyzer::Analyzer(const float sampling_freq)
    : desc_manager_(*new Manager(Manager::Parameters(sampling_freq), true)),
      buffer_(chartreuse::kHopSizeSamples) {
  // Nothing to do here for now
}

Analyzer::~Analyzer() {
  delete &desc_manager_;
}

unsigned int Analyzer::Process(const float* const input,
                               const unsigned int length,
                               float* const output) {
  CHARTREUSE_ASSERT(input != nullptr);
  CHARTREUSE_ASSERT(length > 0);
  CHARTREUSE_ASSERT(output != nullptr);
  CHARTREUSE_ASSERT(input != output);

  unsigned int current_index(0);
  float* current_out(output);
  // First process the remaining data from the last call
  // If more data than one hop size was remaining at the last call,
  // it would have been used as another subframe...
  CHARTREUSE_ASSERT(buffer_.Size() < chartreuse::kHopSizeSamples);
  const unsigned int kCompletingCount(
    std::min(chartreuse::kHopSizeSamples - buffer_.Size(), length));
  const unsigned int kPoppedCount(buffer_.Size());
  // Fill the buffer so it makes a whole subframe
  buffer_.Push(&input[0], kCompletingCount);
  // Handling tiny frame length
  if (buffer_.Size() < chartreuse::kHopSizeSamples) {
    return 0;
  }
  float tmp_input[chartreuse::kHopSizeSamples];
  buffer_.Pop(&tmp_input[0], chartreuse::kHopSizeSamples);
  desc_manager_.ProcessFrame(&tmp_input[0],
                              chartreuse::kHopSizeSamples);
  current_index += kCompletingCount;
  while (length - current_index >= chartreuse::kHopSizeSamples) {
    for (unsigned int desc_idx(0);
         desc_idx < kAvailableDescriptors.size();
         ++desc_idx) {
      const DescriptorId::Type current_descriptor(
        kAvailableDescriptors[desc_idx]);
      const float kRawValue(*desc_manager_.GetDescriptor(current_descriptor));
      const descriptors::Descriptor_Meta& kMeta(desc_manager_.GetDescriptorMeta(
                                                  current_descriptor));
      // Normalization
      *current_out = Normalize(kRawValue, kMeta.out_min, kMeta.out_max);
      current_out += 1;
    }
    desc_manager_.ProcessFrame(&input[current_index],
                               chartreuse::kHopSizeSamples);
    current_index += chartreuse::kHopSizeSamples;
  }
  for (unsigned int desc_idx(0);
        desc_idx < kAvailableDescriptors.size();
        ++desc_idx) {
    const DescriptorId::Type current_descriptor(
      kAvailableDescriptors[desc_idx]);
    const float kRawValue(*desc_manager_.GetDescriptor(current_descriptor));
    const descriptors::Descriptor_Meta& kMeta(desc_manager_.GetDescriptorMeta(
                                                current_descriptor));
    // Normalization
    *current_out = Normalize(kRawValue, kMeta.out_min, kMeta.out_max);
    current_out += 1;
  }
  CHARTREUSE_ASSERT(buffer_.Size() == 0);
  buffer_.Push(&input[current_index], length - current_index);

  return (current_index + kPoppedCount) / chartreuse::kHopSizeSamples;
}

float Analyzer::Normalize(const float input,
                          const float in_min,
                          const float in_max) const {
  return (input - in_min) / (in_max - in_min);
}

}  // namespace interface
}  // namespace chartreuse
