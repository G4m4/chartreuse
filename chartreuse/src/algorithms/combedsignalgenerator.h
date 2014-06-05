/// @file combedsignalgenerator.h
/// @brief Combed signal generator algorithm declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_COMBEDSIGNALGENERATOR_H_
#define CHARTREUSE_SRC_ALGORITHMS_COMBEDSIGNALGENERATOR_H_

#include "chartreuse/src/common.h"

namespace chartreuse {
namespace algorithms {

/// @brief Combed signal generator
/// Perform a comb fitlering of the input by substracting a lagged version
/// of the signal to itself.
class CombedSignalGenerator {
 public:
  /// @brief Default constructor, the lag value has to be computed already
  ///
  /// @param[in]  window_length   Length of the signal window in samples
  /// @param[in]  lag   Lag to apply to the signal
  explicit CombedSignalGenerator(const unsigned int window_length,
                                 const unsigned int lag);
  ~CombedSignalGenerator();

  void operator()(const float* const frame,
                  const std::size_t frame_length,
                  float* const data);

 private:
  // No assignment operator for this class
  CombedSignalGenerator& operator=(const CombedSignalGenerator& right);

  const unsigned int window_length_;  ///< Input signal window length
  const unsigned int lag_;  ///< Lag value in samples
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_COMBEDSIGNALGENERATOR_H_
