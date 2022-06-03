#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>

using namespace std;

typedef struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4];        // "RIFF" = 0x46464952
    uint32_t ChunkSize;     // RIFF Chunk Size
    uint8_t WAVE[4];        // "WAVE" = 0x45564157
    /* "fmt" sub-chunk */
    uint8_t FmtTag[4];      // "fmt " = 0x20746D66
    uint32_t SubChunk1Size; // Size of the fmt chunk
    uint16_t AudioFormat;   // Audio format: 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t NumOfChan;     // Number of channels: 1=Mono, 2=Sterio
    uint32_t SamplesRate;   // Sampling Frequency (Hz)
    uint32_t BytesRate;     // bytes per second
    uint16_t BlockAlign;    // 2=16-bit mono, 4=16-bit stereo
    uint16_t BitDepth;      // Number of bits per sample
} wavheader;

typedef struct WAV_DATA_CHUNK
{
    uint8_t ID[4]; // "data" = 0x61746164
    uint32_t Size; // Chunk data bytes
} wavchunk;

class WavFile
{
public:
    WavFile(){};
    WavFile(const char *infile_path): infile_path(infile_path) {};
    void printHeaderInfo();
    void readAudioData(vector<int16_t> &data);
    void writeAudioData(vector<int16_t> &data);

    int file_size;
    wavheader header;
    wavchunk chunk;

private:
    FILE *openWav(const char *infile_path)
    {
        FILE *wav_file = fopen(infile_path, "rb");
        if (wav_file == nullptr)
        {
            fprintf(stderr, "Unable to open wave file: %s\n", infile_path);
            exit(1);
        }
        else
        {
            printf("-> Open %s successfully.\n", infile_path);
            setOutfilePath();
        }

        return wav_file;
    }
    void setOutfilePath();
    void setFileSize(FILE *&infile);
    vector<int16_t> removeChannel(vector<int16_t> data);

    const char *infile_path;
    const char *outfile_path;
};

void WavFile::printHeaderInfo()
{
    cout << "\nHeader Infomation: \n";
    cout << "RIFF header                :" << header.RIFF[0] << header.RIFF[1] << header.RIFF[2] << header.RIFF[3] << endl;
    cout << "Data size                  :" << header.ChunkSize << endl;
    cout << "WAVE header                :" << header.WAVE[0] << header.WAVE[1] << header.WAVE[2] << header.WAVE[3] << endl;

    cout << "FMT                        :" << header.FmtTag[0] << header.FmtTag[1] << header.FmtTag[2] << header.FmtTag[3] << endl;
    cout << "FMT Chunk Size             :" << header.SubChunk1Size << endl;
    cout << "Audio Format               :" << header.AudioFormat << endl;
    cout << "Number of channels         :" << header.NumOfChan << endl;
    cout << "Sampling Rate              :" << header.SamplesRate << endl;
    cout << "Number of bytes per second :" << header.BytesRate << endl;
    cout << "Block align                :" << header.BlockAlign << endl;
    cout << "Bits per Sample            :" << header.BitDepth << endl;

    cout << "Data string                :" << chunk.ID[0] << chunk.ID[1] << chunk.ID[2] << chunk.ID[3] << endl;
    cout << "Data length                :" << chunk.Size << endl;
}

void WavFile::setOutfilePath()
{
    size_t len = strlen(infile_path);
    char *out = new char[len + 4];

    strcpy(out, infile_path);
    out[len - 4] = '_'; out[len - 3] = 'o'; out[len - 2] = 'u'; out[len - 1] = 't';
    out[len]     = '.'; out[len + 1] = 'w'; out[len + 2] = 'a'; out[len + 3] = 'v';

    outfile_path = out;
}

void WavFile::setFileSize(FILE *&infile)
{
    fseek(infile, 0, SEEK_END);
    file_size = ftell(infile);
    fseek(infile, 0, SEEK_SET);
}

vector<int16_t> WavFile::removeChannel(vector<int16_t> data)
{
    vector<int16_t> tmp;
    for (size_t i = 0; i < data.size(); i += 2)
        tmp.push_back(data[i]);
    return tmp;
}

void WavFile::readAudioData(vector<int16_t> &data)
{
    FILE *infile = openWav(infile_path);
    setFileSize(infile);

    // read header
    size_t byte_read = fread(&header, 1, sizeof(header), infile);
    printf("Header Read %zu bytes.\n", byte_read);
    if (byte_read != 36)
    {
        fprintf(stderr, "wav header is wrong\n");
        exit(1);
    }

    // read chunk: find data chunk
    while (ftell(infile) < file_size)
    {
        fread(&chunk, 1, sizeof(chunk), infile);
        if (*(unsigned int *)&chunk.ID == 0x61746164)
        {
            printf("Data chunk found.\n");
            break;
        }
        // skip chunk data bytes
        fseek(infile, chunk.Size, SEEK_CUR);
    }
    if (chunk.ID == nullptr)
    {
        fprintf(stderr, "Data chunk is missing\n");
        exit(1);
    }

    // read data
    const uint16_t sample_size = header.BitDepth / 8; // Number of bytes per sample
    const uint16_t buffer_size = chunk.Size * 8 / header.BitDepth;

    int16_t *buffer = new int16_t[buffer_size];
    while ((byte_read = fread(buffer, sample_size, buffer_size, infile)) > 0)
    {
        copy(buffer, buffer + byte_read, std::back_inserter(data));
    }

    if (header.NumOfChan != 1)
        data = removeChannel(data);

    delete[] buffer;
    buffer = nullptr;
    fclose(infile);
}

void WavFile::writeAudioData(vector<int16_t> &data)
{
    FILE *outfile = fopen(outfile_path, "wb");

    // write the header
    header.NumOfChan = 1;
    header.BytesRate = header.NumOfChan * header.SamplesRate * (header.BitDepth / 8);
    header.BlockAlign = header.NumOfChan * (header.BitDepth / 8);
    fwrite(&header, 1, sizeof(header), outfile);
    fwrite(&chunk, 1, sizeof(chunk), outfile);

    // write data
    for (size_t i = 0; i < data.size(); i++)
    {
        fwrite(&data[i], 1, sizeof(int16_t), outfile);
    }

    printf("-> Write into %s successfully.\n", outfile_path);

    fclose(outfile);
}

/*
    size_t fread(void * ptr, size_t size, size_t count, FILE * stream);
        ptr：指向一塊記憶體的指標，該塊記憶體至少要(size * count)個的大小
        size：要讀取的每一個元素大小(單位為 byte)
        count：要讀取的元素數量
        stream：指向 FILE 物件的指標
*/

/*
    int fseek(FILE *stream, long offset, int origin); // 將文件指針移動到指定的地方
        stream: 待移動的 FILE 型指針變量
        offset: 偏移量，每次移動多少個字節
        origin: 指針開始的位置，只可是以下 3 種值
                SEEK_CUR: 文件指針目前的位置
                SEEK_END: 文件末尾處
                SEEK_SET: 文件開始處
        return value: 0 = 成功，1 = 失敗
*/
/*
    long ftell(FILE *stream); // 獲取一個文件指針的當前位置
*/