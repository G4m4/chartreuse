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
#include <vector>

#include "chartreuse/src/common.h"

#include "chartreuse/src/algorithms/autocorrelation.h"
#include "chartreuse/src/algorithms/dftpower.h"
#include "chartreuse/src/algorithms/kissfft.h"
#include "chartreuse/src/algorithms/ringbuffer.h"
#include "chartreuse/src/algorithms/spectrogram.h"
#include "chartreuse/src/algorithms/spectrogrampower.h"

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
  kDftPower,
  kSpectrogramPower,
  kAutoCorrelation,
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
  /// @brief Manager parameters class
  /// Hold all common parameters for internal descriptors retrieval
  ///
  /// These parameters cannot be changed throughout all the manager life.
  class Parameters {
   public:
    /// @brief Default constructor, all default parameters value defined here
    Parameters(const float sampling_freq = 48000.0f,
               const unsigned int dft_length = 2048,
               const float low_freq = 62.5f,
               const float high_freq = 1500.0f,
               const unsigned int hop_size_sample = 480,
               const unsigned int overlap = 3);

    const float sampling_freq;  ///< Analysis sampling frequency
    const unsigned int dft_length;  ///< Spectrum signal length
    const float low_freq;  ///< Lower bound for analysis frequency spectrum
    const float high_freq;  ///< Higher bound for analysis frequency spectrum
    const unsigned int min_lag;  ///< Smaller lag for analysis given the above
    const unsigned int max_lag;  ///< Higher lag for analysis given the above
    const unsigned int hop_size_sample;  ///< Input signal length
    const unsigned int overlap;  ///< Accumulated input signal overlap count
    const unsigned int window_length;  ///< Accumulated input signal length

   private:
    // No assignment operator for this class
    Parameters& operator=(const Parameters& right);
  };

  /// @brief Constructor, parameters have to be passed to it (no default)
  ///
  /// @param[in]  parameters    Analysis parameters to use
  /// @param[in]  zero_init   Zero initialization of internal memory,
  /// in order to compensate the missing beginning for all overlap algorithms
  explicit Manager(const Parameters& parameters, const bool zero_init = true);
  ~Manager();

  /// @brief Main processing function
  ///
  /// Feed the manager with the next signal frame.
  ///
  /// Note that calling this function will invalidate all previously computed
  /// data, e.g. after calling this function all descriptors will be evaluated
  /// on the new given frame
  ///
  /// @param[in]  frame    Frame to be analysed
  /// @param[in]  frame_length    Input frame length
  ///
  /// @return Nothing - cannot fail
  void ProcessFrame(const float* const frame,
                    const std::size_t frame_length);

  /// @brief Descriptor enabling
  ///
  /// Activate/deactivate the given descriptor,
  /// which will then be extracted at the next processing method call.
  ///
  /// @param[in]  descriptor    Descriptor to be enabled
  /// @param[in]  enable    True to enable
  void EnableDescriptor(const DescriptorId::Type descriptor,
                        const bool enable);

  /// @brief Per-descriptor processing function, read-only version
  ///
  /// Retrieve the descriptor, even if not previously activated.
  /// This version only returns a const pointer:
  /// hence no copy of any sort is done on the output.
  ///
  /// @param[in]  descriptor    Descriptor to be retrieved
  /// @param[in]  frame    Frame to be analysed
  /// @param[in]  frame_length    Input frame length
  ///
  /// @return pointer to the first element of computed data
  const float* GetDescriptor(const DescriptorId::Type descriptor,
                             const float* const frame,
                             const std::size_t frame_length);

  /// @brief Retrieve the given descriptor metadata
  descriptors::Descriptor_Meta GetDescriptorMeta(
    const DescriptorId::Type descriptor) const;

  /// @brief Check if the given descriptor was computed for the current frame
  bool IsDescriptorComputed(const DescriptorId::Type descriptor) const;

  /// @brief Descriptor output size
  ///
  /// Retrieve total size of all enabled descriptors
  std::size_t DescriptorsOutputSize(void) const;

  /// @brief Analysis parameters getter
  const Parameters& AnalysisParameters(void) const;

  /// @brief Retrieve current data
  const float* CurrentFrame(void) const;

  /// @brief Retrieve current (overlapped) data window
  const float* CurrentWindow(void) const;

 private:
  // No assignment operator for this class
  Manager& operator=(const Manager& right);

  /// @brief Set a descriptor as "computed" for the current frame
  void DescriptorIsComputed(const DescriptorId::Type descriptor,
                            const bool is_computed);

  /// @brief Retrieve the pointer for internal data buffer given the descriptor
  const float* DescriptorDataPtr(const DescriptorId::Type descriptor) const;

  std::array<bool, DescriptorId::kCount> enabled_descriptors_;
  std::array<bool, DescriptorId::kCount> computed_descriptors_;
  std::vector<float> descriptors_data_;  ///< Temporary buffer
                                         ///< holding descriptors data result
  std::vector<float> current_frame_;  ///< Internal scratch memory
                                       ///< for input data saving
  std::vector<float> current_window_;  ///< Internal scratch memory
                                       ///< for overlapped data saving
  const Parameters parameters_;

  // TODO(gm): use a smarter factory
  descriptors::AudioPower audio_power_;
  descriptors::AudioSpectrumCentroid audio_spectrum_centroid_;
  descriptors::AudioSpectrumSpread audio_spectrum_spread_;
  descriptors::AudioWaveform audio_waveform_;
  algorithms::RingBuffer ringbuf_;
  algorithms::AutoCorrelation autocorrelation_;
  algorithms::KissFFT dft_;
  algorithms::Spectrogram spectrogram_;
  algorithms::DftPower dft_power_;
  algorithms::SpectrogramPower spectrogram_power_;
};

}  // namespace manager
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_MANAGER_MANAGER_H_
