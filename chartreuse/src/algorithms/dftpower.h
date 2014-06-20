/// @file dftpower.h
/// @brief DFT squared magnitude algorithm declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_DFTPOWER_H_
#define CHARTREUSE_SRC_ALGORITHMS_DFTPOWER_H_

#include "chartreuse/src/common.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace algorithms {

/// @brief Compute the Dft squared magnitude.
///
/// Note that the output is normalized, the default normalization being:
/// 2.0f / (dft length * input signal window length)
class DftPower : public descriptors::Descriptor_Interface {
 public:
  explicit DftPower(interface::Manager* manager);

  void operator()(float* const output);

  /// @brief Independent process method: this is where the actual computation
  /// is done, to be used in a "raw" way when no manager is available
  void Process(const float* const input,
               const std::size_t input_length,
               float* const output);

  descriptors::Descriptor_Meta Meta(void) const;

 private:
  // No assignment operator for this class
  DftPower& operator=(const DftPower& right);

  const float normalization_factor_;  ///< Normalization to be applied on the output
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_DFTPOWER_H_
