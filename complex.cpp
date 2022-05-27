#include <vector>

typedef struct Complex
{
    double re, im;
    Complex() : re(0), im(0) {};
    Complex(double a, double b) : re(a), im(b) {};
} complex;

complex* initialize(vector<double> real, vector<double> imag)
{
    complex *data = new complex[real.size()];
    for (int i = 0; i < real.size(); i++)
    {
        data[i].re = real[i];
        data[i].im = imag[i];
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
