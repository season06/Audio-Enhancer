void scale(complex data[], WavFile wav_file, uint64_t N, const char *mode, int freq)
{
    double points_per_freq = (double)N / (double)wav_file.header.SamplesRate;

    double target_idx = points_per_freq * freq; // traget freq
    double target_idx_n = N - (int)target_idx;

    int start = (int)target_idx - 100;
    int end = (int)target_idx + 100;
    int start_n = (int)target_idx_n - 100;
    int end_n = (int)target_idx_n + 100;

    if (strcmp(mode, "gain") == 0)
    {
        for (int i = start; i < end; i++)
            toDouble(data[i]);
        for (int i = start_n; i < end_n; i++)
            toDouble(data[i]);
    }
    else if (strcmp(mode, "debuff") == 0)
    {
        for (int i = start; i < end; i++)
            toZero(data[i]);
        for (int i = start_n; i < end_n; i++)
            toZero(data[i]);
    }
}