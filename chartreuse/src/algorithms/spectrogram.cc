/// @file spectrogram.h
/// @brief Spectrogram algorithm implementation
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

#include "chartreuse/src/algorithms/spectrogram.h"

// std::copy_n
#include <algorithm>

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/algorithms/dftraw.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

Spectrogram::Spectrogram(manager::Manager* manager)
    : Descriptor_Interface(manager),
      apodizer_(manager_->AnalysisParameters().window_length, Window::kHamming),
      tmp_buffer_(manager_->AnalysisParameters().dft_length) {
  // Nothing to do here for now
}

Spectrogram::~Spectrogram() {
  // Nothing to do here for now
}

void Spectrogram::operator()(const float* const frame,
                             const std::size_t frame_length,
                             float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  // Retrieve current window
  std::copy_n(manager_->CurrentWindow(),
              manager_->AnalysisParameters().dft_length,
              &tmp_buffer_[0]);
  // Apply the window
  apodizer_.ApplyWindow(&tmp_buffer_[0]);
  // Apply DFT
  const float* kDft(manager_->GetDescriptor(manager::DescriptorId::kDft,
                                            &tmp_buffer_[0],
                                            tmp_buffer_.size()));
  std::copy_n(kDft, manager_->AnalysisParameters().dft_length + 2, data);
}

descriptors::Descriptor_Meta Spectrogram::Meta(void) const {
  return descriptors::Descriptor_Meta(
    // Not that this is the actual total length
    // (e.g. it should be half of it considering it's complex data)
    manager_->AnalysisParameters().dft_length + 2,
    // Actually the input frame_length...
    -static_cast<float>(manager_->AnalysisParameters().dft_length),
    // Actually the input frame_length...
    static_cast<float>(manager_->AnalysisParameters().dft_length));
}

}  // namespace algorithms
}  // namespace chartreuse
