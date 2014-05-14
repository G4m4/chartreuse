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

namespace chartreuse {
namespace algorithms {

KissFFT::KissFFT(const unsigned int dft_length)
    : config_(kiss_fft_alloc(dft_length, 0, NULL, NULL)),
      zeropad_(2 * dft_length, 0.0f) {
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
}

KissFFT::~KissFFT() {
  ::free(config_);
  kiss_fft_cleanup();
}

void KissFFT::operator()(const float* const begin,
                         const float* const end,
                         const bool is_inverted,
                         const unsigned int dft_length,
                         float* const dft_container) {
  IGNORE(end);
  IGNORE(dft_length);
  IGNORE(is_inverted);
  const unsigned int kInputLength((end - begin) + 1);
  const unsigned int kActualInputLength(std::min(kInputLength, dft_length));
  const unsigned int kRemaining(dft_length - kActualInputLength);
  for (unsigned int i(0); i < std::min(kInputLength, dft_length); ++i) {
    zeropad_[2 * i] = begin[i];
  }
  //std::copy_n(begin, std::min(kInputLength, dft_length), zeropad_.begin());
  //std::fill_n(zeropad_.begin() + kActualInputLength, kRemaining, 0.0f);
  return kiss_fft(
    config_,
    // TODO(gm): something cleaner?
    reinterpret_cast<kiss_fft_cpx*>(&zeropad_[0]),
    reinterpret_cast<kiss_fft_cpx*>(dft_container));
}

}  // namespace algorithms
}  // namespace chartreuse
