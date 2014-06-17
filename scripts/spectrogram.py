#!/usr/bin/env python
'''
@file spectrogram.py
@brief Sandbox for a spectrogram algorithm
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

import ringbuffer
import window

class Spectrogram(object):
    '''
    Spectrogram implementation
    '''
    def __init__(self, sampling_freq, dft_length, frame_length, overlap):
        self.sampling_freq = sampling_freq
        self.dft_length = dft_length
        self.window_length = frame_length + overlap
        self.apodizer = window.Hamming(self.window_length)
        self.buffer = ringbuffer.RingBuffer(self.window_length, overlap)
        self.buffer.Fill(0.0, frame_length * 2)

    def Process(self, frame):
        '''
        Actual processing function, retrieve the accumulated spectrogram
        for the given frame
        '''
        self.buffer.Push(frame)
        current_frame = self.buffer.PopOverlapped(self.window_length)

        current_frame = self.apodizer.window_data * current_frame
        dft_data = numpy.fft.rfft(current_frame, self.dft_length)

        return dft_data

if __name__ == "__main__":
    from scipy import signal
    from scipy import io
    from scipy.io.wavfile import read
    import scipy.io.wavfile

    import utilities

    sampling_freq = 48000.0
    dft_length = 2048

    frame_length = 480
    overlap = 2 * frame_length
    window_length = overlap + frame_length
    actual_num_frame = 32
    actual_in_length = actual_num_frame * frame_length

    descriptor_length = dft_length / 2 + 1

    desc_data = numpy.zeros((descriptor_length, actual_num_frame),
                            dtype = numpy.complex64)

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

    descriptor = Spectrogram(sampling_freq, dft_length, frame_length, overlap)

    for idx, frame in enumerate(sin_data.reshape(actual_num_frame, frame_length)):
        desc_data[:, idx] = descriptor.Process(frame)

    pylab.imshow(numpy.abs(desc_data), aspect = 'auto', origin = 'lower')

    pylab.show()
