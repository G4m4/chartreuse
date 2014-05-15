/// @file audiopower.h
/// @brief AudioPower descriptor declaration
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

#ifndef CHARTREUSE_SRC_DESCRIPTORS_AUDIOPOWER_H_
#define CHARTREUSE_SRC_DESCRIPTORS_AUDIOPOWER_H_

#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace descriptors {

/// @brief AudioPower descriptor: for each frame, retrieve its square mean
class AudioPower : public Descriptor_Interface {
 public:
  void operator()(const float* const frame,
                  const std::size_t frame_length,
                  float* const data);

  static inline const Descriptor_Meta& Meta(void) {
    static const Descriptor_Meta metas(1, 0.0f, 1.0f);
    return metas;
  }
};

}  // namespace descriptors
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_DESCRIPTORS_AUDIOPOWER_H_
