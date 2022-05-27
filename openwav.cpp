#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>

using namespace std;

typedef struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4];        // RIFF Header Magic header
    uint32_t ChunkSize;     // RIFF Chunk Size
    uint8_t WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t FmtTag[4];      // "fmt" header
    uint32_t SubChunk1Size; // Size of the fmt chunk
    uint16_t AudioFormat;   // Audio format: 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t NumOfChan;     // Number of channels: 1=Mono, 2=Sterio
    uint32_t SamplesRate;   // Sampling Frequency (Hz)
    uint32_t bytesRate;     // bytes per second
    uint16_t blockAlign;    // 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample; // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t DataTag[4];     // "data" string
    uint32_t SubChunk2Size; // Sampled data length
} wavheader;

class WavFile
{
public:
    WavFile() {};
    void printHeaderInfo();
    int readAudioData(const char *file_path, vector<int16_t> &data);
    int writeAudioData(vector<int16_t> &data);

private:
    int getFileSize(FILE *inFile)
    {
        int fileSize = 0;
        fseek(inFile, 0, SEEK_END);

        fileSize = ftell(inFile);
        fseek(inFile, 0, SEEK_SET);

        return fileSize;
    }
    FILE *openWav(const char *file_path)
    {
        string input;
        if (file_path == nullptr)
        {
            cout << "Input wave file name: ";
            cin >> input;
            file_path = input.c_str();
        }
        else
        {
            cout << "Input wave file name: " << file_path << endl;
        }

        FILE *wav_file = fopen(file_path, "rb");
        if (wav_file == nullptr)
        {
            fprintf(stderr, "Unable to open wave file: %s\n", file_path);
            exit(1);
        }

        return wav_file;
    }

    wavheader wav_header;
    vector<size_t> byte_arr;
};

void WavFile::printHeaderInfo()
{
    cout << "\nHeader Infomation: \n";
    cout << "RIFF header                :" << wav_header.RIFF[0] << wav_header.RIFF[1] << wav_header.RIFF[2] << wav_header.RIFF[3] << endl;
    cout << "Data size                  :" << wav_header.ChunkSize << endl;
    cout << "WAVE header                :" << wav_header.WAVE[0] << wav_header.WAVE[1] << wav_header.WAVE[2] << wav_header.WAVE[3] << endl;

    cout << "FMT                        :" << wav_header.FmtTag[0] << wav_header.FmtTag[1] << wav_header.FmtTag[2] << wav_header.FmtTag[3] << endl;
    cout << "Chunk 1 Size               :" << wav_header.SubChunk1Size << endl;
    cout << "Audio Format               :" << wav_header.AudioFormat << endl;
    cout << "Number of channels         :" << wav_header.NumOfChan << endl;
    cout << "Sampling Rate              :" << wav_header.SamplesRate << endl;
    cout << "Number of bytes per second :" << wav_header.bytesRate << endl;
    cout << "Block align                :" << wav_header.blockAlign << endl;
    cout << "Number of bits used        :" << wav_header.bitsPerSample << endl;

    cout << "Data string                :" << wav_header.DataTag[0] << wav_header.DataTag[1] << wav_header.DataTag[2] << wav_header.DataTag[3] << endl;
    cout << "Data length                :" << wav_header.SubChunk2Size << endl;
}

int WavFile::readAudioData(const char *file_path, vector<int16_t> &data)
{
    FILE *wav_file = openWav(file_path);
    FILE *outfile = fopen("test_r.wav", "wb");  // write test
    FILE *outfile2 = fopen("test_w.wav", "wb"); // write test

    // read the header
    size_t byte_read = fread(&wav_header, 1, sizeof(wav_header), wav_file);
    fwrite(&wav_header, 1, sizeof(wav_header), outfile);  // write test
    fwrite(&wav_header, 1, sizeof(wav_header), outfile2); // write test

    fseek(wav_file, 44, SEEK_SET);  // skip header data
    cout << "Header Read " << byte_read << " bytes." << endl;
    if (byte_read != 44)
    {
        fprintf(stderr, "wav header is not 44 bytes\n");
        exit(1);
    }

    // read data
    if (byte_read > 0)
    {
        uint16_t bytes_per_sample = wav_header.bitsPerSample / 8;      // Number of bytes per sample
        uint64_t numSamples = wav_header.ChunkSize / bytes_per_sample; // How many samples are in the wav file
        const uint16_t buffer_size = wav_header.SubChunk2Size / bytes_per_sample;

        int16_t *buffer = new int16_t[buffer_size];
        int i = 0;
        while ((byte_read = fread(buffer, bytes_per_sample, buffer_size, wav_file)) > 0)
        {
            data.push_back(*buffer);
            byte_arr.push_back(byte_read);
            fwrite(buffer, bytes_per_sample, byte_read, outfile);      // write test
            fwrite(&(data[i]), bytes_per_sample, byte_read, outfile2); // write test
            i++;
        }

        delete[] buffer;
        buffer = nullptr;
    }
    fclose(wav_file);

    return 0;
}

int WavFile::writeAudioData(vector<int16_t> &data)
{
    FILE *outfile = fopen("test_output.wav", "wb");

    // write the header
    fwrite(&wav_header, 1, sizeof(wav_header), outfile);
    fseek(outfile, 44, SEEK_SET);  // skip header data

    uint16_t bytes_per_sample = wav_header.bitsPerSample / 8;
    const uint16_t buffer_size = wav_header.SubChunk2Size / bytes_per_sample;

    // write data
    int16_t *buffer = new int16_t[buffer_size];
    for(int i = 0; i < byte_arr.size(); i++)
    {
        *buffer = data[i];
        fwrite(buffer, bytes_per_sample, byte_arr[i], outfile);
    }
    delete[] buffer;
    buffer = nullptr;

    fclose(outfile);

    return 0;
}

/*
    size_t fread(void * ptr, size_t size, size_t count, FILE * stream)
        ptr：指向一塊記憶體的指標，該塊記憶體至少要(size * count)個的大小
        size：要讀取的每一個元素大小(單位為 byte)
        count：要讀取的元素數量
        stream：指向 FILE 物件的指標
*/