/// @file audiowaveform.cc
/// @brief AudioWaveform descriptor declaration
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

// std::min, max
#include <algorithm>

#include "chartreuse/src/descriptors/audiowaveform.h"

namespace chartreuse {
namespace descriptors {

AudioWaveform::AudioWaveform(manager::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here!
}

void AudioWaveform::operator()(const float* const frame,
                               const std::size_t frame_length,
                               float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  data[0] = *std::min_element(&frame[0], &frame[frame_length]);
  data[1] = *std::max_element(&frame[0], &frame[frame_length]);
}

Descriptor_Meta AudioWaveform::Meta(void) const {
  return Descriptor_Meta(2, -1.0f, 1.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
