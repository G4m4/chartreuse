#!/usr/bin/env python
'''
@file correlation.py
@brief Sandbox for various correlation algorithm tests
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
import utilities

@utilities.PrintTiming
def AutoCorrelation(frame):
    '''
    Perform the normalized autocorrelation of the input signal
    '''
    sample_num = len(frame)

    # Normalize with window norm
    kNorm = 1.0 / numpy.sum(frame ** 2)
#     kNorm = 1.0
    out = numpy.zeros(sample_num)
    for k in range(0, sample_num):
        out[k] = numpy.sum(frame[k + 1:sample_num] * frame[0:sample_num - k - 1])
        out[k] *= kNorm

    return out

@utilities.PrintTiming
def NormalizedAutoCorrelation(frame):
    '''
    Perform the normalized autocorrelation of the input signal
    '''
    sample_num = len(frame)

    out = numpy.ones(sample_num)
    toto  = numpy.ones(sample_num)
    for k in range(0, sample_num):
        part_power = numpy.sqrt(numpy.sum(frame[k + 1:sample_num] ** 2)
                                * numpy.sum(frame[0:sample_num - k - 1] ** 2))
        toto[k] = part_power
        out[k] = numpy.sum(frame[k + 1:sample_num] * frame[0:sample_num - k - 1])
        if part_power > 0:
            out[k] /= part_power

    return out

@utilities.PrintTiming
def ZNCC(frame):
    '''
    Perform the zero-mean normalized autocorrelation of the input signal
    '''
    sample_num = len(frame)

    out = numpy.ones(sample_num)
    toto  = numpy.ones(sample_num)
    for k in range(0, sample_num):
        left_part = frame[0:sample_num - k - 1]
        right_part = frame[k + 1:sample_num]
        mean_left = numpy.mean(left_part)
        mean_right = numpy.mean(right_part)
        part_power = numpy.sqrt(numpy.sum((left_part - mean_left) ** 2)
                                * numpy.sum((right_part - mean_right) ** 2))
        toto[k] = part_power
        out[k] = numpy.sum((left_part - mean_left)
                           * (right_part - mean_right))
        if part_power > 0:
            out[k] /= part_power

    return out

@utilities.PrintTiming
def CustomAutoCorrelation(frame, min_lag, max_lag):
    '''
    Perform a normalized autocorrelation of the input signal, on a limited interval
    '''
    frame_length = len(frame)

    out_length = max_lag - min_lag
    out = numpy.zeros(out_length)
    sig_power = numpy.sum(frame[max_lag:frame_length] ** 2.0)
    for lag in range(min_lag, max_lag):
        corr_power = numpy.sum(frame[max_lag:frame_length] * frame[max_lag - lag:frame_length - lag])
        lag_power = numpy.sum(frame[max_lag - lag:frame_length - lag] ** 2.0)
        if (lag_power != 0.0):
            out[lag - min_lag] = corr_power / numpy.sqrt(sig_power * 2.0 * lag_power)
        else:
            out[lag - min_lag] = 0.0

    return out

if __name__ == "__main__":
    from scipy import io
    from scipy.io.wavfile import read
    import scipy.io.wavfile

    sampling_freq = 48000.0
    window_length = 1440
    actual_num_frame = 2
    actual_in_length = actual_num_frame * window_length

    out_data = numpy.zeros(actual_in_length)
    ref_data = numpy.zeros(actual_in_length)
    desc_data = numpy.zeros((actual_num_frame, 2))
    autocorr_data = numpy.zeros(actual_in_length)
    normautocorr_data = numpy.zeros(actual_in_length)
    zncc_data = numpy.zeros(actual_in_length)

    sep = 'f,\n'
    sin_path = '../chartreuse/tests/data/'
    sin_filename = "data_in_sin.dat"
    sin_data = numpy.fromfile(sin_path + sin_filename, sep=sep)[0:actual_in_length]
    freq = 440.0
    time = numpy.linspace(0, 2 * numpy.pi * freq, sampling_freq)
    sin_data = numpy.sin(time)[0:actual_in_length]
    (_, sin_data) = read("../chartreuse/tests/data/C5_flute.wav")
    sin_data = sin_data[0:actual_in_length] / float(numpy.max(sin_data[0:actual_in_length]))
#     time = numpy.arange(0, 1.0, 1.0 / sampling_freq)
#     sin_data = signal.chirp(t = time,
#                             f0 = 440.0,
#                             t1 = 1,
#                             f1 = 8800.0)[0:actual_in_length]
#     sin_data = numpy.random.rand(actual_in_length) * 2.0 - 1.0
#     sin_data = 0.5 * numpy.ones(actual_in_length)
#     sin_data = numpy.zeros(actual_in_length)
#     pulse_occurence = 512
#     pulse_width = 64
#     for idx in range(0, actual_in_length, pulse_width):
#         if (idx % pulse_occurence == 0):
#             sin_data[idx:idx + pulse_width / 2] = 1
#             sin_data[idx + pulse_width / 2:idx + pulse_width] = -1

    min_lag = 32
    max_lag = 768
    corr_length = max_lag - min_lag

    for idx, frame in enumerate(sin_data.reshape(actual_num_frame, window_length)):
        normautocorr_data[idx * window_length: (idx + 1) * window_length] = NormalizedAutoCorrelation(frame)
        autocorr_data[idx * window_length: (idx + 1) * window_length] = AutoCorrelation(frame)
        zncc_data[idx * window_length: (idx + 1) * window_length] = ZNCC(frame)
        out_data[idx * corr_length: (idx + 1) * corr_length] = CustomAutoCorrelation(frame, min_lag, max_lag)

    pylab.plot(sin_data, label = "in")
    pylab.plot(autocorr_data, label = "autocorr")
    pylab.plot(normautocorr_data, label = "NCC")
    pylab.plot(zncc_data, label = "ZNCC")
    pylab.plot(out_data, label = "out")

    pylab.legend()
    pylab.show()
