/// @file tests_autocorrelation.cc
/// @brief Chartreuse autocorrelation algorithm tests
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

#include "chartreuse/src/algorithms/autocorrelation.h"
#include "chartreuse/src/manager/manager.h"

// Using declarations for tested class
using chartreuse::algorithms::AutoCorrelation;
// Useful using declarations
using chartreuse::manager::Manager;

/// @brief Check output range for white noise
TEST(AutoCorrelation, WhiteNoise) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  AutoCorrelation analyzer(&manager);
  std::vector<float> out_data(analyzer.Meta().out_dim);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::vector<float> frame(manager.AnalysisParameters().window_length);
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    analyzer(&frame[0], frame.size(), &out_data[0]);
    for (unsigned int desc_index(0);
         desc_index < out_data.size();
         ++desc_index) {
      EXPECT_GE(analyzer.Meta().out_max, out_data[desc_index]);
      EXPECT_LE(analyzer.Meta().out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}

/// @brief Check performance
TEST(AutoCorrelation, Perf) {
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);

  Manager manager(Manager::Parameters(kSamplingFreq, kDftLength));
  AutoCorrelation analyzer(&manager);
  std::vector<float> out_data(analyzer.Meta().out_dim);

  std::size_t index(0);
  while (index < kDataPerfSetSize / 8) {
    std::vector<float> frame(manager.AnalysisParameters().window_length);
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    analyzer(&frame[0], frame.size(), &out_data[0]);
    for (unsigned int desc_index(0);
         desc_index < out_data.size();
         ++desc_index) {
      EXPECT_GE(analyzer.Meta().out_max, out_data[desc_index]);
      EXPECT_LE(analyzer.Meta().out_min, out_data[desc_index]);
    }
    index += frame.size();
  }
}
