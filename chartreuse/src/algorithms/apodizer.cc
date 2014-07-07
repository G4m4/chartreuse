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

#include "Eigen/Core"

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
  const Eigen::Map<const Eigen::Array<float, Eigen::Dynamic, 1>> input(buffer, data_.size());
  Eigen::Map<Eigen::Array<float, Eigen::Dynamic, 1>> data_map(buffer, data_.size());
  const Eigen::Map<const Eigen::Array<float, Eigen::Dynamic, 1>> internal_data(&data_[0], data_.size());
  data_map = input.cwiseProduct(internal_data);
}

void Apodizer::SynthesizeData(const Window::Type type) {
  switch (type) {
    case Window::kRectangular: {
      // Nothing to do, the constructor already took care of that
      break;
    }
    case Window::kHamming: {
      const float kHighBound((2.0f * Pi * data_.size()) / (data_.size() - 1));
      const Eigen::Array<float, Eigen::Dynamic, 1> const_data(Eigen::VectorXf::Constant(data_.size(), 1, 0.54f));
      const Eigen::Array<float, Eigen::Dynamic, 1> cos_data(Eigen::VectorXf::LinSpaced(Eigen::Sequential,
                                                                data_.size(),
                                                                0.0f,
                                                                kHighBound));
      Eigen::Map<Eigen::Array<float, Eigen::Dynamic, 1>> internal_data(&data_[0], data_.size());
      internal_data = const_data - 0.46f * cos_data.cos();
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
