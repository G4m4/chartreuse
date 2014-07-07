/// @file scalegenerator.cc
/// @brief Scale (of time, frequency, etc.) generator algorithm implementation
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

#include "chartreuse/src/algorithms/scalegenerator.h"

#include "Eigen/Core"

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"

namespace chartreuse {
namespace algorithms {

ScaleGenerator::ScaleGenerator(const unsigned int length,
                               const Scale::Type type,
                               const unsigned int dft_length,
                               const float sampling_freq)
    // TODO(gm): check if the data can be generated at compile-time
    : data_(length + 1, 1.0f) {
  CHARTREUSE_ASSERT(length > 0);
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(IsPowerOfTwo(dft_length));
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  SynthesizeData(type, dft_length, sampling_freq);
}

const float* ScaleGenerator::Data(void) const {
  return data_.data();
}

void ScaleGenerator::SynthesizeData(const Scale::Type type,
                                    const unsigned int dft_length,
                                    const float sampling_freq) {
  switch (type) {
    case Scale::kLogFreq: {
      const float kLowBound(2.0f * sampling_freq / dft_length);
      const float kHighBound(sampling_freq * 0.5f);
      Eigen::Array<float, Eigen::Dynamic, 1> tmp(Eigen::VectorXf::LinSpaced(Eigen::Sequential,
                                                                            data_.size(),
                                                                            kLowBound,
                                                                            kHighBound));
      const float kInvLog2(1.442695040888963f);
      Eigen::Map<Eigen::Array<float, Eigen::Dynamic, 1>> internal_data(&data_[0], data_.size());
      internal_data = (tmp * 0.001f).log() * kInvLog2;
      internal_data(0) = std::log((0.001f * sampling_freq) / (dft_length * (3.0f / 4.0f))) * kInvLog2;
      break;
    }
    default: {
      // Should never happen
      CHARTREUSE_ASSERT(false);
    }
  }
}

}  // namespace algorithms
}  // namespace chartreuse
