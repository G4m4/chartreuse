#!/usr/bin/env python
'''
@file fundamental_frequency.py
@brief Sandbox for "Fundamental frequency extraction" algorithm tests
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

import correlation
import ringbuffer

def LinearInterpolation(left, right, ratio):
    '''
    Given two values, linearly interpolate between them
    '''
    return left * (1.0 - ratio) + right * ratio

def ParabolicArgMin(vector):
    '''
    Given a "peak" vector, e.g. one that contain [y1 y2 y3] such as y2
    is the peak value, perform a parabolic estimation of the peak position
    '''
    prev = vector[0]
    peak = vector[1]
    after = vector[2]
    min_position = 0.5 * (prev - after) / (prev - 2.0 * peak + after)

    return min_position

class FundamentalFrequency(object):
    '''
    Fundamental frequency algorithm test implementation
    '''
    def __init__(self, sampling_freq, frame_length, overlap):
        self.sampling_freq = sampling_freq
        self.lofreq = 62.5
        self.hifreq = 1500.0
        self.max_lag = int(numpy.ceil(sampling_freq / self.lofreq))
        self.min_lag = int(numpy.floor(sampling_freq / self.hifreq))
        self.frame_length = frame_length
        self.window_length = frame_length * overlap
        self.buffer = ringbuffer.RingBuffer(self.window_length, overlap)
        self.buffer.Fill(0.0, self.frame_length * 2)

    def Process(self, frame):
        '''
        Actual processing function, retrieve the fundamental frequency
        for the given frame
        '''
        self.buffer.Push(frame)
        current_frame = self.buffer.PopOverlapped(self.window_length)

        xcorr = correlation.CustomAutoCorrelation(current_frame,
                                                  self.min_lag,
                                                  self.max_lag)
        f0_lag = self._FindPeaks(xcorr, self.min_lag, self.max_lag)
        f0_lag += self.min_lag
        self.combed_signal = self._GetCombedSignal(current_frame,
                                                   f0_lag,
                                                   self.frame_length)

        return f0_lag

    def _FindPeaks(self, data, min_lag, max_lag):
        '''
        Basic peak finding algorithm
        '''
        value = 0.0
        min_idx = max_lag
        threshold = 5e-3
        for i in range(min_lag, min(max_lag, len(data) - 1)):
            if data[i] - data[i - 1] > 0.0:
                if data[i + 1] - data[i] < 0.0:
                    neighbourhood = [data[i - 1], data[i], data[i + 1]]
                    argmin = ParabolicArgMin(neighbourhood)
                    interpolated_min = LinearInterpolation(data[i - 1],
                                                           data[i],
                                                           1.0 + argmin)
                    # Value of the parabolic approximation, not proven useful
#                         interpolated_min = data[i - 1] * data[i - 1] \
#                                             - 8.0 * data[i - 1] * data[i] \
#                                             - 2.0 * data[i - 1] * data[i + 1] \
#                                             + data[i + 1] * data[i + 1] - 8.0 * data[i] * data[i + 1] \
#                                             + 16.0 * data[i] * data[i]
#                         interpolated_min /= -8.0 * (data[i - 1] - 2.0 * data[i] + data[i + 1])
                    if (interpolated_min > value + threshold) :
                        min_idx = argmin + i
                        value = interpolated_min
        return min_idx

    def _GetCombedSignal(self, signal, lag, frame_length):
        '''
        Retrieve the combed signal of the input
        '''
        base_range = range(len(signal) - frame_length,
                           len(signal))
        lag_range = range(int(len(signal) - frame_length - lag),
                          int(len(signal) - lag))

        a = numpy.sum(signal[base_range]
                      * signal[lag_range])
        b = numpy.sum(signal[lag_range] ** 2.0)

        d = 0.0
        if b > 0.0:
            d = a / b

        out = signal[base_range] - d * signal[lag_range]

        return out

if __name__ == "__main__":
    from scipy import signal
    from scipy import io
    from scipy.io.wavfile import read
    import scipy.io.wavfile

    import utilities

    sampling_freq = 48000.0
    frame_length = 480
    overlap = 3
    window_length = overlap * frame_length
    actual_num_frame = 16
    actual_in_length = actual_num_frame * frame_length

    out_data = numpy.zeros(actual_in_length)
    combed_data = numpy.zeros(actual_in_length)
    desc_data = numpy.zeros((actual_num_frame, 1))

    sep = 'f,\n'
    sin_path = '../chartreuse/tests/data/'
    sin_filename = "data_in_sin.dat"
    sin_data = numpy.fromfile(sin_path + sin_filename, sep=sep)[0:actual_in_length]
    freq = 445.0
    time = numpy.linspace(0, 2 * numpy.pi * freq, sampling_freq)
    sin_data = numpy.sin(time)[0:actual_in_length]
    (_, sin_data) = read("../chartreuse/tests/data/C5_flute.wav")
    sin_data = sin_data[0:actual_in_length] / float(numpy.max(sin_data[0:actual_in_length]))
#     time = numpy.arange(0, 1.0, 1.0 / sampling_freq)
#     sin_data = signal.chirp(t = time,
#                             f0 = 100.0,
#                             t1 = 0.5,
#                             f1 = 1000.0)[0:actual_in_length]
#     sin_data = numpy.random.rand(actual_in_length) * 2.0 - 1.0
#     sin_data = 0.5 * numpy.ones(actual_in_length)
#     sin_data = numpy.zeros(actual_in_length)
#     pulse_occurence = 512
#     pulse_width = 64
#     for idx in range(0, actual_in_length, pulse_width):
#         if (idx % pulse_occurence == 0):
#             sin_data[idx:idx + pulse_width / 2] = 1
#             sin_data[idx + pulse_width / 2:idx + pulse_width] = -1

    descriptor = FundamentalFrequency(sampling_freq, frame_length, overlap)

    for idx, frame in enumerate(sin_data.reshape(actual_num_frame, frame_length)):
        desc_data[idx, :] = descriptor.Process(frame)
        out_data[idx * frame_length: (idx + 1) * frame_length] = desc_data[idx, 0] * numpy.ones(frame_length)
        combed_data[idx * frame_length: (idx + 1) * frame_length] = descriptor.combed_signal

    print(utilities.PrintMetadata(utilities.GetMetadata(combed_data)))

    pylab.plot(sin_data, label = "in")
    pylab.plot(combed_data, label = "combed")
    pylab.plot(out_data / numpy.max(out_data), label = "out")

    pylab.legend()
    pylab.show()
