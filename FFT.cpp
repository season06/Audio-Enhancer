#define PI acos(-1.0)

vector<int16_t> paddingToPowerOfTwo(vector<int16_t> data)
{
    unsigned int n = data.size();

    // if power is 2 then return
    if ((n & (n - 1)) == 0)
        return data;

    // get the next number which power is 2
    unsigned int base_2_num = 1;
    while (base_2_num < n)
        base_2_num <<= 1;

    // padding zero
    data.resize(base_2_num, 0);

    return data;
}

uint64_t reverseBits(uint64_t n, uint64_t bits)
{
    uint64_t result = 0;
    for (uint64_t i = 0; i < bits; i++)
        result = (result << 1) + (n >> i & 1);

    return result;
}

void arrayReorder(complex data[], uint64_t N)
{
    uint64_t x, r = log2(N);
    for (uint64_t i = 0; i < N; ++i)
    {
        x = reverseBits(i, r);
        if (x > i)
        {
            swap(data[i], data[x]);
        }
    }
}

void fft(complex data[], uint64_t N, bool inv = false)
{
    complex wn, deltawn, t, u;
    uint64_t log2n = (N == 0) ? 0 : log2(N);
    int64_t sign = inv ? 1 : -1;

    arrayReorder(data, N);

    for (uint64_t i = 1; i <= log2n; ++i) // logn
    {
        uint64_t m = 1 << i;
        deltawn = complex(cos(2 * PI / m), sign * sin(2 * PI / m));
        for (uint64_t k = 0; k < N; k += m) // this `for` and next `for` add up to O(n)
        {
            wn = complex(1, 0);
            for (uint64_t j = 0; j < m / 2; j++)
            {
                t = mul(data[k + j + m / 2], wn);
                u = data[k + j];
                data[k + j] = add(u, t);
                data[k + j + m / 2] = sub(u, t);
                wn = mul(wn, deltawn);
            }
        }
    }
}

void ifft(complex data[], uint64_t N)
{
    fft(data, N, true);

    for (uint64_t i = 0; i < N; ++i)
        data[i].re /= N;
}
