/// @file audiofundamentalfrequency.cc
/// @brief AudioFundamentalFrequency descriptor implementation
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

#include "chartreuse/src/descriptors/audiofundamentalfrequency.h"

// std::floor
#include <cmath>

#include "Eigen/Core"

#include "chartreuse/src/interface/manager.h"
#include "chartreuse/src/algorithms/algorithms_common.h"

namespace chartreuse {
namespace descriptors {

AudioFundamentalFrequency::AudioFundamentalFrequency(interface::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here for now
}

void AudioFundamentalFrequency::operator()(float* const output) {
  // Retrieve current frame autocorrelation
  Process(manager_->GetDescriptor(interface::DescriptorId::kAutoCorrelation),
          manager_->AnalysisParameters().min_lag,
          manager_->AnalysisParameters().max_lag,
          output);
}

void AudioFundamentalFrequency::Process(const float* const autocorrelation,
                                        const unsigned int min_lag,
                                        const unsigned int max_lag,
                                        float* const output) {
  CHARTREUSE_ASSERT(autocorrelation != nullptr);
  CHARTREUSE_ASSERT(min_lag > 0);
  CHARTREUSE_ASSERT(max_lag > 0);
  CHARTREUSE_ASSERT(max_lag > min_lag);
  CHARTREUSE_ASSERT(output != nullptr);
  CHARTREUSE_ASSERT(output != autocorrelation);

  float argmin(0.0f);
  const unsigned int kDataLength(max_lag - min_lag);
  // TODO(gm): This is a magic, why?
  const float kThreshold(5e-3f);
  algorithms::ParabolicApproximation(&autocorrelation[min_lag],
                                     kDataLength,
                                     kThreshold,
                                     &argmin);
  output[0] = static_cast<float>(min_lag) + std::floor(argmin);
}

Descriptor_Meta AudioFundamentalFrequency::Meta(void) const {
  return Descriptor_Meta(
    1,
    static_cast<float>(manager_->AnalysisParameters().min_lag),
    static_cast<float>(manager_->AnalysisParameters().max_lag));
}

}  // namespace descriptors
}  // namespace chartreuse
