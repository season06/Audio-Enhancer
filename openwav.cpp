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

int getFileSize(FILE *inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);

    return fileSize;
}

void printHeaderInfo(wavheader wav_header, FILE* wav_file)
{
    int file_length = getFileSize(wav_file);

    cout << "\nHeader Infomation: \n";
    cout << "File is                    :" << file_length << " bytes." << endl;
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

int loadAudioData(const char *file_path, vector<double> &data)
{
    FILE *wav_file = openWav(file_path);
    wavheader wav_header;
    int header_size = sizeof(wav_header);

    // Read the header
    size_t byte_read = fread(&wav_header, 1, header_size, wav_file);
    cout << "Header Read " << byte_read << " bytes." << endl;
    if (byte_read != 44)
    {
        fprintf(stderr, "wav header is not 44 bytes\n");
        exit(1);
    }

    // Read data
    if (byte_read > 0)
    {
        uint16_t bytes_per_sample = wav_header.bitsPerSample / 8;      // Number of bytes per sample
        // uint64_t numSamples = wav_header.ChunkSize / bytes_per_sample; // How many samples are in the wav file?
        const uint16_t buffer_size = wav_header.SubChunk2Size / bytes_per_sample;

        int16_t *buffer = new int16_t[buffer_size];
        while ((byte_read = fread(buffer, bytes_per_sample, buffer_size, wav_file)) > 0)
        {
            data.push_back(*buffer);
        }
        // byte_read = 74
        // i * 74 / 2 (+ 37)

        delete[] buffer;
        buffer = nullptr;

        // printHeaderInfo(wav_header, wav_file);
    }
    fclose(wav_file);

    return 0;
}

/*
    size_t fread(void * ptr, size_t size, size_t count, FILE * stream)
        ptr：指向一塊記憶體的指標，該塊記憶體至少要(size * count)個的大小
        size：要讀取的每一個元素大小(單位為 byte)
        count：要讀取的元素數量
        stream：指向 FILE 物件的指標
*/