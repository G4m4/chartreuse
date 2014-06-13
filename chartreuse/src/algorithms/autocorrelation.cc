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

void AutoCorrelation::operator()(float* const output) {
  Process(manager_->CurrentWindow(),
          manager_->AnalysisParameters().window_length,
          manager_->AnalysisParameters().min_lag,
          manager_->AnalysisParameters().max_lag,
          output);
}

void AutoCorrelation::Process(const float* const input,
                              const std::size_t input_length,
                              const unsigned int min_lag,
                              const unsigned int max_lag,
                              float* const output) {
  CHARTREUSE_ASSERT(input != nullptr);
  CHARTREUSE_ASSERT(input_length > 0);
  CHARTREUSE_ASSERT(min_lag > 0);
  CHARTREUSE_ASSERT(max_lag > 0);
  CHARTREUSE_ASSERT(max_lag > min_lag);
  CHARTREUSE_ASSERT(input_length > max_lag);
  CHARTREUSE_ASSERT(output != nullptr);

  const float kPower(Eigen::Map<const Eigen::VectorXf>(&input[max_lag], input_length - max_lag).cwiseAbs2().sum());
  const Eigen::Map<const Eigen::VectorXf> right_part(&input[max_lag], input_length - max_lag);
  for (unsigned int lag(min_lag); lag < max_lag; ++lag) {
    const Eigen::Map<const Eigen::VectorXf> lagged_part(&input[max_lag - lag], input_length - max_lag);
    const float kCorrPower(right_part.cwiseProduct(lagged_part).sum());
    const float kLagPower(lagged_part.cwiseAbs2().sum());
    if (kLagPower != 0.0f) {
      output[lag - min_lag] = kCorrPower / std::sqrt(kPower * 2.0f * kLagPower);
    } else {
      output[lag - min_lag] = 0.0f;
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
