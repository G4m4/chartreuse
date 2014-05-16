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

namespace chartreuse {
namespace algorithms {

DftRaw::DftRaw(const unsigned int dft_length)
    : dft_length_(dft_length) {
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
}

void DftRaw::operator()(const float* const frame,
                        const std::size_t frame_length,
                        float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  const unsigned int kActualInDataLength
    // Cast for 64b systems
    = std::min(static_cast<unsigned int>(frame_length), dft_length_);
  const float kTwiddleBase((2.0f * chartreuse::algorithms::Pi) / dft_length_);

  for (unsigned int i = 0; i < dft_length_ / 2 + 1; ++i) {
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
    dft_length_ + 2,
    // Actually the input frame_length...
    -static_cast<float>(dft_length_),
    // Actually the input frame_length...
    static_cast<float>(dft_length_));
}

}  // namespace algorithms
}  // namespace chartreuse
