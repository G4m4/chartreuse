/// @file kissfft.h
/// @brief KissFFT wrapper implementation
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

#include "chartreuse/src/algorithms/kissfft.h"

#include <algorithm>

#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

KissFFT::KissFFT(manager::Manager* manager)
    : Descriptor_Interface(manager),
      config_(kiss_fftr_alloc(manager_->AnalysisParameters().dft_length,
                              0,
                              NULL,
                              NULL)),
      zeropad_(manager_->AnalysisParameters().dft_length + 2, 0.0f) {
  // Nothing to do here for now
}

KissFFT::~KissFFT() {
  ::free(config_);
  kiss_fft_cleanup();
}

void KissFFT::operator()(const float* const frame,
                         const std::size_t frame_length,
                         float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  const unsigned int kActualInputLength(
    // Cast for 64b systems
    std::min(static_cast<unsigned int>(frame_length),
             manager_->AnalysisParameters().dft_length));
  std::copy_n(&frame[0],
              kActualInputLength,
              &zeropad_[0]);
  return kiss_fftr(
    config_,
    &zeropad_[0],
    // TODO(gm): something cleaner?
    reinterpret_cast<kiss_fft_cpx*>(data));
}

descriptors::Descriptor_Meta KissFFT::Meta(void) const {
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
