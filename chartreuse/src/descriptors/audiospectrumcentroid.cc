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

#include "Eigen/Core"

#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace descriptors {

AudioSpectrumCentroid::AudioSpectrumCentroid(manager::Manager* manager)
    : Descriptor_Interface(manager),
      // TODO(gm): remove this magic
      normalization_factor_(manager->AnalysisParameters().dft_length * 571.865f) {
  // Nothing to do here for now
}

void AudioSpectrumCentroid::operator()(float* const output) {
  Process(manager_->GetDescriptor(manager::DescriptorId::kSpectrogramPower),
          output,
          manager_->FrequencyScale(),
          manager_->AnalysisParameters().low_edge,
          manager_->AnalysisParameters().high_edge);
}

void AudioSpectrumCentroid::Process(const float* const spectrogram_power,
                                    float* const output,
                                    const float* const frequency_scale,
                                    const unsigned int low_edge_idx,
                                    const unsigned int high_edge_idx) {
  CHARTREUSE_ASSERT(spectrogram_power != nullptr);
  CHARTREUSE_ASSERT(output != nullptr);
  CHARTREUSE_ASSERT(spectrogram_power != output);
  CHARTREUSE_ASSERT(spectrogram_power != frequency_scale);
  CHARTREUSE_ASSERT(frequency_scale != nullptr);
  CHARTREUSE_ASSERT(low_edge_idx > 0);
  CHARTREUSE_ASSERT(high_edge_idx > 0);
  CHARTREUSE_ASSERT(high_edge_idx > low_edge_idx);

  // Get the normalized squared magnitude spectrogram of the frame
  Eigen::Array<float, Eigen::Dynamic, 1> power(
    Eigen::Map<const Eigen::Array<float, Eigen::Dynamic, 1>>(spectrogram_power,
                                                             high_edge_idx));
  // The DC component is unchanged, everything else is doubled
  power[0] *= 0.5f;
  power *= 2.0f / normalization_factor_;
  // Summing the contributions of all frequencies lower than 62.5
  for (unsigned int i(0); i < low_edge_idx - 1; ++i) {
    power[low_edge_idx - 1] += power[i];
  }
  const float kPowerSum(power.tail(high_edge_idx - low_edge_idx + 1).sum()
    // Prevent divide by zero
    + 1e-7f);
  CHARTREUSE_ASSERT(kPowerSum > 0.0f);
  // Weight each DFT bin by the log of the frequency relative to 1000Hz
  // The first bin is the low edge
  const Eigen::Array<float, Eigen::Dynamic, 1> FreqScale(Eigen::Map<const Eigen::Array<float, Eigen::Dynamic, 1>>(frequency_scale,
    high_edge_idx - low_edge_idx + 1));
  const float kOut(power.tail(high_edge_idx - low_edge_idx + 1).cwiseProduct(FreqScale).sum());
  //float kOut(power_[kLowEdgeIndex_ - 1] * -5.0f);
  //for (unsigned int i(kLowEdgeIndex_); i < kHighEdgeIndex_; ++i) {
  //  kOut += power_[i] * freq_scale_.Data()[i - kLowEdgeIndex_];
  //}
  output[0] = kOut / kPowerSum;
}

Descriptor_Meta AudioSpectrumCentroid::Meta(void) const {
  return Descriptor_Meta(1, -5.0f, 5.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
