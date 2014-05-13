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

void AudioWaveform::operator()(const std::array<float, kHopSizeSamples> frame,
                               float* const data) {
  data[0] = *std::min_element(frame.begin(), frame.end());
  data[1] = *std::max_element(frame.begin(), frame.end());
}

}  // namespace descriptors
}  // namespace chartreuse
