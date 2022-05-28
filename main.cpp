#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "openwav.cpp"
#include "complex.cpp"
#include "FFT.cpp"

using namespace std;

void paddingToPowerOfTwo(vector<int16_t> &data)
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
    int start = 500220, end = 500230;
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
    vector<int16_t> audio_data;

    // 1. load .wav into data
    WavFile wav_file(file_path);
    wav_file.readAudioData(audio_data);

    // 2. padding data
    paddingToPowerOfTwo(audio_data);

    // 3. fft
    uint64_t N = audio_data.size();
    complex *data = initialize(audio_data);
    fft(data, N);

    // 4. filter frequency
    // 5. ifft
    // 6. data to wav audio
    // wav_file.writeAudioData(data);

    // printData(data, N);

    return 0;
}