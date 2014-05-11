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
namespace synthesizer {

RingBuffer::RingBuffer(const unsigned int capacity)
    : data_(nullptr),
      capacity_(capacity),
      size_(0),
      writing_position_(0),
      reading_position_(0) {
  CHARTREUSE_ASSERT(capacity > 0);
  data_ = static_cast<float*>(new float[capacity_]);
  std::fill(&data_[0],
            &data_[capacity_],
            0.0f);
}

RingBuffer::~RingBuffer() {
  delete[] data_;
  data_ = nullptr;
}

void RingBuffer::Pop(float* dest, const unsigned int count) {
  CHARTREUSE_ASSERT(IsGood());
  CHARTREUSE_ASSERT(count > 0);

  // Is zero padding required ?
  // Using "ints" here cause count may be < size, or the opposite
  const unsigned int zeropadding_count(static_cast<unsigned int>(
    (std::max(static_cast<int>(count - size_), 0))));
  // Actual elements count to be copied
  const unsigned int copy_count(count - zeropadding_count);
  // If data needs to be copied...(e.g. ringbuffer size > 0)
  if (copy_count > 0) {
    // Length of the "right" part: from reading cursor to the buffer end
    const unsigned int right_part_size(std::min(capacity_ - reading_position_,
                                       copy_count));
    // Length of the "left" part: from the buffer beginning
    // to the last element to be copied
    const unsigned int left_part_size(copy_count - right_part_size);

    //  Copying the first part
    std::copy(&data_[reading_position_],
              &data_[reading_position_ + right_part_size],
              &dest[0]);
    if (0 != left_part_size) {
      //  copy the second part (if there is one)
      std::copy(&data_[0],
                &data_[left_part_size],
                &dest[right_part_size]);
    }

    reading_position_ += copy_count;
    reading_position_ = reading_position_ % capacity_;

    size_ -= copy_count;
  }  // If data needs to be copied...
  // Zero-padding
  if (zeropadding_count) {
    std::fill(&dest[copy_count],
              &dest[copy_count + zeropadding_count],
              0.0f);
  }
}

void RingBuffer::Push(const float* const src, const unsigned int count) {
  CHARTREUSE_ASSERT(IsGood());
  CHARTREUSE_ASSERT(count > 0);
  CHARTREUSE_ASSERT(count <= Capacity() - Size());
  // Length of the "right" part: from writing cursor to the buffer end
  const unsigned int right_part_size(std::min(capacity_ - writing_position_,
                                              count));
  // Length of the "left" part: from the buffer beginning
  // to the last element to be pushed
  const unsigned int left_part_size(count - right_part_size);

  //  Copying the first part
  std::copy(&src[0],
            &src[right_part_size],
            &data_[writing_position_]);
  if (0 != left_part_size) {
    //  copy the second part (if there is one)
    std::copy(&src[right_part_size],
              &src[right_part_size + left_part_size],
              &data_[0]);
  }

  writing_position_ += count;
  writing_position_ = writing_position_ % capacity_;

  size_ += count;
}

void RingBuffer::Clear(void) {
  writing_position_ = 0;
  reading_position_ = 0;
  size_ = 0;
  if (IsGood()) {
    std::fill(&data_[0],
              &data_[Capacity()],
              0.0f);
  }
}

bool RingBuffer::IsGood(void) const {
  return data_ != nullptr;
}

unsigned int RingBuffer::Capacity(void) const {
  CHARTREUSE_ASSERT(IsGood());
  return capacity_;
}

unsigned int RingBuffer::Size(void) const {
  CHARTREUSE_ASSERT(IsGood());
  return size_;
}

}  // namespace synthesizer
}  // namespace chartreuse
