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

#include <cmath>

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/dftraw.h"

namespace chartreuse {
namespace algorithms {

/// @brief Helper to retrive the nearest power of 2 of the input
unsigned int GetNearestPowerofTwo(const unsigned int value) {
  unsigned int out(value - 1);
  out |= out >> 1;
  out |= out >> 2;
  out |= out >> 4;
  out |= out >> 8;
  out |= out >> 16;
  // In case of value = 1
  const unsigned int kThreshold(2);
  return std::max(kThreshold, out);
}

/// @brief Helper to check if the input is a power of 2 number
bool IsPowerOfTwo(const unsigned int value) {
  return (GetNearestPowerofTwo(value) == value);
}

Spectrogram::Spectrogram(const unsigned int window_length,
                         const unsigned int dft_length,
                         const float sampling_freq)
    : window_length_(window_length),
      dft_length_(dft_length),
      sampling_freq_(sampling_freq),
      scratch_memory_(window_length, 0.0f) {
  CHARTREUSE_ASSERT(window_length > 0);
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
}

void Spectrogram::operator()(const float* const input,
                             const unsigned int length,
                             float* const output) {
}

}  // namespace algorithms
}  // namespace chartreuse
