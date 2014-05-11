/// @filename debug.h
/// @brief Tests common debug include file
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

#ifndef CHARTREUSE_TESTS_DEBUG_H_
#define CHARTREUSE_TESTS_DEBUG_H_

#include <fstream>
#include <string>

#include "chartreuse/tests/tests.h"

/// @brief Namespace holding debug stuff - nothing using it should be committed
namespace debug {

/// @brief Helper class for writing signal data into files
///
/// This is clearly not optimized at all nor really clean,
/// for debugging purpose only!
class FileWriter {
 public:
  explicit FileWriter(const std::string& filename)
    : out_stream_(filename) {
    // Nothing to do here for now
  }
  ~FileWriter() {
    Close();
  }

  /// @brief Push version for one actual value
  void PushFloat(const float sample) {
    out_stream_ << sample << "\n";
  }

  /// @brief Push version for an entire buffer
  void PushBuffer(const float* buffer, const unsigned int buffer_size) {
    for (unsigned int i(0); i < buffer_size; ++i) {
      PushFloat(buffer[i]);
    }
  }

  /// @brief Close writer (automatically done when destroyed)
  void Close(void) {
    out_stream_.close();
  }

 private:
  std::ofstream out_stream_;
};

}  // namespace debug

#endif  // CHARTREUSE_TESTS_DEBUG_H_
