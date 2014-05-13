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

namespace chartreuse {
namespace descriptors {

/// @brief Internal window length for the spectrogram, arbitrarily fixed
// TODO(gm): check were this should be put in order to be both efficient
// and usable
static const unsigned int kSpectrumWindowLength(1440);
/// @brief Internal window length for the spectrogram, arbitrarily fixed
// TODO(gm): check were this should be put in order to be both efficient
// and usable
static const unsigned int kSpectrumDFTLength(2048);

/// @brief Frequency below which all frequencies contribution are summed,
/// arbitrarily fixed
static const float kLowEdge(62.5f);

AudioSpectrumCentroid::AudioSpectrumCentroid(const float sampling_freq)
    : sampling_freq_(sampling_freq),
      kLowEdgeIndex_(static_cast<unsigned int>(
                    std::ceil(kLowEdge * kSpectrumDFTLength / sampling_freq_))),
      kHighEdgeIndex_(kSpectrumDFTLength / 2 + 1),
      spectrogram_(kSpectrumWindowLength, kSpectrumDFTLength, sampling_freq),
      freq_scale_(kHighEdgeIndex_ - kLowEdgeIndex_,
                  algorithms::Scale::kLogFreq,
                  kSpectrumDFTLength,
                  kLowEdgeIndex_,
                  sampling_freq),
      buffer_(kSpectrumDFTLength * 2),
      tmp_(kSpectrumDFTLength / 2 + 1) {
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  CHARTREUSE_ASSERT(kLowEdgeIndex_ > 0);
  CHARTREUSE_ASSERT(kHighEdgeIndex_ > 0);
  CHARTREUSE_ASSERT(kLowEdgeIndex_ < kHighEdgeIndex_);
}

void AudioSpectrumCentroid::operator()(
    const std::array<float, kHopSizeSamples> frame,
    float* const data) {
  // Get the DFT of the frame
  spectrogram_(&frame[0], &buffer_[0]);
  // Normalization of the DFT
  const float kDFTNorm(2.0f
    / static_cast<float>(kSpectrumDFTLength * kSpectrumWindowLength));
  // Retrieving the squared magnitude of the DFT
  for (unsigned int i(0); i < kSpectrumDFTLength; i += 2) {
    tmp_[i / 2] = buffer_[i] * buffer_[i] + buffer_[i + 1] * buffer_[i + 1];
    tmp_[i / 2] *= kDFTNorm;
  }
  // The DC component is unchanged, everything else is doubled
  tmp_[0] *= 0.5f;
  // Summing the contributions of all frequencies lower than 62.5
  for (unsigned int i(0); i < kLowEdgeIndex_ - 1; ++i) {
    tmp_[kLowEdgeIndex_ - 1] += tmp_[i];
  }
  const float kPowerSum(std::accumulate(&tmp_[kLowEdgeIndex_ - 1],
                                        &tmp_[kHighEdgeIndex_ - 1],
                                        0.0f) + tmp_[kHighEdgeIndex_ - 1]);
  // Weight each DFT bin by the log of the frequency relative to 1000Hz
  // The first bin is the low edge
  float kOut(tmp_[kLowEdgeIndex_ - 1] * -5.0f);
  for (unsigned int i(kLowEdgeIndex_); i < kHighEdgeIndex_; ++i) {
    kOut += tmp_[i] * freq_scale_.GetScaleAtIndex(i - kLowEdgeIndex_);
  }
  data[0] = kOut / kPowerSum;
}

unsigned int AudioSpectrumCentroid::DataLength(void) const {
  return 1;
}

}  // namespace descriptors
}  // namespace chartreuse
