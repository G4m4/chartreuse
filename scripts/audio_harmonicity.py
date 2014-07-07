#!/usr/bin/env python
'''
@file audio_harmonicity.py
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

import fundamental_frequency
import window


def PowerSpectrum(data, window, dft_length):
    '''
    Evaluate the power spectrum for the given data
    '''
    fft_data = numpy.fft.rfft(data * window, dft_length)
    out = 2.0 * numpy.abs(fft_data ** 2)

    return out

class AudioHarmonicity(object):
    '''
    Harmonicity algorithm test implementation
    '''
    def __init__(self, sampling_freq, frame_length, overlap):
        self._aff = fundamental_frequency.FundamentalFrequency(sampling_freq,
                                                               frame_length,
                                                               overlap)
        self._window = window.Hamming(frame_length)
        self._dft_length = 2048

    def Process(self, frame):
        '''
        Actual processing function, retrieve the fundamental frequency
        for the given frame
        '''
        arg_min = self._aff.Process(frame) - self._aff.min_lag
        actual_index = min(numpy.floor(arg_min), len(self._aff.xcorr) - 2)
        left = self._aff.xcorr[actual_index]
        right = self._aff.xcorr[actual_index + 1]
        ah = fundamental_frequency.LinearInterpolation(left, right, arg_min - actual_index)
        # ULH
        signal_spectrum = PowerSpectrum(frame,
                                        self._window.window_data,
                                        self._dft_length)
        power_spectrum = PowerSpectrum(self._aff.combed_signal,
                                       self._window.window_data,
                                       self._dft_length)
        ulh = self._GetUpperFrequency(self._GetUpperLimit(signal_spectrum,
                                                          power_spectrum))
        return (numpy.abs(ah), ulh)

    def _GetUpperLimit(self, signal_spectrum, power_spectrum):
        '''
        Actual processing function, retrieve the fundamental frequency
        for the given frame
        '''
        for lolimit in range(len(signal_spectrum) - 1, 0, -1):
            SignalPower = 0.0
            CombedSignalPower = 0.0
            for i in range(len(signal_spectrum) - 1, lolimit, -1):
                SignalPower += signal_spectrum[i]
                CombedSignalPower += power_spectrum[i]
            if SignalPower > 0.0:
                if CombedSignalPower / SignalPower < 0.5:
                    return lolimit
        return 0.0

    def _GetUpperFrequency(self, bin):
        # octave-based scale centered upon 1KHz
        octaves = [31.25, 62.5, 125, 250, 500, 1000, 2000, 4000, 8000, 16000]
        freq = bin * (self._aff.sampling_freq / self._dft_length)

        if freq < octaves[0]:
            return octaves[0]
        elif freq > octaves[9]:
            return octaves[9]
        else:
            i = 1
            while freq > octaves[i]:
                i += 1
            if freq < (octaves[i] + octaves[i-1]) / 2:
                return octaves[i-1]
            else:
                return octaves[i]

if __name__ == "__main__":
    from scipy import signal
    from scipy import io
    from scipy.io.wavfile import read
    import scipy.io.wavfile

    import utilities

    sampling_freq = 48000.0
    frame_length = 480
    overlap = 2 * frame_length
    window_length = overlap + frame_length
    actual_num_frame = 8
    actual_in_length = actual_num_frame * frame_length

    test_data = numpy.zeros(actual_num_frame * 1025)
    out_data = numpy.zeros((actual_in_length, 2))
    desc_data = numpy.zeros((actual_num_frame, 2))

    sep = 'f,\n'
    sin_path = '../chartreuse/tests/data/'
    sin_filename = "data_in_sin.dat"
    sin_data = numpy.fromfile(sin_path + sin_filename, sep=sep)[0:actual_in_length]
    freq = 140.0
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

    descriptor = AudioHarmonicity(sampling_freq, frame_length, overlap)

    for idx, frame in enumerate(sin_data.reshape(actual_num_frame, frame_length)):
        desc_data[idx, :] = descriptor.Process(frame)
        out_data[idx * frame_length: (idx + 1) * frame_length, 0] = desc_data[idx, 0] * numpy.ones(frame_length)
        out_data[idx * frame_length: (idx + 1) * frame_length, 1] = desc_data[idx, 1] * numpy.ones(frame_length)

    pylab.plot(sin_data, label = "in")
    print("ah: " + str(numpy.max(out_data[:, 0])))
    print("ulh: " + str(numpy.max(out_data[:, 1])))
    pylab.plot(out_data[:, 0], label = "ah")
    pylab.plot(out_data[:, 1] / numpy.max(out_data[:, 1]), label = "ulh")

    pylab.legend()
    pylab.show()
