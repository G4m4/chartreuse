/// @file dftraw.h
/// @brief DFT "raw" algorithm wrapper implementation
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

#include "chartreuse/src/algorithms/dftraw.h"

#include <cmath>

#include <algorithm>

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

DftRaw::DftRaw(manager::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here for now
}

void DftRaw::operator()(const float* const frame,
                        const std::size_t frame_length,
                        float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  const unsigned int kActualInDataLength
    // Cast for 64b systems
    = std::min(static_cast<unsigned int>(frame_length),
               manager_->AnalysisParameters().dft_length);
  const float kTwiddleBase((2.0f * chartreuse::algorithms::Pi)
                            / manager_->AnalysisParameters().dft_length);

  for (unsigned int i = 0;
       i < manager_->AnalysisParameters().dft_length / 2 + 1;
       ++i) {
    const float twiddle_factor = i * kTwiddleBase;

    float real = 0.0f;
    float imag = 0.0f;

    for (unsigned int j = 0; j < kActualInDataLength; ++j) {
      const float c = std::cos(j * twiddle_factor);
      const float s = std::sin(j * twiddle_factor);

      real += frame[j] * c;
      imag -= frame[j] * s;
    }  // iterating on input
    data[2 * i] = real;
    data[2 * i + 1] = imag;
  }  // iterating on output
}

descriptors::Descriptor_Meta DftRaw::Meta(void) const {
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
