/// @file tests_apodizer.cc
/// @brief Chartreuse apodizer wrapper tests
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

#include "chartreuse/src/algorithms/apodizer.h"
#include "chartreuse/src/interface/manager.h"

// Using declarations for tested class
using chartreuse::algorithms::Apodizer;
// Useful using declarations
using chartreuse::algorithms::Window::kRectangular;
using chartreuse::algorithms::Window::kHamming;
using chartreuse::interface::Manager;

/// @brief Check that the window max is 1, and is at the middle
TEST(Apodizer, RectangularUnitary) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const float kEpsilon(1e-5f);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  Apodizer apodizer(kWindowLength, kRectangular);
  std::vector<float> data(kWindowLength, 1.0f);

  apodizer.ApplyWindow(&data[0]);
  EXPECT_NEAR(1.0f, data[data.size() / 2], kEpsilon);
}

/// @brief Check that the window max is 1, and is at the middle
TEST(Apodizer, HammingUnitary) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const float kEpsilon(1e-5f);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  Apodizer apodizer(kWindowLength, kHamming);
  std::vector<float> data(kWindowLength, 1.0f);

  apodizer.ApplyWindow(&data[0]);
  EXPECT_NEAR(1.0f, data[data.size() / 2], kEpsilon);
}

/// @brief Check window data range
TEST(Apodizer, RectangularRange) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  Apodizer apodizer(kWindowLength, kRectangular);
  std::vector<float> data(kWindowLength, 1.0f);

  apodizer.ApplyWindow(&data[0]);
  for (std::vector<float>::const_iterator iter(data.begin());
       iter != data.end();
       ++iter) {
    EXPECT_GE(1.0f, *iter);
    EXPECT_LE(0.0f, *iter);
  }
}

/// @brief Check window data range
TEST(Apodizer, HammingRange) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  Apodizer apodizer(kWindowLength, kHamming);
  std::vector<float> data(kWindowLength, 1.0f);

  apodizer.ApplyWindow(&data[0]);
  for (std::vector<float>::const_iterator iter(data.begin());
       iter != data.end();
       ++iter) {
    EXPECT_GE(1.0f, *iter);
    EXPECT_LE(0.0f, *iter);
  }
}

/// @brief Evaluate window synthesis performance
TEST(Apodizer, RectangularSynthesisPerf) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(kDataPerfSetSize);
  Apodizer apodizer(kWindowLength, kRectangular);
  std::vector<float> data(kWindowLength, 1.0f);

  apodizer.ApplyWindow(&data[0]);
  for (std::vector<float>::const_iterator iter(data.begin());
       iter != data.end();
       ++iter) {
    EXPECT_GE(1.0f, *iter);
    EXPECT_LE(0.0f, *iter);
  }
}

/// @brief Evaluate window synthesis performance
TEST(Apodizer, HammingSynthesisPerf) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(kDataPerfSetSize);
  Apodizer apodizer(kWindowLength, kHamming);
  std::vector<float> data(kWindowLength, 1.0f);

  apodizer.ApplyWindow(&data[0]);
  for (std::vector<float>::const_iterator iter(data.begin());
       iter != data.end();
       ++iter) {
    EXPECT_GE(1.0f, *iter);
    EXPECT_LE(0.0f, *iter);
  }
}

/// @brief Evaluate window application performance
TEST(Apodizer, RectangularApplyPerf) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  Apodizer apodizer(kWindowLength, kRectangular);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::vector<float> frame(kWindowLength, 1.0f);
    apodizer.ApplyWindow(&frame[0]);
    for (std::vector<float>::const_iterator iter(frame.begin());
         iter != frame.end();
         ++iter) {
      EXPECT_GE(1.0f, *iter);
      EXPECT_LE(-1.0f, *iter);
    }
    index += frame.size();
  }
}

/// @brief Evaluate window application performance
TEST(Apodizer, HammingApplyPerf) {
  Manager::Parameters parameters;
  Manager manager(parameters);
  const unsigned int kWindowLength(manager.AnalysisParameters().window_length);
  Apodizer apodizer(kWindowLength, kHamming);

  std::size_t index(0);
  while (index < kDataTestSetSize) {
    std::vector<float> frame(kWindowLength, 1.0f);
    apodizer.ApplyWindow(&frame[0]);
    for (std::vector<float>::const_iterator iter(frame.begin());
         iter != frame.end();
         ++iter) {
      EXPECT_GE(1.0f, *iter);
      EXPECT_LE(-1.0f, *iter);
    }
    index += frame.size();
  }
}
