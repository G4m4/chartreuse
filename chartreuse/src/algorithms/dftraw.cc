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

void DftRaw::operator()(const float* const begin,
                        const float* const end,
                        const bool is_inverted,
                        const unsigned int dft_length,
                        float* const dft_container) {
  const unsigned int kInDataLength(end - begin + 1);
  const unsigned int kActualInDataLength = std::min(kInDataLength, dft_length);
  const float kTwiddleBase = (is_inverted) ?
      (-2.0f * chartreuse::algorithms::Pi) / dft_length
      :
      (2.0f * chartreuse::algorithms::Pi) / dft_length;

  for (unsigned int i = 0; i < dft_length; ++i) {
    const float twiddle_factor = i * kTwiddleBase;

    float real = 0.0f;
    float imag = 0.0f;

    for (unsigned int j = 0; j < kActualInDataLength; ++j) {
      const float c = std::cos(j * twiddle_factor);
      const float s = std::sin(j * twiddle_factor);

      real += begin[j] * c;
      imag -= begin[j] * s;
    }  // iterating on input
    if (is_inverted) {
      // divide by DFT length for IDFT
      real /= dft_length;
    }
    dft_container[2 * i] = real;
    dft_container[2 * i + 1] = imag;
  }  // iterating on output
}

}  // namespace algorithms
}  // namespace chartreuse
