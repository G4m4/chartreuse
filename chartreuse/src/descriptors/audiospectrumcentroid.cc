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

AudioSpectrumCentroid::AudioSpectrumCentroid(const float sampling_freq)
    : sampling_freq_(sampling_freq),
      spectrogram_(kSpectrumWindowLength, kSpectrumDFTLength, sampling_freq),
      buffer_(kSpectrumDFTLength),
      tmp_(kSpectrumDFTLength / 2 + 1) {
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
}

void AudioSpectrumCentroid::operator()(
    const std::array<float, kHopSizeSamples> frame,
    float* const data) {
  // Get the DFT of the frame
  spectrogram_(&frame[0], &buffer_[0]);
  const float kLowEdge(62.5f);
  const unsigned int kLowEdgeIndex(static_cast<unsigned int>(
    std::ceil(kLowEdge * kSpectrumDFTLength / sampling_freq_)));
  const unsigned int kHighEdgeIndex(kSpectrumDFTLength / 2 + 1);
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
  for (unsigned int i(0); i < kLowEdgeIndex - 1; ++i) {
    tmp_[kLowEdgeIndex - 1] += tmp_[i];
  }
  const float kPowerSum(std::accumulate(&tmp_[kLowEdgeIndex - 1],
                                        &tmp_[kHighEdgeIndex - 1],
                                        0.0f) + tmp_[kHighEdgeIndex - 1]);
  // Weight each DFT bin by the log of the frequency relative to 1000Hz
  // The first bin is the low edge
  float kOut(tmp_[kLowEdgeIndex - 1] * -5.0f);
  for (unsigned int i(kLowEdgeIndex); i < kHighEdgeIndex; ++i) {
    kOut += tmp_[i] * GetLogFrequencyScale(kSpectrumDFTLength,
                                           sampling_freq_,
                                           i);
  }
  data[0] = kOut / kPowerSum;
}

unsigned int AudioSpectrumCentroid::DataLength(void) const {
  return 1;
}

float AudioSpectrumCentroid::GetLogFrequencyScale(
    const unsigned int dft_length,
    const float sampling_freq,
    const unsigned int bin_index) const {
  const float kFftFreq(static_cast<float>(bin_index) * sampling_freq / dft_length);
  // Log2 is missing in msvc (C++11)
  return static_cast<float>(std::log(kFftFreq / 1000.0f) / std::log(2.0f));
}

}  // namespace descriptors
}  // namespace chartreuse