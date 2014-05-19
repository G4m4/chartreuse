/// @file manager.cc
/// @brief Manager class implementation
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

#include "chartreuse/src/manager/manager.h"

#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace manager {

DescriptorId::Type DescriptorId::operator++(const DescriptorId::Type value) {
  const unsigned int value_int(static_cast<unsigned int>(value));
  return static_cast<DescriptorId::Type>(value_int + 1);
}

Manager::Manager(const float sampling_freq)
    : audio_power_(this),
      audio_spectrum_centroid_(this, sampling_freq),
      audio_spectrum_spread_(this, sampling_freq),
      audio_waveform_(this) {
  enabled_descriptors_.fill(false);
}

Manager::~Manager() {
  // Nothing to do here for now
}

unsigned int Manager::operator()(const float* const frame,
                                 const std::size_t frame_length,
                                 float* const data) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);
  CHARTREUSE_ASSERT(data != nullptr);

  std::size_t descriptors(0);
  DescriptorId::Type current_id(DescriptorId::kAudioPower);
  float* current_data(data);
  for (const bool enabled_descriptor : enabled_descriptors_) {
    if(enabled_descriptor) {
      current_data += GetDescriptor(current_id,
                                    frame,
                                    frame_length,
                                    current_data);
      descriptors += 1;
    }
    current_id = static_cast<DescriptorId::Type>(++current_id);
  }
  return descriptors;
}

void Manager::EnableDescriptor(const DescriptorId::Type descriptor,
                               const bool enable) {
  CHARTREUSE_ASSERT(descriptor != DescriptorId::kCount);
  enabled_descriptors_[descriptor] = enable;
}

std::size_t Manager::GetDescriptor(const DescriptorId::Type descriptor,
                                   const float* const frame,
                                   const std::size_t frame_length,
                                   float* const data) {
  descriptors::Descriptor_Interface* descriptor_instance(nullptr);
  switch (descriptor) {
    case DescriptorId::kAudioPower: {
        descriptor_instance = &audio_power_;
        break;
      }
    case DescriptorId::kAudioSpectrumCentroid: {
        descriptor_instance = &audio_spectrum_centroid_;
        break;
      }
    case DescriptorId::kAudioSpectrumSpread: {
        descriptor_instance = &audio_spectrum_spread_;
        break;
      }
    case DescriptorId::kAudioWaveform: {
        descriptor_instance = &audio_waveform_;
        break;
      }
    case DescriptorId::kCount:
    default: {
      // Should never happen
      CHARTREUSE_ASSERT(false);
        break;
      }
  }  // switch (descriptor)
  descriptor_instance->operator()(frame, frame_length, data);
  return descriptor_instance->Meta().out_dim;
}

}  // namespace manager
}  // namespace chartreuse
