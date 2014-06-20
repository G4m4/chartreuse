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

namespace chartreuse {
namespace interface {

Analyzer::Analyzer(const float sampling_freq)
    : desc_manager_(Manager::Parameters(sampling_freq), false),
      buffer_(chartreuse::kHopSizeSamples) {
  // Nothing to do here for now
}

Analyzer::~Analyzer() {
  // Nothing to do here for now
}

void Analyzer::Process(const float* const input,
                       const unsigned int length,
                       float* const output) {
  unsigned int remaining_length(length);
  float* current_out(output);
  while (remaining_length >= chartreuse::kHopSizeSamples) {
    desc_manager_.ProcessFrame(input, chartreuse::kHopSizeSamples);
    for (unsigned int desc_idx(0);
         desc_idx < kAvailableDescriptors.size();
         ++desc_idx) {
      const DescriptorId::Type current_descriptor(
        kAvailableDescriptors[desc_idx]);
      const float kRawValue(*desc_manager_.GetDescriptor(current_descriptor));
      const descriptors::Descriptor_Meta& kMeta(desc_manager_.GetDescriptorMeta(
                                                  current_descriptor));
      // Normalization
      *output = (kRawValue - kMeta.out_min) / (kMeta.out_max - kMeta.out_min);
      current_out += 1;
    }
    remaining_length -= chartreuse::kHopSizeSamples;
  }
}

}  // namespace interface
}  // namespace chartreuse
