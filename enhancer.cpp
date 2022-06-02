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

void printHelp()
{
    ifstream file("help.txt");
    string str;
    while (getline(file, str))
        cout << str << endl;
    file.close();

    exit(0);
}

const char *checkMode(const char *mode)
{
    if (strcmp(mode, "gain") != 0 && strcmp(mode, "debuff") != 0)
    {
        fprintf(stderr, "Please select `gain` or `debuff` as the mode.\n");
        exit(1);
    }
    return mode;
}

void manageArguments(int argc, char *argv[], const char *&file_path, const char *&mode, int &freq)
{
    for (int i = 1; i < argc; i += 2)
    {
        string arg = argv[i];
        if (arg == "--help" || arg == "-h")
            printHelp();
        else if (arg == "--file" || arg == "-F")
            file_path = argv[i + 1];
        else if (arg == "--mode" || arg == "-m")
            mode = checkMode(argv[i + 1]);
        else if (arg == "--freq" || arg == "-f")
            freq = atoi(argv[i + 1]);
    }
    if (file_path == nullptr)
    {
        string input;
        cout << "Input wave file name: ";
        cin >> input;
        file_path = input.c_str();
    }
    if (mode == nullptr)
    {
        string input;
        cout << "Input mode: (gain, debuff)";
        cin >> input;
        mode = input.c_str();
    }
    if (freq == 0)
    {
        int input;
        cout << "Input frequency: ";
        cin >> input;
        freq = input;
    }
}

int enhancer(int argc, char *argv[])
{
    const char *file_path = nullptr;
    const char *mode = nullptr;
    int freq = 0;

    // manage arguments
    manageArguments(argc, argv, file_path, mode, freq);

    // 1. load .wav into data
    vector<int16_t> signal;
    WavFile wav_file(file_path);
    wav_file.readAudioData(signal);
    int signal_size = signal.size();

    // 2. fft
    signal = paddingToPowerOfTwo(signal);
    uint64_t N = signal.size();
    complex *data = initialize(signal);
    fft(data, N);

    // 3. adjust frequency
    scale(data, wav_file, mode, freq);

    // 4. ifft
    ifft(data, N);

    // 5. data to wav audio
    vector<int16_t> new_signal = getSignal(data, signal_size);
    wav_file.writeAudioData(new_signal);

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