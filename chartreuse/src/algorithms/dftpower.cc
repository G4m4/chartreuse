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

void DftPower::operator()(float* const output) {
  Process(manager_->GetDescriptor(manager::DescriptorId::kDft),
          manager_->GetDescriptorMeta(manager::DescriptorId::kDft).out_dim,
          output);
}

void DftPower::Process(const float* const input,
                       const std::size_t input_length,
                       float* const output) {
  CHARTREUSE_ASSERT(input != nullptr);
  CHARTREUSE_ASSERT(input_length > 0);
  CHARTREUSE_ASSERT(output != nullptr);
  CHARTREUSE_ASSERT(input != output);

  // Retrieve the normalized squared magnitude of the dft data
  for (std::size_t i(0);
       i < input_length;
       i += 2) {
    output[i / 2] = input[i] * input[i];
    output[i / 2] += input[i + 1] * input[i + 1];
    output[i / 2] *= normalization_factor_;
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
