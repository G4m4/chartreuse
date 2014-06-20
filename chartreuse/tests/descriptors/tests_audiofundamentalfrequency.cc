/// @file tests_audiofundamentalfrequency.cc
/// @brief Chartreuse AudioFundamentalFrequency descriptor tests
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

#include "chartreuse/tests/tests.h"

#include "chartreuse/src/interface/manager.h"

// Useful using declarations
using chartreuse::interface::Manager;
using chartreuse::interface::DescriptorId::kAudioFundamentalFrequency;

/// @brief Compute the descriptor for a null signal,
/// check its output
TEST(AudioFundamentalFrequency, Null) {
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::fill(frame.begin(),
              frame.end(),
              0.0f);
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for an uniform white noise,
/// check that its range lies within [out_min ; out_max]
TEST(AudioFundamentalFrequency, WhiteNoise) {
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid,
/// check that its range lies within [out_min ; out_max]
TEST(AudioFundamentalFrequency, Sin) {
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);

  std::size_t index(0);
  const unsigned int kFrameLength(manager.AnalysisParameters().hop_size_sample);
  while (index < kDataTestSetSize - 1) {
    manager.ProcessFrame(&kInSin[index], kFrameLength);
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += kFrameLength;
  }
}

/// @brief Compute the descriptor for a pure sinusoid of very low frequency,
/// check the descriptor output
TEST(AudioFundamentalFrequency, LowFreq) {
  const float kFrequency(1.0f);
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);

  std::size_t index(0);
  SinusGenerator generator(kFrequency, kSamplingFreq);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  generator);
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid of very high frequency,
/// check the descriptor output
TEST(AudioFundamentalFrequency, HighFreq) {
  const float kFrequency((kSamplingFreq - 10.f) / 2.0f);
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);

  std::size_t index(0);
  SinusGenerator generator(kFrequency, kSamplingFreq);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  generator);
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a constant value,
/// check that its range lies within [out_min ; out_max]
TEST(AudioFundamentalFrequency, Constant) {
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);
  const float kConstant(1.0f);

  std::size_t index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::fill(frame.begin(),
              frame.end(),
              kConstant);
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    for (unsigned int desc_index(0);
         desc_index < manager.GetDescriptorMeta(descriptor).out_dim;
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, out_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Performance test for computing a fixed length signal
TEST(AudioFundamentalFrequency, Perf) {
  Manager manager((Manager::Parameters(kSamplingFreq)));
  chartreuse::interface::DescriptorId::Type descriptor(kAudioFundamentalFrequency);

  std::size_t index(0);
  // Computing the mean output prevents the compiler from optimizing out things
  float mean(0.0f);
  while (index < kDataPerfSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.ProcessFrame(&frame[0], frame.size());
    const float* out_data(manager.GetDescriptor(descriptor));
    mean += out_data[0] * out_data[0];
    index += frame.size();
  }
  EXPECT_LE(-1.0f, mean);
}
