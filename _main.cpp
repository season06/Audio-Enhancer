#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <math.h>
#include <fstream>

#include "openwav.cpp"
#include "complex.cpp"
#include "FFT.cpp"
#include "scale.cpp"

using namespace std;

void printHelp()
{
    ifstream file("help.txt");
    string str;
    while (getline(file, str))
        cout << str << endl;
    file.close();

    exit(0);
}

const char *checkFile(const char *file)
{
    size_t len = strlen(file);
    if (file[len - 4] == '.' || file[len - 3] == 'w' || file[len - 2] == 'a' || file[len - 1] == 'v')
        return file;

    fprintf(stderr, "Please input `.wav` file.\n");
    exit(1);
}

const char *checkMode(const char *mode)
{
    if (strcmp(mode, "gain") == 0 || strcmp(mode, "debuff") == 0)
        return mode;

    fprintf(stderr, "Please select `gain` or `debuff` as the mode.\n");
    exit(1);
}

void manageArguments(int argc, char *argv[], const char *&file_path, const char *&mode, int &freq)
{
    for (int i = 1; i < argc; i += 2)
    {
        string arg = argv[i];
        if (arg == "--help" || arg == "-h")
            printHelp();
        else if (arg == "--file" || arg == "-F")
            file_path = checkFile(argv[i + 1]);
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
        file_path = checkFile(input.c_str());
    }
    if (mode == nullptr)
    {
        string input;
        cout << "Input mode (gain, debuff): ";
        cin >> input;
        mode = checkMode(input.c_str());
    }
    if (freq == 0)
    {
        int input;
        cout << "Input frequency: ";
        cin >> input;
        freq = input;
    }
}

int main(int argc, char *argv[])
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

    // 3. scale frequency
    scale(data, wav_file, N, mode, freq);

    // 4. ifft
    ifft(data, N);

    // 5. data to wav audio
    vector<int16_t> new_signal = getSignal(data, signal_size);
    wav_file.writeAudioData(new_signal);

    return 0;
}