#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "openwav.cpp"
#include "complex.cpp"
#include "FFT.cpp"

using namespace std;

void paddingToPowerOfTwo(vector<double> &data)
{
    unsigned int n = data.size();

    // if power is 2 then return
    if ((n & (n - 1)) == 0)
        return;

    // get the next number which power is 2
    unsigned int base_2_num = 1;
    while (base_2_num < n)
        base_2_num <<= 1;

    // padding zero
    data.resize(base_2_num, 0);
}

void printData(complex data[], uint64_t N)
{
    int start = 50022, end = 50030;
    for (int i = start; i < end; i++)
        cout << data[i].re << " ";
    cout << "\n";

    fft(data, N);
    for (int i = start; i < end; i++)
        cout << data[i].re << " ";
    cout << "\n";

    ifft(data, N);
    for (int i = start; i < end; i++)
        cout << data[i].re << " ";
    cout << "\n";
}

int main(int argc, char *argv[])
{
    const char *file_path = (argc > 0) ? argv[1] : nullptr;
    vector<double> audio_data;

    // load .wav into data
    loadAudioData(file_path, audio_data);

    // padding data
    paddingToPowerOfTwo(audio_data);

    // fft
    uint64_t N = audio_data.size();
    complex *data = initialize(audio_data, vector<double>(N, 0));
    fft(data, N);

    // filter
    // ifft
    // data to wav audio

    printData(data, N);

    return 0;
}