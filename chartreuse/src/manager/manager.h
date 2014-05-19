/// @file manager.h
/// @brief Manager class declarations
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

#ifndef CHARTREUSE_SRC_MANAGER_MANAGER_H_
#define CHARTREUSE_SRC_MANAGER_MANAGER_H_

#include <array>

#include "chartreuse/src/common.h"

#include "chartreuse/src/algorithms/kissfft.h"
#include "chartreuse/src/algorithms/spectrogram.h"

#include "chartreuse/src/descriptors/audiopower.h"
#include "chartreuse/src/descriptors/audiospectrumcentroid.h"
#include "chartreuse/src/descriptors/audiospectrumspread.h"
#include "chartreuse/src/descriptors/audiowaveform.h"

namespace chartreuse {
namespace manager {

namespace DescriptorId {

/// @brief Any available descriptor is uniquely identified by this constant
enum Type {
  kAudioPower = 0,
  kAudioSpectrumCentroid,
  kAudioSpectrumSpread,
  kAudioWaveform,
  kDft,
  kSpectrogram,
  kCount
};

/// @brief Pre-Increment operator for the enum
Type operator++(const Type value);

}  // namespace DescriptorId

/// @brief Manager class:
/// Handle multiple descriptors retrieval in an efficient manner,
/// by batching common processing between numerous descriptors.
///
/// This should be your preferred way to retrieve descriptors from any input.
class Manager {
 public:
  explicit Manager(const float sampling_freq);
  ~Manager();

  /// @brief Main processing function
  ///
  /// Retrieve all activated descriptors, for the given frame.
  ///
  /// Descriptors value will be stored in the output data array,
  /// in their order of declaration in DescriptorId
  ///
  /// @param[in]  frame    Frame to be analysed
  /// @param[in]  frame_length    Input frame length
  /// @param[out]  data     Descriptor output data
  /// (total length is the sum of dimensionality of all activated descriptors)
  ///
  /// @return Actual count of processed descriptors
  unsigned int operator()(const float* const frame,
                          const std::size_t frame_length,
                          float* const data);

  /// @brief Descriptor enabling
  ///
  /// Activate/deactivate the given descriptor,
  /// which will then be extracted at the next processing method call.
  ///
  /// @param[in]  descriptor    Descriptor to be enabled
  /// @param[in]  enable    True to enable
  void EnableDescriptor(const DescriptorId::Type descriptor,
                        const bool enable);

  /// @brief Per-descriptor processing function
  ///
  /// Retrieve the descriptor, even if not previously activated.
  ///
  /// @param[in]  descriptor    Descriptor to be retrieved
  /// @param[in]  frame    Frame to be analysed
  /// @param[in]  frame_length    Input frame length
  /// @param[out]  data     Descriptor output data
  ///
  /// @return Chosen descriptor output data length
  std::size_t GetDescriptor(const DescriptorId::Type descriptor,
                            const float* const frame,
                            const std::size_t frame_length,
                            float* const data);

  /// @brief Descriptor output size
  ///
  /// Retrieve total size of all enabled descriptors
  std::size_t DescriptorsOutputSize(void) const;

  static const unsigned int kSpectrumWindowLength;
  static const unsigned int kSpectrumDftLength;

 private:
  // No assignment operator for this class
  Manager& operator=(const Manager& right);

  std::array<bool, DescriptorId::kCount> enabled_descriptors_;
  // TODO(gm): use a smarter factory
  descriptors::AudioPower audio_power_;
  descriptors::AudioSpectrumCentroid audio_spectrum_centroid_;
  descriptors::AudioSpectrumSpread audio_spectrum_spread_;
  descriptors::AudioWaveform audio_waveform_;
  algorithms::KissFFT dft_;
  algorithms::Spectrogram spectrogram_;
};

}  // namespace manager
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_MANAGER_MANAGER_H_
