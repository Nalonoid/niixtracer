#ifndef __SPECTRUM_HPP__
#define __SPECTRUM_HPP__

template <unsigned nb_samples = 30>
class Spectrum
{
public:
    Spectrum(double sample_value = 0.0);

    Spectrum operator+(const Spectrum &s);
    Spectrum operator-(const Spectrum &s);
    Spectrum operator*(const Spectrum &s);

    double& operator[](int index);

private:
    double _samples[nb_samples];
};


#endif
