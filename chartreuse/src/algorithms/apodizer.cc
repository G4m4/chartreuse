/// @file apodizer.cc
/// @brief Apodizer (window function modulation) algorithm definition
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

#include "chartreuse/src/algorithms/apodizer.h"

#include <cmath>

#include "chartreuse/src/common.h"
#include "chartreuse/src/algorithms/algorithms_common.h"

namespace chartreuse {
namespace algorithms {

Apodizer::Apodizer(const unsigned int length,
                   const Window::Type type)
    // TODO(gm): check if the data can be generated at compile-time
    // At least the default version (rectangular) is done here
    : data_(length, 1.0f) {
  CHARTREUSE_ASSERT(length > 0);
  SynthesizeData(type);
}

void Apodizer::ApplyWindow(float* const buffer) const {
  for (unsigned int i(0); i < data_.size(); ++i) {
    buffer[i] *= data_[i];
  }
}

void Apodizer::SynthesizeData(const Window::Type type) {
  switch (type) {
    case Window::kRectangular: {
      // Nothing to do, the constructor already took care of that
      break;
    }
    case Window::kHamming: {
      const float kDenom(1.0f / static_cast<float>(data_.size() - 1));
      for (unsigned int i(0); i < data_.size(); ++i) {
        const float value(2.0f * chartreuse::algorithms::Pi * static_cast<float>(i)
                          * kDenom);
        data_[i] = 0.54f - (0.46f * std::cos(value));
      }
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
