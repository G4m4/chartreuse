/// @file audiopower.cc
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

#include "chartreuse/src/descriptors/audiopower.h"

namespace chartreuse {
namespace descriptors {

void AudioPower::operator()(const float* const frame,
                            const std::size_t frame_length,
                            float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  float power(0.0f);
  for (unsigned int i(0); i < frame_length; ++i) {
    power += frame[i] * frame[i];
  }
  const float kNormFactor(1 / static_cast<float>(frame_length));
  data[0] = power * kNormFactor;
}

}  // namespace descriptors
}  // namespace chartreuse
