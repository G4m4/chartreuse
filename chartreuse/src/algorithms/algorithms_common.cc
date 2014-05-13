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
  // This is 1 / log(2)
  const float kInvLog2(1.442695040888963f);
  return static_cast<float>(std::log(value) * kInvLog2);
}

}  // namespace algorithms
}  // namespace chartreuse
