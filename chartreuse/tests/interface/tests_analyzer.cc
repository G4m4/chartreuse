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

/// @brief Feed the analyzer with white noise
TEST(Analyzer, WhiteNoise) {
  const float kSamplingFreq(48000.0f);
  const unsigned int kFrameLength(480);
  const unsigned int kExpectedSubframesCount(kFrameLength
                                             / chartreuse::kHopSizeSamples);

  Analyzer analyzer(kSamplingFreq);

  std::vector<float> out_data(kAvailableDescriptors.size()
                              * kExpectedSubframesCount);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
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
    index += frame.size();
  }
}
