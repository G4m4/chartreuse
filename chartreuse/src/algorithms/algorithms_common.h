/// @file algorithms_common.h
/// @brief Common utilities for algorithms - declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_ALGORITHMS_COMMON_H_
#define CHARTREUSE_SRC_ALGORITHMS_ALGORITHMS_COMMON_H_

namespace chartreuse {
namespace algorithms {

/// @brief Helper to retrive the nearest power of 2 of the input
unsigned int GetNearestPowerofTwo(const unsigned int value);

/// @brief Helper to check if the input is a power of 2 number
bool IsPowerOfTwo(const unsigned int value);

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_ALGORITHMS_COMMON_H_
