/// @file dftpower.h
/// @brief DFT squared magnitude algorithm implementation
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

#include "chartreuse/src/algorithms/dftpower.h"

#include <cmath>

#include <algorithm>

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

DftPower::DftPower(manager::Manager* manager)
    : Descriptor_Interface(manager),
      // TODO(gm): remove the magic, replace it by actual norm computation
      normalization_factor_(2.0f
                            / (manager_->AnalysisParameters().dft_length
                            * 571.865f)) {
  CHARTREUSE_ASSERT(normalization_factor_ > 0.0f);
}

void DftPower::operator()(const float* const frame,
                          const std::size_t frame_length,
                          float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  // Get the Dft of the frame
  const float* const dft_data(manager_->GetDescriptor(manager::DescriptorId::kDft));
  // Retrieve the normalized squared magnitude of the data
  for (std::size_t i(0);
      i < manager_->GetDescriptorMeta(manager::DescriptorId::kDft).out_dim;
       i += 2) {
    data[i / 2] = dft_data[i] * dft_data[i];
    data[i / 2] += dft_data[i + 1] * dft_data[i + 1];
    data[i / 2] *= normalization_factor_;
  }
}

descriptors::Descriptor_Meta DftPower::Meta(void) const {
  return descriptors::Descriptor_Meta(
    // Only real data
    manager_->AnalysisParameters().dft_length / 2 + 1,
    // Actually the input frame_length...
    0.0f,
    // Actually the input frame_length...
    static_cast<float>(manager_->AnalysisParameters().dft_length
                       * manager_->AnalysisParameters().dft_length)
    * normalization_factor_);
}

}  // namespace algorithms
}  // namespace chartreuse
