/// @file tests.cc
/// @brief Tests common source file
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

SinusGenerator::SinusGenerator(const float freq, const float sampling_freq)
    : increment_(2.0 * 3.1415926535f * freq / sampling_freq),
      current_phase_(0.0) {
  CHARTREUSE_ASSERT(freq > 0.0f);
  CHARTREUSE_ASSERT(sampling_freq > 0.0f);
  CHARTREUSE_ASSERT(freq <= sampling_freq / 2.0f);
}

SinusGenerator::~SinusGenerator() {
  // Nothing to do here for now
}

float SinusGenerator::operator()(void) {
  const float out(static_cast<float>(std::cos(current_phase_)));
  current_phase_ += increment_;
  return out;
}
