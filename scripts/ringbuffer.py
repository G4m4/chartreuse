#!/usr/bin/env python
'''
@file ringbuffer.py
@brief A simple ringbuffer implementation
@author gm
@copyright gm 2014

This file is part of Chartreuse

Chartreuse is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Chartreuse is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Chartreuse.  If not, see <http://www.gnu.org/licenses/>.
'''
import numpy
import pylab

class RingBuffer(object):
    '''
    A simple non-resizable 1D ringbuffer
    '''
    def __init__(self, capacity, overlap):
        self._data = numpy.zeros(capacity)
        self.capacity = capacity
        self.size = 0
        self._read_cursor = 0
        self._write_cursor = 0
        self._overlap = overlap

    def IsFull(self):
        return self.size == self.capacity

    def IsEmpty(self):
        return self.size == 0

    def Push(self, data):
        data_length = len(data)
        if data_length > self.capacity:
            raise Exception("Too much input data!")

        # TODO(gm): check out numpy.roll
        right_part_length = min(data_length, self.capacity -  self._write_cursor)
        left_part_length = data_length - right_part_length
        self._data[self._write_cursor:self._write_cursor + right_part_length] = data[0:right_part_length]
        self._data[0:left_part_length] = data[right_part_length:right_part_length + left_part_length]

        self._write_cursor += data_length
        self._write_cursor = self._write_cursor % self.capacity
        self.size += data_length

    def Fill(self, value, data_length):
        return self.Push(value * numpy.zeros(data_length))

    def Pop(self, data_length):
        # Zero-padding may occur!
        actual_copy_count = min(self.size, data_length)

        out = numpy.zeros(data_length)
        # TODO(gm): check out numpy.roll
        right_part_length = min(actual_copy_count, self.capacity -  self._read_cursor)
        left_part_length = actual_copy_count - right_part_length
        out[0:right_part_length] = self._data[self._read_cursor:self._read_cursor + right_part_length]
        out[right_part_length:right_part_length + left_part_length] = self._data[0:left_part_length]

        self._read_cursor += data_length
        self._read_cursor = self._read_cursor % self.capacity
        self.size -= data_length

        # Zero-padding
        if actual_copy_count < data_length:
            out[actual_copy_count:data_length] = numpy.zeros(data_length - actual_copy_count)

        return out

    def PopOverlapped(self, data_length):
        # Zero-padding may occur!
        actual_copy_count = min(self.size, data_length)

        out = numpy.zeros(data_length)
        # TODO(gm): check out numpy.roll
        right_part_length = min(actual_copy_count, self.capacity -  self._read_cursor)
        left_part_length = actual_copy_count - right_part_length
        out[0:right_part_length] = self._data[self._read_cursor:self._read_cursor + right_part_length]
        out[right_part_length:right_part_length + left_part_length] = self._data[0:left_part_length]

        self._read_cursor += data_length / self._overlap
        self._read_cursor = self._read_cursor % self.capacity
        self.size -= data_length / self._overlap

        # Zero-padding
        if actual_copy_count < data_length:
            out[actual_copy_count:data_length] = numpy.zeros(data_length - actual_copy_count)

        return out

if __name__ == "__main__":
    import utilities

    frame_length = 480
    length = 9600
    overlap = 3

    out_data = numpy.zeros(length)

    in_data = numpy.random.rand(length)

    ringbuffer = RingBuffer(length, overlap)

    current_in_idx = 0
    while not ringbuffer.IsFull():
        ringbuffer.Push(in_data[current_in_idx * frame_length: (current_in_idx + 1) * frame_length])
        current_in_idx += 1

    current_out_idx = 0
    while not ringbuffer.IsEmpty():
        out_data[current_out_idx * frame_length: (current_out_idx + 1) * frame_length] = ringbuffer.Pop(frame_length)
        current_out_idx += 1

    print(utilities.PrintMetadata(utilities.GetMetadata(out_data[0:length] - in_data)))

    pylab.plot(in_data[0:length], label = "in")
    pylab.plot(out_data[0:length], label = "out")

    pylab.legend()
    pylab.show()
