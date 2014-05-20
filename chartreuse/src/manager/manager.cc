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
      // TODO(gm): this could be computed at compile-time
      descriptors_data_(dft_length + 2
                        + dft_length + 2
                        + 1
                        + 1
                        + 1
                        + 2
                        + dft_length + 2
                        + dft_length + 2, 0.0f),
      audio_power_(this),
      audio_spectrum_centroid_(this),
      audio_spectrum_spread_(this),
      audio_waveform_(this),
      dft_(this),
      spectrogram_(this),
      dft_power_(this),
      spectrogram_power_(this) {
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  CHARTREUSE_ASSERT(dft_length_ > 0);
  CHARTREUSE_ASSERT(algorithms::IsPowerOfTwo(dft_length_));
  enabled_descriptors_.fill(false);
  computed_descriptors_.fill(false);
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
  // Invalidate all previous computation (this has to be done after all
  // decriptors retrieval, since there may be dependencies between them)
  for (unsigned int descriptor_idx(0);
       descriptor_idx < DescriptorId::kCount;
       ++descriptor_idx) {
    DescriptorIsComputed(static_cast<DescriptorId::Type>(descriptor_idx),
                         false);
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
  const float* internal_data_ptr(GetDescriptor(descriptor, frame, frame_length));
  const std::size_t desc_dim(GetDescriptorSize(descriptor));
  std::copy(internal_data_ptr, internal_data_ptr + desc_dim, data);
  return desc_dim;
}

const float* Manager::GetDescriptor(const DescriptorId::Type descriptor,
                                    const float* const frame,
                                    const std::size_t frame_length) {
  // TODO(gm): this cast is ugly, remove it
  float* const internal_data_ptr(const_cast<float* const>(DescriptorDataPtr(descriptor)));
  descriptors::Descriptor_Interface* instance(nullptr);
  if (!IsDescriptorComputed(descriptor)) {
    // TODO(gm): a cleaner code!
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
      case DescriptorId::kDftPower: {
          instance = &dft_power_;
          break;
        }
      case DescriptorId::kSpectrogramPower: {
          instance = &spectrogram_power_;
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
    instance->operator()(frame, frame_length, internal_data_ptr);
    DescriptorIsComputed(descriptor, true);
  }
  return internal_data_ptr;
}

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
    case DescriptorId::kDftPower: {
        instance = &dft_power_;
        break;
      }
    case DescriptorId::kSpectrogramPower: {
        instance = &spectrogram_power_;
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

bool Manager::IsDescriptorComputed(const DescriptorId::Type descriptor) const {
  return computed_descriptors_[static_cast<int>(descriptor)];
}

void Manager::DescriptorIsComputed(const DescriptorId::Type descriptor,
                                   const bool is_computed) {
  computed_descriptors_[static_cast<int>(descriptor)] = is_computed;
}

const float* const Manager::DescriptorDataPtr(const DescriptorId::Type descriptor) const {
  DescriptorId::Type current_id(DescriptorId::kAudioPower);
  std::size_t data_offset(0);
  while (current_id != descriptor) {
    const descriptors::Descriptor_Interface* instance(nullptr);
    switch (current_id) {
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
      case DescriptorId::kDftPower: {
          instance = &dft_power_;
          break;
        }
      case DescriptorId::kSpectrogramPower: {
          instance = &spectrogram_power_;
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
    data_offset += instance->Meta().out_dim;
    current_id = static_cast<DescriptorId::Type>(++current_id);
  }  // while (current_id != descriptor)
  CHARTREUSE_ASSERT(data_offset < descriptors_data_.size());
  return &descriptors_data_[0] + data_offset;
}

}  // namespace manager
}  // namespace chartreuse
