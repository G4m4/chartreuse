#!/usr/bin/env python
'''
@file dft.py
@brief Sandbox for DFT algorithms tests
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

def ExportCArrayFile(data, out_path, sep):
    '''
    Writes  DFT for various DFT size of a same input data
    Write output into text files
    return a tuple of DFT data for various DFT sizes
    '''
    data.tofile(out_path, sep)
    # Append the last trailing "f" for float
    out_file = open(out_path, 'a')
    out_file.write('f')

def CreateTestDFTData(data_in, fft_lengths):
    '''
    Computes DFT for various DFT size of a same input data
    Write output into text files
    return a list of DFT data for various DFT sizes
    '''
    fft_data = []
    for fft_length in fft_lengths:
        fft_data.append(numpy.fft.rfft(data_in, fft_length))

    return fft_data

if __name__ == "__main__":
    small_length = 64
    medium_length = 512
    large_length = 4096

    sep = 'f,\n'

    sin_path = '../chartreuse/tests/data/'
    sin_filename = "data_in_sin.dat"
    sin_data = numpy.fromfile(sin_path + sin_filename, sep=sep)

    out_path = '../chartreuse/tests/data/'
    data = CreateTestDFTData(sin_data,
                             (small_length, medium_length, large_length))
    ExportCArrayFile(numpy.real(data[0]),
                     out_path + 'dft_small.dat',
                     sep)
    ExportCArrayFile(numpy.real(data[1]),
                     out_path + 'dft_medium.dat',
                     sep)
    ExportCArrayFile(numpy.real(data[2]),
                     out_path + 'dft_large.dat',
                     sep)

#     toto = numpy.fromfile('../build/chartreuse/tests/out.dat', dtype=float, sep='\n')
#     pylab.plot(toto)
    pylab.plot(numpy.real(data[1]))
    data = [1.0, 1.0, 1.0, 1.0, 1.0]
    res = numpy.fft.rfft(data, 4)
    pylab.plot(numpy.real(res))
    pylab.show()
