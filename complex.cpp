#include <vector>

typedef struct Complex
{
    double re, im;
    Complex() : re(0), im(0) {};
    Complex(double a, double b) : re(a), im(b) {};
} complex;

complex *initialize(vector<int16_t> real)
{
    complex *data = new complex[real.size()];
    for (unsigned int i = 0; i < real.size(); i++)
    {
        data[i].re = (double)real[i];
        data[i].im = 0;
    }
    return data;
}

complex add(complex a, complex b)
{
    complex c;
    c.re = a.re + b.re;
    c.im = a.im + b.im;
    return c;
}

complex sub(complex a, complex b)
{
    complex c;
    c.re = a.re - b.re;
    c.im = a.im - b.im;
    return c;
}

complex mul(complex a, complex b)
{
    complex c;
    c.re = a.re * b.re - a.im * b.im;
    c.im = a.re * b.im + a.im * b.re;
    return c;
}

vector<double> getReals(complex *comp, int N)
{
    vector<double> real(N);
    for (int i = 0; i < N; i++)
        real[i] = comp[i].re;
    return real;
}

vector<double> getImags(complex *comp, int N)
{
    vector<double> imag(N);
    for (int i = 0; i < N; i++)
        imag[i] = comp[i].im;
    return imag;
}