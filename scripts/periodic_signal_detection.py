#!/usr/bin/env python
'''
@file periodic_signal_detection.py
@brief Sandbox for "Periodic signal detection" algorithm tests
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

def PeriodicSignaDetection(frame, sampling_freq):
    '''
    PeriodicSignaDetection algorithm test implementation
    '''
    sample_num = len(frame)
    total_power = numpy.sum(frame ** 2)

    out = numpy.zeros(sample_num - 1)
    for k in range(0, sample_num - 1):
        part_power = numpy.sum(frame[k + 1:sample_num - 1] ** 2)
        out[k] = numpy.sum((frame[k + 1:sample_num] - frame[0:sample_num - k - 1]) ** 2)
        out[k] /= total_power + part_power

    return out

if __name__ == "__main__":
#     from scipy import signal

    sampling_freq = 48000.0
    window_length = 1920
    actual_num_frame = 2
    actual_in_length = actual_num_frame * window_length

    out_data = numpy.zeros(actual_in_length)
    power_data = numpy.zeros(actual_in_length)
    test_data = numpy.zeros(actual_in_length)

    sep = 'f,\n'
    sin_path = '../chartreuse/tests/data/'
    sin_filename = "data_in_sin.dat"
    sin_data = numpy.fromfile(sin_path + sin_filename, sep=sep)[0:actual_in_length]
#     time = numpy.arange(0, 1.0, 1.0 / sampling_freq)
#     sin_data = signal.chirp(t = time,
#                             f0 = 440.0,
#                             t1 = 1,
#                             f1 = 8800.0)[0:actual_in_length]
#     sin_data = numpy.random.rand(actual_in_length)

    for idx, frame in enumerate(sin_data.reshape(actual_num_frame, window_length)):
        out_data[idx * (window_length - 1):(idx + 1) * (window_length - 1)] = \
            PeriodicSignaDetection(frame, sampling_freq)

    pylab.plot(sin_data / numpy.max(sin_data), label = "in")
    pylab.plot(out_data / numpy.max(out_data), label = "out")

    pylab.legend()
    pylab.show()
