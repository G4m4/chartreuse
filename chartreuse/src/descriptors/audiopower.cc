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

#include "Eigen/Core"

namespace chartreuse {
namespace descriptors {

AudioPower::AudioPower(manager::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here!
}

void AudioPower::operator()(const float* const frame,
                            const std::size_t frame_length,
                            float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  const float kNormFactor(1 / static_cast<float>(frame_length));
  data[0] = Eigen::Map<const Eigen::VectorXf>(frame, frame_length).squaredNorm()
            * kNormFactor;
}

Descriptor_Meta AudioPower::Meta(void) const {
  return Descriptor_Meta(1, 0.0f, 1.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
