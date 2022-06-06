#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>

#include "openwav.cpp"
#include "complex.cpp"
#include "FFT.cpp"
#include "scale.cpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(_enhancer, a)
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