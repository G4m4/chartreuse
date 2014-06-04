/// @file spectrogrampower.h
/// @brief Spectrogram squared magnitude algorithm implementation
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

#include "chartreuse/src/algorithms/spectrogrampower.h"

#include <algorithm>
#include <complex>

#include "Eigen/Core"

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

SpectrogramPower::SpectrogramPower(manager::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here for now
}

void SpectrogramPower::operator()(const float* const frame,
                                  const std::size_t frame_length,
                                  float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  // Get the Dft of the frame
  const float* spectrogram(manager_->GetDescriptor(
                                   manager::DescriptorId::kSpectrogram,
                                   &frame[0],
                                   frame_length));
  const std::complex<float>* spectrogram_casted(reinterpret_cast<const std::complex<float>*>(spectrogram));
  const unsigned int spectro_length(manager_->GetDescriptorSize(manager::DescriptorId::kSpectrogram) / 2);

  // Retrieve the normalized squared magnitude of the data
  const Eigen::VectorXf tmp(Eigen::Map<const Eigen::VectorXcf>(spectrogram_casted, spectro_length).cwiseAbs2());
  std::copy(tmp.data(), tmp.data() + tmp.size(), data);
}

descriptors::Descriptor_Meta SpectrogramPower::Meta(void) const {
  return descriptors::Descriptor_Meta(
    // Only real data
    manager_->AnalysisParameters().dft_length / 2 + 1,
    // Actually the input frame_length...
    0.0f,
    // Actually the input frame_length...
    static_cast<float>(manager_->AnalysisParameters().dft_length
                       * manager_->AnalysisParameters().dft_length)
    * 1.0f);
}

}  // namespace algorithms
}  // namespace chartreuse
