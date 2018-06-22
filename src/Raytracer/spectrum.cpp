#include "spectrum.hpp"

namespace Spectra
{

const ConstantSPD<>     *CONSTANT_SPD   { new ConstantSPD<>()  };

const NormalSPD<>       *NORMAL_RED     { new NormalSPD<>(680) };
const NormalSPD<>       *NORMAL_ORANGE  { new NormalSPD<>(620) };
const NormalSPD<>       *NORMAL_YELLOW  { new NormalSPD<>(590) };
const NormalSPD<>       *NORMAL_GREEN   { new NormalSPD<>(540) };
const NormalSPD<>       *NORMAL_BLUE    { new NormalSPD<>(490) };
const NormalSPD<>       *NORMAL_PURPLE  { new NormalSPD<>(430) };

const BlackBodySPD<>    *BLACK_BODY_A   { new BlackBodySPD<>(2856) }; // Incandescent / Tungstene
const BlackBodySPD<>    *BLACK_BODY_D50 { new BlackBodySPD<>(5003) }; // Horizon daylight
const BlackBodySPD<>    *BLACK_BODY_D55 { new BlackBodySPD<>(5503) }; // mid-morning / mid-afternoon daylight
const BlackBodySPD<>    *BLACK_BODY_D65 { new BlackBodySPD<>(6504) }; // Noon daylight
const BlackBodySPD<>    *BLACK_BODY_D75 { new BlackBodySPD<>(7504) }; // North sky daylight

const BlackBodySPD<>    *BLACK_BODY_E   { new BlackBodySPD<>(5454) }; // Equal energy

const Spectrum<>* spectrum(std::string name)
{
    if (name == "red")
        return Spectra::NORMAL_RED;

    if (name == "orange")
        return Spectra::NORMAL_ORANGE;

    if (name == "yellow")
        return Spectra::NORMAL_YELLOW;

    if (name == "green")
        return Spectra::NORMAL_GREEN;

    if (name == "blue")
        return Spectra::NORMAL_BLUE;

    if (name == "purple")
        return Spectra::NORMAL_PURPLE;

    if (name == "bba")
        return Spectra::BLACK_BODY_A;

    if (name == "bbd50")
        return Spectra::BLACK_BODY_D50;

    if (name == "bbd55")
        return Spectra::BLACK_BODY_D55;

    if (name == "bbd65")
        return Spectra::BLACK_BODY_D65;

    if (name == "bbd75")
        return Spectra::BLACK_BODY_D75;

    if (name == "const")
        return Spectra::CONSTANT_SPD;

    /* If we arrive at this point, maybe the emission spectrum passed in was
     * just a scalar emission term (i.e. a constant SPD) */
    return nullptr;
}

}
