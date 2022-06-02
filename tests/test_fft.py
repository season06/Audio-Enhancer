import sys, os
sys.path.append(os.getcwd())

import enhancer
import pytest
import timeit
import numpy as np
from scipy.fftpack import fft, ifft


def test_padding():
    n_size = [3, 64, 400, 1025, 200000]
    ans_size = [4, 64, 512, 2048, 262144]

    for n, ans in zip(n_size, ans_size):
        data = np.random.randint(10, size=n, dtype=np.int16).tolist()
        padding_data = enhancer.paddingToPowerOfTwo(data)

        assert len(padding_data) == ans


def test_fft():
    max = 32767
    n_size = [1024, 2048, 4096, 8192, 16384, 32768, 65536]

    for n in n_size:
        data = np.random.randint(max, size=n, dtype=np.int16).tolist()
        comp = enhancer.initialize(data)

        enhancer.fft(comp, n)
        real = enhancer.getReals(comp, n)
        imag = enhancer.getImags(comp, n)

        py_fft = fft(data)

        for i in range(0, n):
            assert py_fft[i].real == pytest.approx(real[i], 1e-3)
            assert py_fft[i].imag == pytest.approx(imag[i], 1e-3)


def test_ifft():
    max = 32767
    n_size = [1024, 2048, 4096, 8192, 16384, 32768, 65536]

    for n in n_size:
        data = np.random.randint(max, size=n, dtype=np.int16).tolist()
        comp = enhancer.initialize(data)

        enhancer.fft(comp, n)
        enhancer.ifft(comp, n)
        c_ifft = enhancer.getReals(comp, n)

        py_fft = fft(data)
        py_ifft = ifft(py_fft)

        for i in range(0, n):
            assert abs(data[i] - c_ifft[i]) <= 1e-3
            assert abs(c_ifft[i] - py_ifft[i]) <= 1e-3


def test_performance():
    max = 32767
    n_size = [1024, 2048, 4096, 8192, 16384, 32768, 65536]

    with open('performance.txt', 'w') as f:
        f.write("Performance for FFT\n")

        for n in n_size:
            data = np.random.randint(max, size=n, dtype=np.int16).tolist()
            comp = enhancer.initialize(data)
            self_init = dict(enhancer=enhancer, comp=comp, n=n)
            py_init = dict(fft=fft, data=data)

            f.write(f"\nData size: {str(n)}\n")

            self_fft_time = timeit.timeit("enhancer.fft(comp, n)", number=1, globals=self_init)
            f.write(f"Self FFT time : {str(self_fft_time)}\n")

            py_fft_time = timeit.timeit("fft(data)", number=1, globals=py_init)
            f.write(f"Scipy FFT time: {str(py_fft_time)}\n")

        f.close()
