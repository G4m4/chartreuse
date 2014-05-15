/// @file spectrogram.h
/// @brief Spectrogram algorithm declarations
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_SPECTROGRAM_H_
#define CHARTREUSE_SRC_ALGORITHMS_SPECTROGRAM_H_

#include <vector>

#include "chartreuse/src/common.h"

#include "chartreuse/src/algorithms/apodizer.h"
#include "chartreuse/src/algorithms/kissfft.h"
#include "chartreuse/src/algorithms/ringbuffer.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace algorithms {

/// @brief Spectrogram algorithm class:
/// Given an input of length_input, a dft_length and window of window_size,
/// it splits the input into overlapping segments of window_size length,
/// windows each one of them, and apply to each one a DFT of dft_length
/// (input DFT segment zero-padded if need be).
class Spectrogram : public descriptors::Descriptor_Interface {
 public:
  /// @brief Default constructor, specifies fixed analysis parameters
  ///
  /// @param[in]  dft_length   Length of the DFT applied to each segment
  /// @param[in]  sampling_freq   Sampling frequency (required by the DFT)
  // TODO(gm): an "overlap" parameter should be parameterized too
  // (for now it is fixed at 3)
  explicit Spectrogram(const unsigned int dft_length,
                       const float sampling_freq);
  ~Spectrogram();

  void operator()(const float* const frame,
                  const std::size_t frame_length,
                  float* const data);

 private:
  // No assignment operator for this class
  Spectrogram& operator=(const Spectrogram& right);

   const unsigned int dft_length_;  ///< Length of the DFT
   const float sampling_freq_;  ///< Input sampling frequency
   Apodizer apodizer_;  ///< Dedicated object for window function application
   KissFFT dft_;  ///< DFT algorithm wrapper object
   RingBuffer scratch_memory_;  ///< Internal scratch memory
                                ///< for overlapped data saving
   std::vector<float> tmp_buffer_;  ///< Internal temporary buffer for DFT

  // TODO(gm): this should probably not be fixed
  static const unsigned int kOverlap;
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_SPECTROGRAM_H_
