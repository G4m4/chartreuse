/// @file tests_scalegenerator.cc
/// @brief Chartreuse scale generator tests
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

#include "chartreuse/src/algorithms/scalegenerator.h"
#include "chartreuse/src/manager/manager.h"

// Using declarations for tested class
using chartreuse::algorithms::ScaleGenerator;
// Useful using declarations
using chartreuse::algorithms::Scale::kLogFreq;
using chartreuse::manager::Manager;

/// @brief Check window data range
TEST(ScaleGenerator, LogFreqRange) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  ScaleGenerator generator(kWindowLength,
                           kLogFreq,
                           manager.AnalysisParameters().dft_length,
                           manager.AnalysisParameters().sampling_freq);

  const float* data(generator.Data());
  for (unsigned int i(0);
       i < kWindowLength;
       ++i) {
    EXPECT_GE(6.0f, data[i]);
    EXPECT_LE(-5.0f, data[i]);
  }
}

/// @brief Evaluate window synthesis performance
TEST(ScaleGenerator, HammingSynthesisPerf) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(kDataPerfSetSize);
  ScaleGenerator generator(kWindowLength,
                           kLogFreq,
                           manager.AnalysisParameters().dft_length,
                           manager.AnalysisParameters().sampling_freq);

  const float* data(generator.Data());
  for (unsigned int i(0);
       i < kWindowLength;
       ++i) {
    EXPECT_LE(-5.0f, data[i]);
  }
}
