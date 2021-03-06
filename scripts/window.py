#!/usr/bin/env python
'''
@file window.py
@brief Sandbox for window application tests
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

class Hamming(object):
    '''
    Generator of Hamming window
    '''
    def __init__(self, window_length):
        self.window_length = window_length
        # Generation of the window
        # Scalar version
#         for idx in range(self._window_length):
#             self._window_data[idx] = 0.54 - (0.46 * numpy.cos(2.0 * numpy.pi * idx / (self.window_length - 1)))
        # Better version
        self.window_data = 0.54 * numpy.ones(self.window_length) \
                            - 0.46 * numpy.cos(
                                        numpy.arange(0.0,
                                                     2.0 * numpy.pi * self.window_length / (self.window_length - 1),
                                                     2.0 * numpy.pi / (self.window_length - 1)))

    def NormalizationFactor(self):
        return numpy.sum(self.window_data ** 2)

if __name__ == "__main__":
    length = 512

    out_data = numpy.zeros(length)

    apodizer = Hamming(length)
    out_data = apodizer.window_data

    pylab.plot(out_data)
    pylab.show()
