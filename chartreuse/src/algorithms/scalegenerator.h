/// @file scalegenerator.h
/// @brief Scale (of time, frequency, etc.) generator algorithm definition
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_SCALEGENERATOR_H_
#define CHARTREUSE_SRC_ALGORITHMS_SCALEGENERATOR_H_

#include "Eigen/Core"

namespace chartreuse {
namespace algorithms {

/// @brief Window function type
// Using the namespace trick...
namespace Scale {
enum Type {
  kLogFreq = 0,
  kCount
};
}

/// @brief Scale generator: generates a scale for frequencies etc.
///
/// Various shapes may be generated, although it is fixed for one instance.
// TODO(gm): templatizes this if need be (e.g. performance)
class ScaleGenerator {
 public:
  /// @brief Default constructor, specifies synthesized data parameters
  ///
  /// @param[in]  length   Length of the scale in samples
  /// @param[in]  type   Type of the scale
  /// @param[in]  dft_length
  /// @param[in]  sampling_freq
  explicit ScaleGenerator(const unsigned int length,
                          const Scale::Type type,
                          const unsigned int dft_length,
                          const float sampling_freq);

  /// @brief Retrieve the scale data
  const float* Data(void) const;

 private:
  /// @brief Synthesis method: create the data with all given parameters
  ///
  /// @param[in]  type   Type of the scale to be created
  /// @param[in]  dft_length
  /// @param[in]  sampling_freq
  void SynthesizeData(const Scale::Type type,
                      const unsigned int dft_length,
                      const float sampling_freq);

   Eigen::VectorXf data_;  ///< Internal buffer for synthesized data
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_SCALEGENERATOR_H_
