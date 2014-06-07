/// @file autocorrelation.cc
/// @brief Autocorrelation algorithm declarations
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

#include "chartreuse/src/algorithms/autocorrelation.h"

#include "Eigen/Core"

#include "chartreuse/src/common.h"
#include "chartreuse/src/manager/manager.h"

namespace chartreuse {
namespace algorithms {

AutoCorrelation::AutoCorrelation(manager::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here for now
}

AutoCorrelation::~AutoCorrelation() {
  // Nothing to do here for now
}

void AutoCorrelation::operator()(const float* const frame,
                                 const std::size_t frame_length,
                                 float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  const unsigned int kMinLag(manager_->AnalysisParameters().min_lag);
  const unsigned int kMaxLag(manager_->AnalysisParameters().max_lag);
  const unsigned int kWindowLength(manager_->AnalysisParameters().window_length);
  CHARTREUSE_ASSERT(kMinLag > 0);
  CHARTREUSE_ASSERT(kMaxLag > 0);
  CHARTREUSE_ASSERT(kMaxLag > kMinLag);
  CHARTREUSE_ASSERT(kWindowLength > kMaxLag);
  const float* kCurrentWindow(manager_->CurrentWindow());
  const float kPower(Eigen::Map<const Eigen::VectorXf>(&kCurrentWindow[kMaxLag], kWindowLength - kMaxLag).cwiseAbs2().sum());
  const Eigen::Map<const Eigen::VectorXf> right_part(&kCurrentWindow[kMaxLag], kWindowLength - kMaxLag);
  for (unsigned int lag(kMinLag); lag < kMaxLag; ++lag) {
    const Eigen::Map<const Eigen::VectorXf> lagged_part(&kCurrentWindow[kMaxLag - lag], kWindowLength - kMaxLag);
    const float kCorrPower(right_part.cwiseProduct(lagged_part).sum());
    const float kLagPower(lagged_part.cwiseAbs2().sum());
    if (kLagPower != 0.0f) {
      data[lag - kMinLag] = kCorrPower / std::sqrt(kPower * 2.0f * kLagPower);
    } else {
      data[lag - kMinLag] = 0.0f;
    }
  }
}

descriptors::Descriptor_Meta AutoCorrelation::Meta(void) const {
  return descriptors::Descriptor_Meta(
    manager_->AnalysisParameters().max_lag
      - manager_->AnalysisParameters().min_lag,
    - 1.0f,
    1.0f);
}

}  // namespace algorithms
}  // namespace chartreuse
