/// @file tests_combedsignalgenerator.cc
/// @brief Chartreuse cobmed signal generator algorithm tests
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

#include "chartreuse/src/algorithms/combedsignalgenerator.h"

// Using declarations for tested class
using chartreuse::algorithms::CombedSignalGenerator;

// Required, the factor computation is far from perfect
const float kEpsilon(2e-1f);

/// @brief Check output range for white noise
TEST(CombedSignalGenerator, WhiteNoise) {
  const unsigned int kFrameLength(480);
  const unsigned int kWindowLength(kFrameLength * 3);
  const unsigned int kLag(480);
  CombedSignalGenerator generator(kWindowLength, kLag);
  std::vector<float> out_data(kFrameLength);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::vector<float> frame(kWindowLength);
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    generator(&frame[0], out_data.size(), &out_data[0]);
    for (unsigned int desc_index(0);
         desc_index < out_data.size();
         ++desc_index) {
      EXPECT_GE(1.0f, out_data[desc_index] - kEpsilon);
      EXPECT_LE(-1.0f, out_data[desc_index] + kEpsilon);
    }
    index += frame.size();
  }
}

/// @brief Performance test
TEST(CombedSignalGenerator, Perf) {
  const unsigned int kFrameLength(480);
  const unsigned int kWindowLength(kFrameLength * 3);
  const unsigned int kLag(480);
  CombedSignalGenerator generator(kWindowLength, kLag);
  std::vector<float> out_data(kFrameLength);

  std::size_t index(0);
  while (index < kDataPerfSetSize) {
    std::vector<float> frame(kWindowLength);
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    generator(&frame[0], out_data.size(), &out_data[0]);
    for (unsigned int desc_index(0);
         desc_index < out_data.size();
         ++desc_index) {
      EXPECT_GE(1.0f, out_data[desc_index] - kEpsilon);
      EXPECT_LE(-1.0f, out_data[desc_index] + kEpsilon);
    }
    index += frame.size();
  }
}
