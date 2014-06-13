/// @file autocorrelation.h
/// @brief Autocorrelation algorithm declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_AUTOCORRELATION_H_
#define CHARTREUSE_SRC_ALGORITHMS_AUTOCORRELATION_H_

#include "chartreuse/src/common.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace algorithms {

/// @brief Autocorrelation algorithm class:
///
/// Perform a normalized autocorrelation of the input,
/// within a limited interval
class AutoCorrelation : public descriptors::Descriptor_Interface {
 public:
  explicit AutoCorrelation(manager::Manager* manager);
  ~AutoCorrelation();

  void operator()(float* const output);

  /// @brief Independent process method: this is where the actual computation
  /// is done, to be used in a "raw" way when no manager is available
  void Process(const float* const input,
               const std::size_t input_length,
               const unsigned int min_lag,
               const unsigned int max_lag,
               float* const output);

  descriptors::Descriptor_Meta Meta(void) const;

 private:
  // No assignment operator for this class
  AutoCorrelation& operator=(const AutoCorrelation& right);
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_AUTOCORRELATION_H_
