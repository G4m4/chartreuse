/// @file tests_analyzer.cc
/// @brief Chartreuse analyzer class tests
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

#include "chartreuse/src/interface/analyzer.h"

// Using declarations for tested class
using chartreuse::interface::Analyzer;
// Using declarations for related classes
using chartreuse::interface::kAvailableDescriptors;

/// @brief Feed the analyzer with white noise, check output range
TEST(Analyzer, WhiteNoiseRange) {
  const float kSamplingFreq(48000.0f);
  const unsigned int kFrameLength(chartreuse::kHopSizeSamples * 3);
  const unsigned int kExpectedSubframesCount(kFrameLength
                                             / chartreuse::kHopSizeSamples);

  Analyzer analyzer(kSamplingFreq);
  const unsigned int kMaxIteration(1);

  std::vector<float> out_data(kAvailableDescriptors.size()
                              * kExpectedSubframesCount);

  std::size_t index(0);
  while (index < kMaxIteration) {
    std::vector<float> frame(kFrameLength);
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    analyzer.Process(&frame[0],
                     frame.size(),
                     &out_data[0]);
    for (unsigned int subframe_idx(0);
         subframe_idx < kExpectedSubframesCount;
         ++subframe_idx) {
      for (unsigned int descriptor_idx(0);
           descriptor_idx < kAvailableDescriptors.size();
           ++descriptor_idx) {
        const unsigned int kActualIdx(subframe_idx * kAvailableDescriptors.size()
                                      + descriptor_idx);
        const float kValue(out_data[kActualIdx]);
        EXPECT_GE(1.0f, kValue);
        EXPECT_LE(0.0f, kValue);
      }
    }  // subframe_idx
    index += 1;
  }
}

/// @brief Feed the analyzer with white noise,
/// check consistency across subframes
TEST(Analyzer, WhiteNoiseSubframeConsistency) {
  const float kSamplingFreq(48000.0f);
  const unsigned int kFrameLength(chartreuse::kHopSizeSamples * 3);
  const unsigned int kExpectedSubframesCount(kFrameLength
                                             / chartreuse::kHopSizeSamples);
  const float kEpsilon(1e-3f);
  const unsigned int kMaxIteration(2);

  Analyzer analyzer(kSamplingFreq);

  std::vector<float> out_data(kAvailableDescriptors.size()
                              * kExpectedSubframesCount);

  std::size_t index(0);
  while (index < kMaxIteration) {
    std::vector<float> frame(kFrameLength);
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    analyzer.Process(&frame[0],
                     frame.size(),
                     &out_data[0]);
    // Expecting each subframe to be very close to the second one
    // The first one is ignored due to the "empty first buffer" effect
    for (unsigned int subframe_idx(1);
         subframe_idx < kExpectedSubframesCount;
         ++subframe_idx) {
      for (unsigned int descriptor_idx(0);
        descriptor_idx < kAvailableDescriptors.size();
        ++descriptor_idx) {
        const unsigned int kActualIdx(subframe_idx * kAvailableDescriptors.size()
          + descriptor_idx);
        const float kValue(out_data[kActualIdx]);
        EXPECT_NEAR(out_data[descriptor_idx + kAvailableDescriptors.size()],
                    kValue,
                    kEpsilon);
      }
    }  // subframe_idx
    index += 1;
  }
}

/// @brief Feed the analyzer with a perfect sinusoid,
/// check consistency across subframes
TEST(Analyzer, SinSubframeConsistency) {
  const float kSamplingFreq(48000.0f);
  const unsigned int kFrameLength(chartreuse::kHopSizeSamples * 3);
  const unsigned int kExpectedSubframesCount(kFrameLength
                                             / chartreuse::kHopSizeSamples);
  const float kFrequency(445.0f);
  const float kEpsilon(7e-2f);
  const unsigned int kMaxIteration(2);

  SinusGenerator generator(kFrequency, kSamplingFreq);
  Analyzer analyzer(kSamplingFreq);

  std::vector<float> out_data(kAvailableDescriptors.size()
                              * kExpectedSubframesCount);

  std::size_t index(0);
  while (index < kMaxIteration) {
    std::vector<float> frame(kFrameLength);
    // Fill the frame with sin data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return generator();});
    analyzer.Process(&frame[0],
                     kFrameLength,
                     &out_data[0]);
    // Expecting each subframe to be very close to the second one
    // The first one is ignored due to the "empty first buffer" effect
    for (unsigned int subframe_idx(1);
         subframe_idx < kExpectedSubframesCount;
         ++subframe_idx) {
      for (unsigned int descriptor_idx(0);
        descriptor_idx < kAvailableDescriptors.size();
        ++descriptor_idx) {
        const unsigned int kActualIdx(subframe_idx * kAvailableDescriptors.size()
          + descriptor_idx);
        const float kValue(out_data[kActualIdx]);
        EXPECT_NEAR(out_data[kAvailableDescriptors.size() + descriptor_idx],
                    kValue,
                    kEpsilon);
      }
    }  // subframe_idx
    index += 1;
  }
}

/// @brief Feed the analyzer with a perfect low frequency sinusoid,
/// check consistency across frames
///
/// This is a convoluted way to check that the analyzer internal buffer
/// makes a good job of handling remaining data when the  input block size
/// is not a mutliple of the internal one.
TEST(Analyzer, SinInterFrameConsistency) {
  const float kSamplingFreq(48000.0f);
  const unsigned int kFrameLength(chartreuse::kHopSizeSamples * 7 / 2);
  const unsigned int kMaxSubframesCount(
    static_cast<unsigned int>(std::ceil(static_cast<float>(kFrameLength)
                                          / chartreuse::kHopSizeSamples)));
  const float kFrequency(75.0f);
  const float kEpsilon(7e-2f);
  const unsigned int kMaxIteration(4);

  SinusGenerator generator(kFrequency, kSamplingFreq);
  Analyzer analyzer(kSamplingFreq);

  std::vector<float> out_data(kAvailableDescriptors.size()
                              * kMaxSubframesCount);

  std::vector<float> ref_data(kAvailableDescriptors.size());

  std::vector<float> frame(kFrameLength);
  std::generate(frame.begin(),
                frame.end(),
                [&] {return generator();});
  analyzer.Process(&frame[0],
                    kFrameLength,
                    &out_data[0]);
  // The second subframe of the first frame is taken as a reference
  std::copy_n(&out_data[kAvailableDescriptors.size()],
              kAvailableDescriptors.size(),
              ref_data.begin());

  std::size_t index(1);
  while (index < kMaxIteration) {
    std::vector<float> frame(kFrameLength);
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return generator();});
    const unsigned int kActualSubframesCount(analyzer.Process(&frame[0],
                                                              kFrameLength,
                                                              &out_data[0]));
    // Expecting each subframe to be very close to the second one
    // The first one is ignored due to the "empty first buffer" effect
    for (unsigned int subframe_idx(0);
         subframe_idx < kActualSubframesCount;
         ++subframe_idx) {
      for (unsigned int descriptor_idx(0);
        descriptor_idx < kAvailableDescriptors.size();
        ++descriptor_idx) {
        const unsigned int kActualIdx(subframe_idx * kAvailableDescriptors.size()
          + descriptor_idx);
        const float kValue(out_data[kActualIdx]);
        EXPECT_NEAR(ref_data[descriptor_idx],
                    kValue,
                    kEpsilon);
      }
    }  // subframe_idx
    index += 1;
  }
}
