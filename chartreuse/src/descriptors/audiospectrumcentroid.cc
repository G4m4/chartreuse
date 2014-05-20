/// @file audiospectrumcentroid.cc
/// @brief AudioSpectrumCentroid descriptor declaration
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

#include "chartreuse/src/descriptors/audiospectrumcentroid.h"

// std::ceil, std::log
#include <cmath>
// std::accumulate
#include <numeric>

#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace descriptors {

/// @brief Frequency below which all frequencies contribution are summed,
/// arbitrarily fixed
static const float kLowEdge(62.5f);

AudioSpectrumCentroid::AudioSpectrumCentroid(manager::Manager* manager)
    : Descriptor_Interface(manager),
      kLowEdgeIndex_(static_cast<unsigned int>(
                     std::ceil(kLowEdge * manager->DftLength()
                               / manager->SamplingFrequency()))),
      // TODO(gm): use a static kind of "GetDescriptorSize(kSpectrogramPower)"
      kHighEdgeIndex_(manager->DftLength() / 2 + 1),
      freq_scale_(kHighEdgeIndex_ - kLowEdgeIndex_,
                  algorithms::Scale::kLogFreq,
                  manager->DftLength(),
                  kLowEdgeIndex_,
                  manager->SamplingFrequency()),
      // TODO(gm): use a static kind of "GetDescriptorSize(kSpectrogramPower)"
      power_(manager->DftLength() / 2 + 1) {
  CHARTREUSE_ASSERT(kLowEdgeIndex_ > 0);
  CHARTREUSE_ASSERT(kHighEdgeIndex_ > 0);
  CHARTREUSE_ASSERT(kLowEdgeIndex_ < kHighEdgeIndex_);
}

void AudioSpectrumCentroid::operator()(const float* const frame,
                                       const std::size_t frame_length,
                                       float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);
  // Get the normalized squared magnitude spectrogram of the frame
  manager_->GetDescriptorCopy(manager::DescriptorId::kSpectrogramPower,
                              &frame[0],
                              frame_length,
                              &power_[0]);
  // The DC component is unchanged, everything else is doubled
  power_[0] *= 0.5f;
  // Summing the contributions of all frequencies lower than 62.5
  for (unsigned int i(0); i < kLowEdgeIndex_ - 1; ++i) {
    power_[kLowEdgeIndex_ - 1] += power_[i];
  }
  const float kPowerSum(std::accumulate(&power_[kLowEdgeIndex_ - 1],
                                        &power_[kHighEdgeIndex_ - 1],
                                        0.0f)
                        + power_[kHighEdgeIndex_ - 1]
                        // Prevent divide by zero
                        + 1e-7f);
  CHARTREUSE_ASSERT(kPowerSum > 0.0f);
  // Weight each DFT bin by the log of the frequency relative to 1000Hz
  // The first bin is the low edge
  float kOut(power_[kLowEdgeIndex_ - 1] * -5.0f);
  for (unsigned int i(kLowEdgeIndex_); i < kHighEdgeIndex_; ++i) {
    kOut += power_[i] * freq_scale_.GetScaleAtIndex(i - kLowEdgeIndex_);
  }
  data[0] = kOut / kPowerSum;
}

Descriptor_Meta AudioSpectrumCentroid::Meta(void) const {
  return Descriptor_Meta(1, -5.0f, 5.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
