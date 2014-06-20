/// @file interface_common.h
/// @brief Common interface declarations
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

#ifndef CHARTREUSE_SRC_INTERFACE_INTERFACE_COMMON_H_
#define CHARTREUSE_SRC_INTERFACE_INTERFACE_COMMON_H_

namespace chartreuse {
namespace interface {

namespace DescriptorId {

/// @brief Any available descriptor is uniquely identified by this constant
enum Type {
  kAudioPower = 0,
  kAudioSpectrumCentroid,
  kAudioSpectrumSpread,
  kAudioWaveform,
  kAudioFundamentalFrequency,
  kAudioHarmonicity,
  kDft,
  kSpectrogram,
  kDftPower,
  kSpectrogramPower,
  kAutoCorrelation,
  kCount
};

/// @brief Pre-Increment operator for the enum
static inline Type operator++(const Type value) {
  const unsigned int value_int(static_cast<unsigned int>(value));
  return static_cast<DescriptorId::Type>(value_int + 1);
}

}  // namespace DescriptorId

}  // namespace interface
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_INTERFACE_INTERFACE_COMMON_H_
