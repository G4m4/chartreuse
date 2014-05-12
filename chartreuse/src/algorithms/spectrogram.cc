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

// TODO(gm): this should probably not be fixed
static const unsigned int kOverlap(3);

Spectrogram::Spectrogram(const unsigned int window_length,
                         const unsigned int dft_length,
                         const float sampling_freq)
    : window_length_(window_length),
      dft_length_(dft_length),
      sampling_freq_(sampling_freq),
      apodizer_(window_length, Window::kRectangular),
      dft_(),
      scratch_memory_(window_length),
      tmp_buffer_(dft_length) {
  CHARTREUSE_ASSERT(window_length > 0);
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  // The first input buffer is to be considered as the "future" part
  // in the overlap.
  // Hence, the first 2 parts ("past" and "present") have to be filled in order
  // for the internal buffer writing cursor to be at the right position
  // TODO(gm): Find a better way using an "overlap" parameter to do this
  scratch_memory_.Fill(0.0f, window_length * (kOverlap - 1) / kOverlap);
}

Spectrogram::~Spectrogram() {
  // Nothing to do here for now
};

void Spectrogram::operator()(const float* const input,
                             float* const output) {
  // Push into ringbuffer for overlap
  scratch_memory_.Push(input, window_length_ / kOverlap);
  // Pop - zero-padding done in the ringbuffer method
  scratch_memory_.PopOverlapped(&tmp_buffer_[0], dft_length_, kOverlap);
  // Apply the window
  apodizer_.ApplyWindow(&tmp_buffer_[0]);
  // Apply DFT
  dft_(&tmp_buffer_[0],
       &tmp_buffer_[dft_length_ - 1],
       false,
       dft_length_,
       output);
}

}  // namespace algorithms
}  // namespace chartreuse
