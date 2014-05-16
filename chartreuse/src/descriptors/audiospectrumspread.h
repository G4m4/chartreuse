/// @file audiospectrumspread.h
/// @brief AudioSpectrumSpread descriptor declaration
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

#ifndef CHARTREUSE_SRC_DESCRIPTORS_AUDIOSPECTRUMSPREAD_H_
#define CHARTREUSE_SRC_DESCRIPTORS_AUDIOSPECTRUMSPREAD_H_

#include <vector>

#include "chartreuse/src/algorithms/scalegenerator.h"
#include "chartreuse/src/algorithms/spectrogram.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace descriptors {

/// @brief AudioPower descriptor: for each frame, retrieve its spectrum centroid
class AudioSpectrumSpread : public Descriptor_Interface {
 public:
  explicit AudioSpectrumSpread(manager::Manager* manager, float sampling_freq);

  void operator()(const float* const frame,
                  const std::size_t frame_length,
                  float* const data);

  Descriptor_Meta Meta(void) const;

 private:
  // No assignment operator for this class
  AudioSpectrumSpread& operator=(const AudioSpectrumSpread& right);

  const float sampling_freq_;  ///< Expected input data sampling frequency
  const unsigned int kLowEdgeIndex_;  ///< Dft index lower bound
  const unsigned int kHighEdgeIndex_;  ///< Dft index higher bound
  algorithms::Spectrogram spectrogram_;  ///< Internal spectrogram
  algorithms::ScaleGenerator freq_scale_;  ///< Frequency scale generator
  // TODO(gm): this could be actually static
  std::vector<float> buffer_;  ///< Internal temporary buffer
  // TODO(gm): this is useless
  std::vector<float> power_;  ///< Internal power of the signal
};

}  // namespace descriptors
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_DESCRIPTORS_AUDIOSPECTRUMSPREAD_H_
