/// @file spectrogram.h
/// @brief Spectrogram algorithm implementation
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

#include "chartreuse/src/algorithms/spectrogram.h"

#include <cmath>

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/algorithms/dftraw.h"

namespace chartreuse {
namespace algorithms {

const unsigned int Spectrogram::kOverlap(3);

Spectrogram::Spectrogram(const unsigned int dft_length,
                         const float sampling_freq)
    : dft_length_(dft_length),
      sampling_freq_(sampling_freq),
      apodizer_(kHopSizeSamples * kOverlap, Window::kRectangular),
      dft_(dft_length),
      scratch_memory_(kHopSizeSamples * kOverlap),
      tmp_buffer_(dft_length) {
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  // The first input buffer is to be considered as the "future" part
  // in the overlap.
  // Hence, the first 2 parts ("past" and "present") have to be filled in order
  // for the internal buffer writing cursor to be at the right position
  // TODO(gm): Find a better way using an "overlap" parameter to do this
  scratch_memory_.Fill(0.0f,
                       kHopSizeSamples * kOverlap * (kOverlap - 1) / kOverlap);
}

Spectrogram::~Spectrogram() {
  // Nothing to do here for now
}

void Spectrogram::operator()(const float* const frame,
                             const std::size_t frame_length,
                             float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  // Push into ringbuffer for overlap
  scratch_memory_.Push(frame, frame_length);
  // Pop - zero-padding done in the ringbuffer method
  scratch_memory_.PopOverlapped(&tmp_buffer_[0], dft_length_, kOverlap);
  // Apply the window
  apodizer_.ApplyWindow(&tmp_buffer_[0]);
  // Apply DFT
  dft_(&tmp_buffer_[0],
       tmp_buffer_.size(),
       data);
}

descriptors::Descriptor_Meta Spectrogram::Meta(void) const {
  return descriptors::Descriptor_Meta(
    // Not that this is the actual total length
    // (e.g. it should be half of it considering it's complex data)
    dft_length_ + 2,
    // Actually the input frame_length...
    -static_cast<float>(dft_length_),
    // Actually the input frame_length...
    static_cast<float>(dft_length_));
}

}  // namespace algorithms
}  // namespace chartreuse
