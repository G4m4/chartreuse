/// @file audioupperlimitofharmonicity.cc
/// @brief AudioUpperLimitOfHarmonicity "ULH" part descriptor implementation
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

#include "chartreuse/src/descriptors/audioupperlimitofharmonicity.h"

#include "chartreuse/src/interface/manager.h"

namespace chartreuse {
namespace descriptors {

AudioUpperLimitOfHarmonicity::AudioUpperLimitOfHarmonicity(interface::Manager* manager)
    : Descriptor_Interface(manager) {
  // Nothing to do here for now
}

void AudioUpperLimitOfHarmonicity::operator()(float* const output) {
  IGNORE(output);
}

void AudioUpperLimitOfHarmonicity::Process(const float* const autocorrelation,
                                           const float estimated_lag,
                                           const unsigned int min_lag,
                                           float* const output) {
  IGNORE(autocorrelation);
  IGNORE(estimated_lag);
  IGNORE(min_lag);
  IGNORE(output);
}

Descriptor_Meta AudioUpperLimitOfHarmonicity::Meta(void) const {
  return Descriptor_Meta(
    1,
    -1.0f,
    1.0f);
}

}  // namespace descriptors
}  // namespace chartreuse
