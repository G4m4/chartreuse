/// @file audiofundamentalfrequency.h
/// @brief AudioFundamentalFrequency descriptor declaration
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

#ifndef CHARTREUSE_SRC_DESCRIPTORS_AUDIOFUNDAMENTALFREQUENCY_H_
#define CHARTREUSE_SRC_DESCRIPTORS_AUDIOFUNDAMENTALFREQUENCY_H_

#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace descriptors {

/// @brief AudioFundamentalFrequency descriptor: retrieve each frame estimated f0
class AudioFundamentalFrequency : public Descriptor_Interface {
 public:
  explicit AudioFundamentalFrequency(manager::Manager* manager);

  void operator()(float* const output);

  /// @brief Independent process method: this is where the actual computation
  /// is done, to be used in a "raw" way when no manager is available
  void Process(const float* const autocorrelation,
               const unsigned int min_lag,
               const unsigned int max_lag,
               float* const output);

  Descriptor_Meta Meta(void) const;

 private:
  // No assignment operator for this class
  AudioFundamentalFrequency& operator=(const AudioFundamentalFrequency& right);
};

}  // namespace descriptors
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_DESCRIPTORS_AUDIOFUNDAMENTALFREQUENCY_H_
