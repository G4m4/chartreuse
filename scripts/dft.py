#!/usr/bin/env python
'''
@file dft.py
@brief Sandbox scripts for DFT algorithm tests
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
    pylab.show()
