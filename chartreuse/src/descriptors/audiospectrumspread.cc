/// @file audiospectrumspread.cc
/// @brief AudioSpectrumSpread descriptor declaration
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

#include "chartreuse/src/descriptors/audiospectrumspread.h"

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

AudioSpectrumSpread::AudioSpectrumSpread(manager::Manager* manager,
                                         const float sampling_freq)
    : Descriptor_Interface(manager),
      sampling_freq_(sampling_freq),
      kLowEdgeIndex_(static_cast<unsigned int>(
                    std::ceil(kLowEdge * manager::Manager::kSpectrumDftLength / sampling_freq_))),
      kHighEdgeIndex_(manager::Manager::kSpectrumDftLength / 2 + 1),
      spectrogram_(manager, manager::Manager::kSpectrumDftLength, sampling_freq),
      freq_scale_(kHighEdgeIndex_ - kLowEdgeIndex_,
                  algorithms::Scale::kLogFreq,
                  manager::Manager::kSpectrumDftLength,
                  kLowEdgeIndex_,
                  sampling_freq),
      buffer_(manager::Manager::kSpectrumDftLength + 2),
      power_(manager::Manager::kSpectrumDftLength / 2 + 1) {
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  CHARTREUSE_ASSERT(kLowEdgeIndex_ > 0);
  CHARTREUSE_ASSERT(kHighEdgeIndex_ > 0);
  CHARTREUSE_ASSERT(kLowEdgeIndex_ < kHighEdgeIndex_);
}

void AudioSpectrumSpread::operator()(const float* const frame,
                                       const std::size_t frame_length,
                                       float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);
  // Get the DFT of the frame
  spectrogram_(&frame[0], frame_length, &buffer_[0]);
  // Normalization of the DFT
  const float kDFTNorm(2.0f
    / static_cast<float>(manager::Manager::kSpectrumDftLength * manager::Manager::kSpectrumWindowLength));
  // Retrieving the squared magnitude of the DFT
  for (std::size_t i(0); i < buffer_.size(); i += 2) {
    power_[i / 2] = buffer_[i] * buffer_[i] + buffer_[i + 1] * buffer_[i + 1];
    power_[i / 2] *= kDFTNorm;
  }
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
  float kTmp(power_[kLowEdgeIndex_ - 1] * -5.0f);
  for (unsigned int i(kLowEdgeIndex_); i < kHighEdgeIndex_; ++i) {
    kTmp += power_[i] * freq_scale_.GetScaleAtIndex(i - kLowEdgeIndex_);
  }
  const float kCentroid(kTmp / kPowerSum);
  float out(0.0f);
  for (unsigned int i(kLowEdgeIndex_); i < kHighEdgeIndex_; ++i) {
    out += power_[i]
           * (freq_scale_.GetScaleAtIndex(i - kLowEdgeIndex_) - kCentroid)
           * (freq_scale_.GetScaleAtIndex(i - kLowEdgeIndex_) - kCentroid);
  }
  data[0] = std::sqrt(out / kPowerSum);
}

Descriptor_Meta AudioSpectrumSpread::Meta(void) const {
  return Descriptor_Meta(1, 0.0f, 4.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
