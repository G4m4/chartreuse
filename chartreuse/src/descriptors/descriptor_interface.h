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

#include "chartreuse/src/common.h"

namespace chartreuse {

// Internal forward declaration
namespace manager {
class Manager;
}  // namespace manager

namespace descriptors {

/// @brief Struct holding a descriptor "metadata",
/// e.g. various meta informations such as its output bounds
///
/// Each descriptor specialization has its own, static one.
// TODO(gm): find a way to to do this in a 100% static way
struct Descriptor_Meta {
  Descriptor_Meta(const unsigned int out_dim,
                  const float out_min,
                  const float out_max)
    : out_dim(out_dim),
      out_min(out_min),
      out_max(out_max) {
    CHARTREUSE_ASSERT(out_dim > 0);
    // TODO(gm): These have to be static asserts
    CHARTREUSE_ASSERT(out_min < out_max);
  };

  const unsigned int out_dim;  ///< Output dimensionality
  const float out_min;  ///< Lower bound for output
  const float out_max;  ///< Higher bound for output

 private:
  // No assignment operator for this class
  Descriptor_Meta& operator=(const Descriptor_Meta& right);
};

/// @brief Define all common methods to be implemented by the descriptors
class Descriptor_Interface {
 public:
   /// @brief Constructor, mandatory requires to give the manager to be used
  explicit Descriptor_Interface(manager::Manager* manager)
      : manager_(manager) {
    CHARTREUSE_ASSERT(manager != nullptr);
  }

  virtual ~Descriptor_Interface() {
    // Nothing to do here for now
  }

  /// @brief Actual processing method for one frame
  ///
  /// @param[in]  frame    Frame to be analysed
  /// @param[in]  frame_length    Input frame length
  /// @param[out]  data     Descriptor output data
  /// @return nothing (not allowed to fail)
  virtual void operator()(
    const float* const frame,
    const std::size_t frame_length,
    float* const data) = 0;

  /// @brief Retrieve descriptor metadata
  virtual Descriptor_Meta Meta(void) const = 0;

 protected:
  manager::Manager* manager_;  ///< Internal access to common manager
};

}  // namespace descriptors
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_DESCRIPTORS_DESCRIPTOR_INTERFACE_H_
