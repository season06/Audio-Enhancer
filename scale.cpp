void scale(complex data[], WavFile &wav_file, const char *mode, int freq)
{
    int file_size = wav_file.getFileSize();
    int rate = wav_file.getSampleRate();
    double points_per_freq = ((double)file_size - 44) / 2 / (double)rate;
    double target_idx = points_per_freq * freq; // traget freq

    int start = (int)target_idx - 100;
    int end = (int)target_idx + 100;

    if (strcmp(mode, "gain") == 0)
    {
        for (int i = start; i < end; i++)
            toDouble(data[i]);
    }
    else if (strcmp(mode, "debuff") == 0)
    {
        for (int i = start; i < end; i++)
            toZero(data[i]);
    }
}