/// @file descriptor_interface.h
/// @brief Common interface to be implemented by all descriptors
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

#ifndef CHARTREUSE_SRC_DESCRIPTORS_DESCRIPTOR_INTERFACE_H_
#define CHARTREUSE_SRC_DESCRIPTORS_DESCRIPTOR_INTERFACE_H_

#include <array>

#include "chartreuse/src/common.h"

namespace chartreuse {
namespace descriptors {

/// @brief Define all common methods to be implemented by the descriptors
class Descriptor_Interface {
 public:
  virtual ~Descriptor_Interface() {
    // Nothing to do here for now
  }

  /// @brief Actual processing method for one frame
  ///
  /// @param[in]  frame    Frame to be analysed
  /// @param[out]  data     Descriptor output data
  /// @return nothing (not allowed to fail)
  virtual void operator()(
    const std::array<float, chartreuse::kHopSizeSamples> frame,
    float* const data) = 0;

  /// @brief Retrieve descriptor output length
  virtual unsigned int DataLength(void) const = 0;
};

}  // namespace descriptors
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_DESCRIPTORS_DESCRIPTOR_INTERFACE_H_
