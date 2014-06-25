/// @file analyzer.h
/// @brief Analyzer class declaration
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

#ifndef CHARTREUSE_SRC_INTERFACE_ANALYZER_H_
#define CHARTREUSE_SRC_INTERFACE_ANALYZER_H_

#include <array>

#include "chartreuse/src/algorithms/ringbuffer.h"
#include "chartreuse/src/interface/interface_common.h"

namespace chartreuse {
namespace interface {

// Internal namespace
class Manager;

/// @brief Available descriptors count
// TODO(gm): this could be cleaner since the size of the array below
// is known at compile-time...
static const unsigned int kAvailableDescriptorsCount(5);

/// @brief Descriptors computed by the Analyzer.
/// Note that this is a compile time known list.
static const std::array<DescriptorId::Type,
                        kAvailableDescriptorsCount> kAvailableDescriptors = {{
  DescriptorId::kAudioPower,
  DescriptorId::kAudioSpectrumCentroid,
  DescriptorId::kAudioSpectrumSpread,
  DescriptorId::kAudioFundamentalFrequency,
  DescriptorId::kAudioHarmonicity
}};

/// @brief Analyzer class: wraps and hide the manager implementation details
/// and external library use (such as Eigen).
///
/// Also handles input/output format adaption - the manager audio input format
/// being fixed.
///
/// This should be your preferred way to retrieve descriptors from any input.
/// The descriptors retrieved here cannot be choosen at runtime.
/// @see kAvailableDescriptors
class Analyzer {
 public:
  explicit Analyzer(const float sampling_freq);
  ~Analyzer();

  /// @brief Actual process function, feed a frame whatever its length is.
  /// Retrieve all available audio descriptors for all subframes.
  ///
  /// @param[in]  input   Input frame data
  /// @param[in]  length   Input frame length
  /// @param[out]  output   Output data, in a flattened structure as follows:
  ///
  /// subframe1_desc_1 ... subframe1_desc_N subframe2_desc_1 ... subframe2_desc_N
  ///
  /// @return Actual count of processed subframes
  unsigned int Process(const float* const input,
                       const unsigned int length,
                       float* const output);

 private:

  /// @brief Normalization helper method: wraps the normalization
  /// from [in_min; in_max] into [0.0f ; 1.0f]
  ///
  /// @param[in]  input   Input value
  /// @param[in]  in_min   Input lower bound
  /// @param[in]  in_max   Input higher bound
   float Normalize(const float input,
                   const float in_min,
                   const float in_max) const;

  Manager& desc_manager_;  ///< Audio descriptor manager
  algorithms::RingBuffer buffer_;  ///< Internal buffer for data framing
};

}  // namespace interface
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_INTERFACE_ANALYZER_H_
