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
  unsigned int current_index(0);
  float* current_out(output);
  // TODO(gm): beware of the case wih a tiny input length
  // AND a tiny buffer remaining count!
  // First process the remaining data from the last call
  const unsigned int kPoppedCount(buffer_.Size());
  if (kPoppedCount > 0) {
    float tmp_input[chartreuse::kHopSizeSamples];
    // If more data than one hop size was remaining at the last call,
    // it would have been used as another subframe...
    CHARTREUSE_ASSERT(buffer_.Size() < chartreuse::kHopSizeSamples);
    // ...Which means that we can empty the buffer here
    buffer_.Pop(&tmp_input[0], kPoppedCount);
    // Completing with the current input data
    std::copy_n(input,
                chartreuse::kHopSizeSamples - kPoppedCount,
                &tmp_input[kPoppedCount]);
    current_index += kPoppedCount;
    desc_manager_.ProcessFrame(&tmp_input[0],
                                chartreuse::kHopSizeSamples);
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
  }
  while (length - current_index >= chartreuse::kHopSizeSamples) {
    desc_manager_.ProcessFrame(&input[current_index],
                               chartreuse::kHopSizeSamples);
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
    current_index += chartreuse::kHopSizeSamples;
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
