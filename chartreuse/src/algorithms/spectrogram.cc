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

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/algorithms/dftraw.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

Spectrogram::Spectrogram(manager::Manager* manager)
    : Descriptor_Interface(manager),
      dft_(manager) {
  // Nothing to do here for now
}

Spectrogram::~Spectrogram() {
  // Nothing to do here for now
}

void Spectrogram::operator()(float* const output) {
  Process(manager_->CurrentFrame(),
          manager_->AnalysisParameters().hop_size_sample,
          manager_->AnalysisParameters().dft_length,
          output);
}
void Spectrogram::Process(const float* const input,
                          const std::size_t input_length,
                          const unsigned int dft_length,
                          float* const output) {
  CHARTREUSE_ASSERT(input != nullptr);
  CHARTREUSE_ASSERT(input_length > 0);
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
  CHARTREUSE_ASSERT(output != nullptr);
  CHARTREUSE_ASSERT(input != output);

  dft_.Process(manager_->CurrentWindowApodized(),
               manager_->AnalysisParameters().dft_length,
               manager_->AnalysisParameters().dft_length,
               output);
}

descriptors::Descriptor_Meta Spectrogram::Meta(void) const {
  return descriptors::Descriptor_Meta(
    // Not that this is the actual total length
    // (e.g. it should be half of it considering it's complex data)
    manager_->AnalysisParameters().dft_length + 2,
    // Actually the input frame_length...
    -static_cast<float>(manager_->AnalysisParameters().dft_length),
    // Actually the input frame_length...
    static_cast<float>(manager_->AnalysisParameters().dft_length));
}

}  // namespace algorithms
}  // namespace chartreuse
