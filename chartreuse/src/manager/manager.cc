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

// std::floor
#include <cmath>

#include "chartreuse/src/algorithms/algorithms_common.h"
#include "chartreuse/src/descriptors/descriptor_interface.h"

namespace chartreuse {
namespace manager {

DescriptorId::Type DescriptorId::operator++(const DescriptorId::Type value) {
  const unsigned int value_int(static_cast<unsigned int>(value));
  return static_cast<DescriptorId::Type>(value_int + 1);
}

Manager::Parameters::Parameters(const float sampling_freq,
                                const unsigned int dft_length,
                                const float low_freq,
                                const float high_freq,
                                const unsigned int hop_size_sample,
                                const unsigned int overlap)
    : sampling_freq(sampling_freq),
      dft_length(dft_length),
      low_freq(low_freq),
      high_freq(high_freq),
      min_lag(static_cast<unsigned int>(std::floor(sampling_freq / high_freq))),
      max_lag(static_cast<unsigned int>(std::floor(sampling_freq / low_freq))),
      hop_size_sample(hop_size_sample),
      overlap(overlap),
      window_length(hop_size_sample * overlap) {
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  CHARTREUSE_ASSERT(dft_length > 0);
  CHARTREUSE_ASSERT(algorithms::IsPowerOfTwo(dft_length));
  CHARTREUSE_ASSERT(low_freq < sampling_freq / 2.0f);
  CHARTREUSE_ASSERT(high_freq < sampling_freq / 2.0f);
  CHARTREUSE_ASSERT(low_freq > 0.0f);
  CHARTREUSE_ASSERT(high_freq > 0.0f);
  CHARTREUSE_ASSERT(high_freq > low_freq);
  CHARTREUSE_ASSERT(min_lag > 0);
  CHARTREUSE_ASSERT(max_lag > 0);
  CHARTREUSE_ASSERT(max_lag > min_lag);
  CHARTREUSE_ASSERT(hop_size_sample > 0);
  CHARTREUSE_ASSERT(overlap >= 1);
  CHARTREUSE_ASSERT(window_length > 0);
  CHARTREUSE_ASSERT(window_length >= hop_size_sample);
}

Manager::Manager(const Parameters& parameters, const bool zero_init)
    : enabled_descriptors_(),
      computed_descriptors_(),
      // TODO(gm): this could be computed at compile-time
      descriptors_data_(parameters.dft_length + 2
                        + parameters.dft_length + 2
                        + 1
                        + 1
                        + 1
                        + 2
                        + parameters.dft_length + 2
                        + parameters.dft_length + 2, 0.0f),
      current_frame_(parameters.hop_size_sample),
      current_window_(parameters.dft_length),
      parameters_(parameters),
      audio_power_(this),
      audio_spectrum_centroid_(this),
      audio_spectrum_spread_(this),
      audio_waveform_(this),
      ringbuf_(parameters.window_length),
      autocorrelation_(this),
      dft_(this),
      spectrogram_(this),
      dft_power_(this),
      spectrogram_power_(this) {
  // TODO(gm): Find a cleaner way to do this
  if (zero_init) {
    // The first input buffer is to be considered as the "future" part
    // in the overlap.
    // Hence, the first 2 parts ("past" and "present") have to be filled in order
    // for the internal buffer writing cursor to be at the right position
    // TODO(gm): Find a better way using an "overlap" parameter to do this
    ringbuf_.Fill(0.0f,
                  parameters.window_length * (parameters.overlap - 1)
                  / parameters.overlap);
  }
  enabled_descriptors_.fill(false);
  computed_descriptors_.fill(false);
}

Manager::~Manager() {
  // Nothing to do here for now
}

void Manager::ProcessFrame(const float* const frame,
                           const std::size_t frame_length) {
  CHARTREUSE_ASSERT(frame != nullptr);
  CHARTREUSE_ASSERT(frame_length > 0);

  // Invalidate all computation from the previous frame
  for (unsigned int descriptor_idx(0);
       descriptor_idx < DescriptorId::kCount;
       ++descriptor_idx) {
    DescriptorIsComputed(static_cast<DescriptorId::Type>(descriptor_idx),
                         false);
  }
  // Push the frame into internal scratch memory
  std::copy_n(frame, frame_length, current_frame_.begin());
  // Push into ringbuffer for overlap
  ringbuf_.Push(frame, frame_length);
  // Pop - zero-padding done in the ringbuffer method
  ringbuf_.PopOverlapped(&current_window_[0],
                          parameters_.dft_length,
                          parameters_.overlap);
  std::size_t descriptors(0);
  DescriptorId::Type current_id(DescriptorId::kAudioPower);
  for (const bool enabled_descriptor : enabled_descriptors_) {
    if(enabled_descriptor) {
      GetDescriptor(current_id, frame, frame_length);
      descriptors += 1;
    }  // for (const bool enabled_descriptor : enabled_descriptors_)
    current_id = static_cast<DescriptorId::Type>(++current_id);
  }
}

void Manager::EnableDescriptor(const DescriptorId::Type descriptor,
                               const bool enable) {
  CHARTREUSE_ASSERT(descriptor != DescriptorId::kCount);
  enabled_descriptors_[descriptor] = enable;
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
      case DescriptorId::kAutoCorrelation: {
          instance = &autocorrelation_;
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

std::size_t Manager::GetDescriptorCopy(const DescriptorId::Type descriptor,
                                       const float* const frame,
                                       const std::size_t frame_length,
                                       float* const data) {
  const float* internal_data_ptr(GetDescriptor(descriptor, frame, frame_length));
  const std::size_t desc_dim(GetDescriptorMeta(descriptor).out_dim);
  std::copy(internal_data_ptr, internal_data_ptr + desc_dim, data);
  return desc_dim;
}

descriptors::Descriptor_Meta Manager::GetDescriptorMeta(
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
    case DescriptorId::kAutoCorrelation: {
        instance = &autocorrelation_;
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
  return instance->Meta();
}

std::size_t Manager::DescriptorsOutputSize(void) const {
  std::size_t out(0);
  DescriptorId::Type current_id(DescriptorId::kAudioPower);
  for (const bool enabled_descriptor : enabled_descriptors_) {
    if(enabled_descriptor) {
      out += GetDescriptorMeta(current_id).out_dim;
    }  // for (const bool enabled_descriptor : enabled_descriptors_)
    current_id = static_cast<DescriptorId::Type>(++current_id);
  }
  return out;
}

const Manager::Parameters& Manager::AnalysisParameters(void) const {
  return parameters_;
}

const float* Manager::CurrentFrame(void) const {
  return &current_frame_[0];
}

const float* Manager::CurrentWindow(void) const {
  return &current_window_[0];
}

bool Manager::IsDescriptorComputed(const DescriptorId::Type descriptor) const {
  return computed_descriptors_[static_cast<int>(descriptor)];
}

void Manager::DescriptorIsComputed(const DescriptorId::Type descriptor,
                                   const bool is_computed) {
  computed_descriptors_[static_cast<int>(descriptor)] = is_computed;
}

const float* Manager::DescriptorDataPtr(const DescriptorId::Type descriptor) const {
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
      case DescriptorId::kAutoCorrelation: {
          instance = &autocorrelation_;
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
