/// @file algorithms_common.cc
/// @brief Common utilities for algorithms - implementations
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

#include "chartreuse/src/algorithms/algorithms_common.h"

// std::min, max
#include <algorithm>
// std::log
#include <cmath>

#include "chartreuse/src/common.h"

namespace chartreuse {
namespace algorithms {

unsigned int GetNearestPowerofTwo(const unsigned int value) {
  unsigned int out(value - 1);
  out |= out >> 1;
  out |= out >> 2;
  out |= out >> 4;
  out |= out >> 8;
  out |= out >> 16;
  // In case of value = 1
  const unsigned int kThreshold(2);
  return std::max(kThreshold, out + 1);
}

bool IsPowerOfTwo(const unsigned int value) {
  return (GetNearestPowerofTwo(value) == value);
}

float LogTwo(const float value) {
  CHARTREUSE_ASSERT(value > 0.0f);
  // This is 1 / log(2)
  const float kInvLog2(1.442695040888963f);
  return static_cast<float>(std::log(value) * kInvLog2);
}

float LinearInterpolation(const float left,
                          const float right,
                          const float ratio) {
  return (1.0f - ratio) * left + ratio * right;
}

float ParabolicArgMin(const float prev, const float peak, const float next) {
  const float min_position(0.5f * (prev - next) / (prev - 2.0f * peak + next));
  return min_position;
}

float ParabolicApproximation(const float* const data,
                             const unsigned int data_length,
                             const float threshold,
                             float* const argmin) {
  CHARTREUSE_ASSERT(data != nullptr);
  CHARTREUSE_ASSERT(data_length > 0);
  CHARTREUSE_ASSERT(argmin != nullptr);
  CHARTREUSE_ASSERT(data != argmin);

  float min_value(0.0f);
  float arg_min(0);
  for (unsigned int idx(1); idx < data_length - 1; ++idx) {
    const float prev(data[idx - 1]);
    const float peak(data[idx]);
    const float next(data[idx + 1]);
    // Looking for peaks:
    if (peak - prev > 0.0f) {
      if (next - peak < 0.0f) {
        const float tmp_argmin(ParabolicArgMin(prev, peak, next) + 1.0f);
        const float tmp_min(LinearInterpolation(prev, peak, tmp_argmin));
        if (tmp_min > min_value + threshold) {
          arg_min = tmp_argmin + idx;
          min_value = tmp_min;
        }
      }
    }
  }

  *argmin = arg_min;
  return min_value;
}

}  // namespace algorithms
}  // namespace chartreuse
