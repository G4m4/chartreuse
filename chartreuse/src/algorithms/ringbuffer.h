/// @filename ringbuffer.h
/// @brief Implementation of a simple resizable ringbuffer
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

#ifndef CHARTREUSE_SRC_ALGORITHMS_RINGBUFFER_H_
#define CHARTREUSE_SRC_ALGORITHMS_RINGBUFFER_H_

namespace chartreuse {
namespace algorithms {

/// @brief Simple implementation of a circular buffer
///
/// Resizable, FIFO-type container; its general philosophy is that,
/// if one operation could not be done (pushing too much data, etc.)
/// it asserts - there are no return values nor exceptions.
class RingBuffer {
 public:
  /// @brief Default constructor: the user has to provide a fixed buffer length
  explicit RingBuffer(const unsigned int capacity);
  ~RingBuffer();

  /// @brief Pop elements out of the buffer
  ///
  /// Output may be zero-padded if more elements are poped than those available
  ///
  /// @param[out]   dest          Buffer to store the elements into
  /// @param[in]    count         Elements count to retrieve
  void Pop(float* dest, const unsigned int count);

  /// @brief Push elements into the buffer
  ///
  /// @param[in]  src   Buffer to push
  /// @param[in]  count   Buffer elements count
  void Push(const float* const src, const unsigned int count);

  /// @brief Explicitly clear buffer content but does not deallocate it
  void Clear(void);

  /// @brief Returns true if the buffer is "usable"
  ///
  /// For now, this means that some memory is allocated
  bool IsGood(void) const;

  /// @brief How many elements may be pushed into the buffer
  unsigned int Capacity(void) const;

  /// @brief How many elements may be popped from the buffer
  unsigned int Size(void) const;

 private:
  float* data_;  ///< Internal elements buffer
  unsigned int capacity_;  ///< Internal buffer length
  unsigned int size_;  ///< Count of elements currently held within the buffer
  unsigned int writing_position_;  ///< Beginning of the writing part
  unsigned int reading_position_;  ///< Beginning of the reading part
};

}  // namespace algorithms
}  // namespace chartreuse

#endif  // CHARTREUSE_SRC_ALGORITHMS_RINGBUFFER_H_
