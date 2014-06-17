#!/usr/bin/env python
'''
@file freq_scale.py
@brief Sandbox for various frequency scale generators
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

class LogFreqScale(object):
    '''
    Log frequency scale
    '''
    def __init__(self, length, dft_length, sampling_freq):
        self.length = length
        self.dft_length = dft_length
        self.sampling_freq = sampling_freq
        self._Synthesize()

    def _Synthesize(self):
        '''
        Actual processing function for generating the scale
        '''
        kLowBound = 2.0 * self.sampling_freq / self.dft_length
        kHighBound = self.sampling_freq * 0.5
        tmp = numpy.linspace(kLowBound, kHighBound, self.length)
        tmp[0] = self.sampling_freq / (self.dft_length * (3.0 / 4.0))

        self.data = numpy.log2(tmp * 0.001)

if __name__ == "__main__":
    import utilities

    sampling_freq = 48000.0
    dft_bins_count = 2048
    low_edge = 62.5
    high_edge = 1500.0

    low_edge_idx = numpy.ceil(low_edge * dft_bins_count / sampling_freq)
    high_edge_idx = dft_bins_count / 2 + 1
    length = high_edge_idx - low_edge_idx + 1

    generator = LogFreqScale(length, dft_bins_count, sampling_freq)

    out_data = generator.data

    print(utilities.PrintMetadata(utilities.GetMetadata(out_data)))

    pylab.plot(out_data, label = "out")

    pylab.legend()
    pylab.show()
