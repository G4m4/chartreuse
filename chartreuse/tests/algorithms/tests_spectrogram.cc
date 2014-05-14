/// @file tests_spectrogram.cc
/// @brief Chartreuse spectrogram algorithm tests
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

#include "chartreuse/src/algorithms/spectrogram.h"

// Using declarations for tested class
using chartreuse::algorithms::Spectrogram;

/// @brief Compute the spectrogram for white noise
TEST(Spectrogram, WhiteNoise) {
  const unsigned int kWindowLength(1440);
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  std::vector<float> out_data(kDftLength * 2);

  Spectrogram spectrogram(kWindowLength, kDftLength, kSamplingFreq);

  unsigned int index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with random data
    std::generate(frame.begin(),
                  frame.end(),
                  [&] {return kNormDistribution(kRandomGenerator);});
    spectrogram(&frame[0],
                &out_data[0]);
    index += frame.size();
  }
}

/// @brief Compute the spectrogram for a sinus
TEST(Spectrogram, Sin) {
  const unsigned int kWindowLength(1440);
  const unsigned int kDftLength(2048);
  const float kSamplingFreq(48000.0f);
  std::vector<float> out_data(kDftLength * 2);

  Spectrogram spectrogram(kWindowLength, kDftLength, kSamplingFreq);

  unsigned int index(0);
  while (index < kDataTestSetSize) {
    std::array<float, chartreuse::kHopSizeSamples> frame;
    // Fill the frame with sin data
    const std::size_t kRightIndex(
      std::min(index + frame.size(),
      static_cast<std::size_t>(kDataInSinLength - 1)));
    std::copy(&kInSin[index],
              &kInSin[kRightIndex],
              frame.begin());
    spectrogram(&frame[0],
                &out_data[0]);
    index += frame.size();
  }
}
