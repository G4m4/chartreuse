/// @file tests_audiopower.cc
/// @brief Chartreuse AudioPower descriptor tests
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

#include "chartreuse/src/manager/manager.h"

// Useful using declarations
using chartreuse::manager::Manager;
using chartreuse::manager::DescriptorId::kAudioPower;

/// @brief Compute the descriptor for a null signal,
/// check its output
TEST(AudioPower, Null) {
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::fill(frame.begin(),
              frame.end(),
              0.0f);
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, desc_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for an uniform white noise,
/// check that its range lies within [out_min ; out_max]
TEST(AudioPower, WhiteNoise) {
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, desc_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid,
/// check that its range lies within [out_min ; out_max]
TEST(AudioPower, Sin) {
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);

  std::size_t index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    const std::size_t kRightIndex(
      std::min(index + frame.size(),
      static_cast<std::size_t>(kDataInSinLength - 1)));
    std::copy(&kInSin[index],
              &kInSin[kRightIndex],
              frame.begin());
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, desc_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid of very low frequency,
/// check the descriptor output
TEST(AudioPower, LowFreq) {
  const float kFrequency(1.0f);
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);

  std::size_t index(0);
  SinusGenerator generator(kFrequency, kSamplingFreq);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  generator);
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, desc_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a pure sinusoid of very high frequency,
/// check the descriptor output
TEST(AudioPower, Highfreq) {
  const float kFrequency((kSamplingFreq - 10.f) / 2.0f);
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);

  std::size_t index(0);
  SinusGenerator generator(kFrequency, kSamplingFreq);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  generator);
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, desc_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Compute the descriptor for a constant value,
/// check that its range lies within [out_min ; out_max]
TEST(AudioPower, Constant) {
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);
  const float kConstant(manager.GetDescriptorMeta(descriptor).out_max);

  std::size_t index(0);
  while (index < kDataTestSetSize - 1) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    std::fill(frame.begin(),
              frame.end(),
              kConstant);
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    for (unsigned int desc_index(0);
         desc_index < desc_data.size();
         ++desc_index) {
      EXPECT_GE(manager.GetDescriptorMeta(descriptor).out_max, desc_data[desc_index]);
      EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, desc_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Performance test for computing a fixed length signal
TEST(AudioPower, Perf) {
  Manager manager(kSamplingFreq);
  chartreuse::manager::DescriptorId::Type descriptor(kAudioPower);
  std::vector<float> desc_data(manager.GetDescriptorMeta(descriptor).out_dim);

  std::size_t index(0);
  // Computing the mean output prevents the compiler from optimizing out things
  float mean(0.0f);
  while (index < kDataPerfSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    manager.GetDescriptorCopy(descriptor, &frame[0], frame.size(), &desc_data[0]);
    mean += desc_data[0] * desc_data[0];
    index += frame.size();
  }
  EXPECT_LE(manager.GetDescriptorMeta(descriptor).out_min, mean);
}
