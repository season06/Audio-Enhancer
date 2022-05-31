#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "openwav.cpp"
#include "complex.cpp"
#include "FFT.cpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;

vector<int16_t> paddingToPowerOfTwo(vector<int16_t> data)
{
    unsigned int n = data.size();

    // if power is 2 then return
    if ((n & (n - 1)) == 0)
        return data;

    // get the next number which power is 2
    unsigned int base_2_num = 1;
    while (base_2_num < n)
        base_2_num <<= 1;

    // padding zero
    data.resize(base_2_num, 0);

    return data;
}

int enhancer(int argc, char *argv[])
{
    const char *file_path = (argc > 0) ? argv[1] : nullptr;
    vector<int16_t> audio_data;

    // 1. load .wav into data
    WavFile wav_file(file_path);
    wav_file.readAudioData(audio_data);

    // 2. padding data
    audio_data = paddingToPowerOfTwo(audio_data);

    // 3. fft
    uint64_t N = audio_data.size();
    complex *data = initialize(audio_data);
    fft(data, N);

    // 4. filter frequency
    // 5. ifft
    // 6. data to wav audio
    // wav_file.writeAudioData(data);

    return 0;
}

PYBIND11_MODULE(enhancer, a)
{
    a.doc() = "audio enhancer";
    a.def("fft", &fft, py::arg("data"), py::arg("N"), py::arg("inv") = false);
    a.def("ifft", &ifft);
    a.def("paddingToPowerOfTwo", &paddingToPowerOfTwo);
    a.def("initialize", &initialize);
    a.def("getReals", &getReals);
    a.def("getImags", &getImags);

    py::class_<Complex>(a, "Complex")
        .def_readonly("re", &Complex::re)
        .def_readonly("im", &Complex::im);

    py::class_<WavFile>(a, "WavFile")
        .def(py::init<>())
        .def(py::init<const char *>())
        .def("printHeaderInfo", &WavFile::printHeaderInfo)
        .def("readAudioData", &WavFile::readAudioData)
        .def("writeAudioData", &WavFile::writeAudioData);
}