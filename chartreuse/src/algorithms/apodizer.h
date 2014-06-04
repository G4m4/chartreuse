/// @file apodizer.h
/// @brief Apodizer (window function modulation) algorithm implementation
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_APODIZER_H_
#define CHARTREUSE_SRC_ALGORITHMS_APODIZER_H_

#include "Eigen/Core"

namespace chartreuse {
namespace algorithms {

/// @brief Window function type
// Using the namespace trick...
namespace Window {
enum Type {
  kRectangular = 0,
  kHamming,
  kCount
};
}

/// @brief Apodizer class:
/// Apply a window function on a signal.
///
/// Various shapes may be generated, although it is fixed for one instance.
// TODO(gm): templatizes this if need be (e.g. performance)
class Apodizer {
 public:
  /// @brief Default constructor, specifies synthesized data parameters
  ///
  /// @param[in]  length   Length of the window in samples
  /// @param[in]  type   Type of the window
  explicit Apodizer(const unsigned int length,
                    const Window::Type type);

  /// @brief Actual performing method for a whole buffer, in-place
  ///
  /// @param[in]  buffer    Buffer to apply the window to (of window_length)
  void ApplyWindow(float* const buffer) const;

 private:
  /// @brief Synthesis method: create the data with all given parameters
  ///
  /// @param[in]  type   Type of the window to be created
  void SynthesizeData(const Window::Type type);

   Eigen::Array<float, Eigen::Dynamic, 1> data_;  ///< Internal buffer for synthesized data
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_APODIZER_H_
