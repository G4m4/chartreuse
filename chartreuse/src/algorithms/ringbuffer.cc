/// @filename ringbuffer.cc
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

// std::fill, std::copy, std::min, std::max
#include <algorithm>

#include "chartreuse/src/algorithms/ringbuffer.h"
#include "chartreuse/src/common.h"

namespace chartreuse {
namespace algorithms {

RingBuffer::RingBuffer(const std::size_t capacity)
    : data_(nullptr),
      capacity_(capacity),
      size_(0),
      writing_position_(0),
      reading_position_(0) {
  CHARTREUSE_ASSERT(capacity > 0);
  data_ = static_cast<float*>(new float[capacity_]);
  std::fill_n(&data_[0], capacity_, 0.0f);
}

RingBuffer::~RingBuffer() {
  delete[] data_;
  data_ = nullptr;
}

void RingBuffer::PopOverlapped(float* dest,
                               const std::size_t count,
                               const unsigned int overlap) {
  CHARTREUSE_ASSERT(IsGood());
  CHARTREUSE_ASSERT(overlap > 0);

  // Is zero padding required ?
  // Using "ints" here cause count may be < size, or the opposite
  const std::size_t zeropadding_count(static_cast<std::size_t>(
    (std::max(static_cast<int>(count - size_), 0))));
  // Actual elements count to be copied
  const std::size_t copy_count(count - zeropadding_count);

  // Length of the "right" part: from reading cursor to the buffer end
  const std::size_t right_part_size(std::min(capacity_ - reading_position_,
                                     copy_count));
  // Length of the "left" part: from the buffer beginning
  // to the last element to be copied
  const std::size_t left_part_size(copy_count - right_part_size);


  //  Copy the first part
  std::copy_n(&data_[reading_position_], right_part_size, &dest[0]);
  //  Copy the second part
  std::copy_n(&data_[0], left_part_size, &dest[right_part_size]);

  reading_position_ += copy_count / overlap;
  reading_position_ = reading_position_ % capacity_;

  size_ -= copy_count / overlap;

  // Zero-padding
  std::fill_n(&dest[copy_count],
              zeropadding_count,
              0.0f);
}

void RingBuffer::Pop(float* dest, const std::size_t count) {
  return PopOverlapped(dest, count, 1);
}

void RingBuffer::Push(const float* const src, const std::size_t count) {
  CHARTREUSE_ASSERT(IsGood());
  CHARTREUSE_ASSERT(count <= Capacity() - Size());
  // Length of the "right" part: from writing cursor to the buffer end
  const std::size_t right_part_size(std::min(capacity_ - writing_position_,
                                              count));
  // Length of the "left" part: from the buffer beginning
  // to the last element to be pushed
  const std::size_t left_part_size(count - right_part_size);

  //  Copy the first part
  std::copy_n(&src[0], right_part_size, &data_[writing_position_]);
  //  Copy the second part
  std::copy_n(&src[right_part_size], left_part_size, &data_[0]);

  writing_position_ += count;
  writing_position_ = writing_position_ % capacity_;

  size_ += count;
}

void RingBuffer::Fill(const float value, const std::size_t count) {
  CHARTREUSE_ASSERT(IsGood());
  CHARTREUSE_ASSERT(count > 0);
  CHARTREUSE_ASSERT(count <= Capacity() - Size());
  // Length of the "right" part: from writing cursor to the buffer end
  const std::size_t right_part_size(std::min(capacity_ - writing_position_,
                                              count));
  // Length of the "left" part: from the buffer beginning
  // to the last element to be pushed
  const std::size_t left_part_size(count - right_part_size);

  // Filling the first part
  std::fill_n(&data_[writing_position_], right_part_size, value);
  // Fill the second part
  std::fill_n(&data_[0], left_part_size, value);

  writing_position_ += count;
  writing_position_ = writing_position_ % capacity_;

  size_ += count;
}

void RingBuffer::Clear(void) {
  writing_position_ = 0;
  reading_position_ = 0;
  size_ = 0;
  if (IsGood()) {
    std::fill_n(&data_[0], Capacity(), 0.0f);
  }
}

bool RingBuffer::IsGood(void) const {
  return data_ != nullptr;
}

std::size_t RingBuffer::Capacity(void) const {
  CHARTREUSE_ASSERT(IsGood());
  return capacity_;
}

std::size_t RingBuffer::Size(void) const {
  CHARTREUSE_ASSERT(IsGood());
  return size_;
}

}  // namespace algorithms
}  // namespace chartreuse
