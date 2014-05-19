/// @file dftraw.h
/// @brief DFT "raw" algorithm wrapper declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_DFTRAW_H_
#define CHARTREUSE_SRC_ALGORITHMS_DFTRAW_H_

#include "chartreuse/src/common.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace algorithms {

/// @brief Dft "raw" algorithm implementation class
class DftRaw : public descriptors::Descriptor_Interface {
 public:
  explicit DftRaw(manager::Manager* manager);

  /// Output size: dft_length / 2 + 1
  void operator()(const float* const frame,
                  const std::size_t frame_length,
                  float* const data);

  descriptors::Descriptor_Meta Meta(void) const;

 private:
  // No assignment operator for this class
  DftRaw& operator=(const DftRaw& right);
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_DFTRAW_H_
