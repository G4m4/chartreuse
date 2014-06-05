/// @file combedsignalgenerator.cc
/// @brief Combed Signal generator algorithm implementation
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

#include "chartreuse/src/algorithms/combedsignalgenerator.h"

#include <algorithm>

#include "Eigen/Core"

#include "chartreuse/src/common.h"

namespace chartreuse {
namespace algorithms {

CombedSignalGenerator::CombedSignalGenerator(const unsigned int window_length,
                                             const unsigned int lag)
    : window_length_(window_length),
      lag_(lag) {
  CHARTREUSE_ASSERT(window_length > 0);
  CHARTREUSE_ASSERT(lag > 0);
  CHARTREUSE_ASSERT(lag < window_length);
}

CombedSignalGenerator::~CombedSignalGenerator() {
  // Nothing to do here for now
}

void CombedSignalGenerator::operator()(const float* const frame,
                                       const std::size_t frame_length,
                                       float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);
  CHARTREUSE_ASSERT(frame_length > lag_);

  const unsigned int kBaseSignalPos(window_length_ - frame_length);
  const unsigned int kLaggedSignalPos(window_length_ - frame_length - lag_);

  const Eigen::Map<const Eigen::VectorXf> base_signal(&frame[kBaseSignalPos],
                                                      frame_length);
  const Eigen::Map<const Eigen::VectorXf> lagged_signal(&frame[kLaggedSignalPos],
                                                        frame_length);
  const float kNum(base_signal.cwiseProduct(lagged_signal).sum());
  const float kDen(lagged_signal.cwiseAbs2().sum());

  const float kFactor((kDen > 0.0f) ? kNum / kDen : 0.0f);
  const Eigen::VectorXf out(base_signal - kFactor * lagged_signal);
  std::copy(out.data(), out.data() + out.size(), data);
}

}  // namespace algorithms
}  // namespace chartreuse
