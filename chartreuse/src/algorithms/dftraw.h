/// @file dft.h
/// @brief DFT algorithm wrapper declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_DFT_H_
#define CHARTREUSE_SRC_ALGORITHMS_DFT_H_

#include "chartreuse/src/common.h"

namespace chartreuse {
namespace algorithms {

/// @brief Dft "raw" algorithm implementation class
class DftRaw {
 public:
  explicit DftRaw(void) {
    // Nothing to do here for now
  };

  /// @brief Actual performing method
  /// Apply a N-wide points DFT on the given interval
  ///
  /// @param[in]  container      First element of the container
  /// @param[in]  length         Interval length
  /// @param[in]  is_inverted    Is DFT inverted?
  /// @param[in]  dft_length     DFT points count
  /// @param[out] dft_container  First element of the DFT container
  /// @return nothing (cannot fail)
  void operator()(const float* const begin,
                  const float* const end,
                  const bool is_inverted,
                  const unsigned int dft_length,
                  float* const dft_container);
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_DFT_H_
