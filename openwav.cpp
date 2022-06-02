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
    uint32_t BytesPerSecond;// bytes per second
    uint16_t BlockAlign;    // 2=16-bit mono, 4=16-bit stereo
    uint16_t BitDepth;      // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t DataTag[4];     // "data" string
    uint32_t SubChunk2Size; // Sampled data length
} wavheader;

class WavFile
{
public:
    WavFile(){};
    WavFile(const char *file_path): file_path(file_path) {};
    void printHeaderInfo();
    void readAudioData(vector<int16_t> &data);
    void writeAudioData(vector<int16_t> &data);
    int getSampleRate();
    int getFileSize();

private:
    FILE *openWav(const char *file_path)
    {
        FILE *wav_file = fopen(file_path, "rb");
        if (wav_file == nullptr)
        {
            fprintf(stderr, "Unable to open wave file: %s\n", file_path);
            exit(1);
        }
        else
        {
            printf("Open %s successfully.\n", file_path);
        }

        return wav_file;
    }

    const char *file_path;
    wavheader wav_header;
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
    cout << "Number of bytes per second :" << wav_header.BytesPerSecond << endl;
    cout << "Block align                :" << wav_header.BlockAlign << endl;
    cout << "Number of bits used        :" << wav_header.BitDepth << endl;

    cout << "Data string                :" << wav_header.DataTag[0] << wav_header.DataTag[1] << wav_header.DataTag[2] << wav_header.DataTag[3] << endl;
    cout << "Data length                :" << wav_header.SubChunk2Size << endl;
}

int WavFile::getSampleRate()
{
    return wav_header.BytesPerSecond;
}

int WavFile::getFileSize()
{
    FILE *infile = openWav(file_path);

    int fileSize = 0;
    fseek(infile, 0, SEEK_END);
    fileSize = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    return fileSize;
}

void WavFile::readAudioData(vector<int16_t> &data)
{
    FILE *infile = openWav(file_path);

    // read the header
    size_t byte_read = fread(&wav_header, 1, sizeof(wav_header), infile);
    cout << "Header Read " << byte_read << " bytes." << endl;
    if (byte_read != 44)
    {
        fprintf(stderr, "wav header is not 44 bytes\n");
        exit(1);
    }

    // read data
    if (byte_read > 0)
    {
        uint16_t bytes_per_sample = wav_header.BitDepth / 8;      // Number of bytes per sample
        // uint64_t numSamples = wav_header.ChunkSize / bytes_per_sample; // How many samples are in the wav file
        const uint16_t buffer_size = wav_header.SubChunk2Size / bytes_per_sample;

        int16_t *buffer = new int16_t[buffer_size];
        while ((byte_read = fread(buffer, bytes_per_sample, buffer_size, infile)) > 0)
        {
            copy(buffer, buffer + byte_read, std::back_inserter(data));
        }
        delete[] buffer;
        buffer = nullptr;
    }
    fclose(infile);
    cout << "Data Read " << data.size() * 2 << " bytes." << endl;
}

void WavFile::writeAudioData(vector<int16_t> &data)
{
    FILE *outfile = fopen("output.wav", "wb");

    // write the header
    wav_header.NumOfChan = 1;
    wav_header.BytesPerSecond = wav_header.NumOfChan * wav_header.SamplesRate * (wav_header.BitDepth / 8);
    wav_header.BlockAlign = wav_header.NumOfChan * (wav_header.BitDepth / 8);
    fwrite(&wav_header, 1, sizeof(wav_header), outfile);

    // write data
    for(unsigned int i = 0; i < data.size(); i++)
    {
        fwrite(&data[i], 1, sizeof(int16_t), outfile);
    }

    fclose(outfile);
}

/*
    size_t fread(void * ptr, size_t size, size_t count, FILE * stream)
        ptr：指向一塊記憶體的指標，該塊記憶體至少要(size * count)個的大小
        size：要讀取的每一個元素大小(單位為 byte)
        count：要讀取的元素數量
        stream：指向 FILE 物件的指標
*/