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

#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/common.h"

namespace chartreuse {
namespace algorithms {

KissFFT::KissFFT(const unsigned int dft_length)
    : config_(kissfft::kiss_fftr_alloc(dft_length, 0, NULL, NULL)) {
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
}

void KissFFT::operator()(const float* const begin,
                         const float* const end,
                         const bool is_inverted,
                         const unsigned int dft_length,
                         float* const dft_container) {
  IGNORE(end);
  IGNORE(dft_length);
  IGNORE(is_inverted);
  return kissfft::kiss_fftr(
    config_,
    begin,
    reinterpret_cast<kissfft::kiss_fft_cpx*>(dft_container));
}

}  // namespace algorithms
}  // namespace chartreuse
