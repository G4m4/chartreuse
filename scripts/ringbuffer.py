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
    def __init__(self, capacity):
        self._data = numpy.zeros(capacity)
        self.capacity = capacity
        self.size = 0
        self._read_cursor = 0
        self._write_cursor = 0

    def IsFull(self):
        return self.size == self.capacity

    def IsEmpty(self):
        return self.size == 0

    def Push(self, data):
        data_length = len(data)
        if data_length > self.capacity - self.size:
            raise Exception("Too much input data!")

        # TODO(gm): check out numpy.roll
        right_part_length = min(data_length, self.capacity -  self._write_cursor)
        left_part_length = data_length - right_part_length
        self._data[self._write_cursor:self._write_cursor + right_part_length] = data[0:right_part_length]
        self._data[0:left_part_length] = data[right_part_length:right_part_length + left_part_length]

        if left_part_length == 0:
            self._write_cursor += right_part_length
        else:
            self._write_cursor = left_part_length
        self.size += data_length

    def Fill(self, value, data_length):
        return self.Push(value * numpy.zeros(data_length))

    def Pop(self, data_length):
        if data_length > self.size:
            raise Exception("Too much required data!")

        out = numpy.zeros(data_length)
        # TODO(gm): check out numpy.roll
        right_part_length = min(data_length, self.capacity -  self._read_cursor)
        left_part_length = data_length - right_part_length
        out[0:right_part_length] = self._data[self._read_cursor:self._read_cursor + right_part_length]
        out[right_part_length:right_part_length + left_part_length] = self._data[0:left_part_length]

        if left_part_length == 0:
            self._read_cursor += right_part_length
        else:
            self._read_cursor = left_part_length

        self.size -= data_length
        return out

if __name__ == "__main__":
    import utilities

    frame_length = 480
    length = 9600

    out_data = numpy.zeros(length)

    in_data = numpy.random.rand(length)

    buffer = RingBuffer(length)

    current_in_idx = 0
    while not buffer.IsFull():
        buffer.Push(in_data[current_in_idx * frame_length: (current_in_idx + 1) * frame_length])
        current_in_idx += 1

    current_out_idx = 0
    while not buffer.IsEmpty():
        out_data[current_out_idx * frame_length: (current_out_idx + 1) * frame_length] = buffer.Pop(frame_length)
        current_out_idx += 1

    print(utilities.PrintMetadata(utilities.GetMetadata(out_data - in_data)))

    pylab.plot(in_data, label = "in")
    pylab.plot(out_data, label = "out")

    pylab.legend()
    pylab.show()
