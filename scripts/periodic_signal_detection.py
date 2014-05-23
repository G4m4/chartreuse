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

def LinearInterpolation(left, right, ratio):
    '''
    Given two values, linearly interpolate between them
    '''
    return left * (1.0 - ratio) + right * ratio

def ParabolicArgMin(vector, peak_time):
    '''
    Given a "peak" vector, perform a parabolic estimation of the minimum
    '''
    prev = vector[0]
    peak = vector[1]
    after = vector[2]
    a = (after - prev) / 2.0 - (peak - prev)
    b = (after - prev) / 2.0 + a * 2.0
    min_position = -b / (2 * a) + peak_time
    return min_position

def AutoCorrelation(frame, sampling_freq):
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

def NormalizedAutoCorrelation(frame, sampling_freq):
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

def ZNCC(frame, sampling_freq):
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

def PeriodicSignaDetection(frame, sampling_freq):
    '''
    PeriodicSignaDetection algorithm test implementation
    @return (min_idx, Hk) where:
            min_idx    (integer)        Index of the minimum
            Hk    (float, within[0 ; 1])        Harmonicity, with 1 meaning
                                    purely periodic and 0 white noise
    '''
    sample_num = len(frame)
    nnc = AutoCorrelation(frame, sampling_freq)
    min_idx = -1
    min_value = 1.0
    min_idx = numpy.argmin(nnc[0:sample_num - 960])
    min_value = nnc[min_idx]
#     for i in range(1, sample_num - 20):
#         # TODO(gm): parabolic approximation of the minimum
#         if nac[i] - nac[i - 1] < 0.0:
#             if nac[i + 1] - nac[i] > 0.0:
#                 neighbourhood = [nac[i - 1], nac[i], nac[i + 1]]
#                 argmin = ParabolicArgMin(neighbourhood,
#                                          i)
#                 if (LinearInterpolation(nac[i - 1], nac[i], argmin - i) < min_value) :
#                     min_idx = argmin
#                     min_value = LinearInterpolation(nac[i - 1], nac[i], argmin - i)

    return (min_idx, 1.0 - min_value)

if __name__ == "__main__":
    from scipy import signal

    sampling_freq = 48000.0
    window_length = 1920
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
    time = numpy.arange(0, 1.0, 1.0 / sampling_freq)
    sin_data = signal.chirp(t = time,
                            f0 = 440.0,
                            t1 = 1,
                            f1 = 8800.0)[0:actual_in_length]
#     sin_data = numpy.random.rand(actual_in_length)
#     sin_data = 0.5 * numpy.ones(actual_in_length)
#     sin_data = numpy.zeros(actual_in_length)
#     pulse_occurence = 512
#     pulse_width = 64
#     for idx in range(0, actual_in_length, pulse_width):
#         if (idx % pulse_occurence == 0):
#             sin_data[idx:idx + pulse_width / 2] = 1
#             sin_data[idx + pulse_width / 2:idx + pulse_width] = -1

    for idx, frame in enumerate(sin_data.reshape(actual_num_frame, window_length)):
        desc_data[:, idx] = PeriodicSignaDetection(frame, sampling_freq)
        out_data[idx * window_length: (idx + 1) * window_length] = desc_data[1, idx] * numpy.ones(window_length)
        autocorr_data[idx * window_length: (idx + 1) * window_length] = AutoCorrelation(frame, sampling_freq)
        normautocorr_data[idx * window_length: (idx + 1) * window_length] = NormalizedAutoCorrelation(frame, sampling_freq)
        zncc_data[idx * window_length: (idx + 1) * window_length] = ZNCC(frame, sampling_freq)

    pylab.plot(sin_data, label = "in")
#     pylab.plot(2.0 * ref_data / numpy.max(ref_data), label = "ref")
    pylab.plot(autocorr_data, label = "autocorr")
    pylab.plot(normautocorr_data, label = "NCC")
    pylab.plot(zncc_data, label = "ZNCC")
    pylab.plot(out_data, label = "out")

    pylab.legend()
    pylab.show()
