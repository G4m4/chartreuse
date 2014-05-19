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

#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace manager {

/// @brief Internal window length for the spectrogram, arbitrarily fixed
// TODO(gm): check were this should be put in order to be both efficient
// and usable
const unsigned int Manager::kSpectrumWindowLength(1440);
/// @brief Internal window length for the spectrogram, arbitrarily fixed
// TODO(gm): check were this should be put in order to be both efficient
// and usable
const unsigned int Manager::kSpectrumDftLength(2048);

DescriptorId::Type DescriptorId::operator++(const DescriptorId::Type value) {
  const unsigned int value_int(static_cast<unsigned int>(value));
  return static_cast<DescriptorId::Type>(value_int + 1);
}

Manager::Manager(const float sampling_freq,
                 const unsigned int dft_length)
    : sampling_freq_(sampling_freq),
      dft_length_(dft_length),
      audio_power_(this),
      audio_spectrum_centroid_(this),
      audio_spectrum_spread_(this),
      audio_waveform_(this),
      dft_(this),
      spectrogram_(this) {
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  CHARTREUSE_ASSERT(dft_length_ > 0);
  CHARTREUSE_ASSERT(algorithms::IsPowerOfTwo(dft_length_));
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
    }  // for (const bool enabled_descriptor : enabled_descriptors_)
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
  // TODO(gm): a cleaner code!
  descriptors::Descriptor_Interface* instance(nullptr);
std::size_t Manager::GetDescriptorSize(
    const DescriptorId::Type descriptor) const {
  // TODO(gm): a cleaner code!
  const descriptors::Descriptor_Interface* instance(nullptr);
  switch (descriptor) {
    case DescriptorId::kAudioPower: {
        instance = &audio_power_;
        break;
      }
    case DescriptorId::kAudioSpectrumCentroid: {
        instance = &audio_spectrum_centroid_;
        break;
      }
    case DescriptorId::kAudioSpectrumSpread: {
        instance = &audio_spectrum_spread_;
        break;
      }
    case DescriptorId::kAudioWaveform: {
        instance = &audio_waveform_;
        break;
      }
    case DescriptorId::kDft: {
        instance = &dft_;
        break;
      }
    case DescriptorId::kSpectrogram: {
        instance = &spectrogram_;
        break;
      }
    case DescriptorId::kCount:
    default: {
        // Should never happen
        CHARTREUSE_ASSERT(false);
        break;
      }
  }  // switch (descriptor)
  CHARTREUSE_ASSERT(instance != nullptr);
  instance->operator()(frame, frame_length, data);
  return instance->Meta().out_dim;
}

std::size_t Manager::DescriptorsOutputSize(void) const {
  std::size_t out(0);
  DescriptorId::Type current_id(DescriptorId::kAudioPower);
  for (const bool enabled_descriptor : enabled_descriptors_) {
    if(enabled_descriptor) {
      out += GetDescriptorSize(current_id);
    }  // for (const bool enabled_descriptor : enabled_descriptors_)
    current_id = static_cast<DescriptorId::Type>(++current_id);
  }
  return out;
}

unsigned int Manager::DftLength(void) const {
  return dft_length_;
}

float Manager::SamplingFrequency(void) const {
  return sampling_freq_;
}

}  // namespace manager
}  // namespace chartreuse
