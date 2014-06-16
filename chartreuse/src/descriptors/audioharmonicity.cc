/// @file audioharmonicity.cc
/// @brief AudioHarmonicity descriptor implementation
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

#include "chartreuse/src/descriptors/audioharmonicity.h"

// std::floor
#include <cmath>

#include "Eigen/Core"

#include "chartreuse/src/manager/manager.h"
#include "chartreuse/src/algorithms/algorithms_common.h"

namespace chartreuse {
namespace descriptors {

AudioHarmonicity::AudioHarmonicity(manager::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here for now
}

void AudioHarmonicity::operator()(float* const output) {
  // Retrieve current frame autocorrelation & AFF
  const float kAFF(*manager_->GetDescriptor(
    manager::DescriptorId::kAudioFundamentalFrequency));
  Process(manager_->GetDescriptor(manager::DescriptorId::kAutoCorrelation),
          kAFF,
          manager_->AnalysisParameters().min_lag,
          output);
}

void AudioHarmonicity::Process(const float* const autocorrelation,
                               const float estimated_lag,
                               const unsigned int min_lag,
                               float* const output) {
  CHARTREUSE_ASSERT(autocorrelation != nullptr);
  CHARTREUSE_ASSERT(estimated_lag > 0);
  CHARTREUSE_ASSERT(min_lag > 0);
  CHARTREUSE_ASSERT(estimated_lag >= static_cast<float>(min_lag));
  CHARTREUSE_ASSERT(output != nullptr);
  CHARTREUSE_ASSERT(output != autocorrelation);

  const float kActualLag(estimated_lag - static_cast<float>(min_lag));
  const unsigned int kActualIndex(static_cast<unsigned int>(
                                  std::floor(kActualLag)));
  const float kLeft(autocorrelation[kActualIndex]);
  const float kRight(autocorrelation[kActualIndex + 1]);
  const float out(algorithms::LinearInterpolation(
                    kLeft,
                    kRight,
                    kActualLag - static_cast<float>(kActualIndex)));

  output[0] = out;
}

Descriptor_Meta AudioHarmonicity::Meta(void) const {
  return Descriptor_Meta(
    1,
    -1.0f,
    1.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
