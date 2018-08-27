#include "spectrum.hpp"

namespace Spectra
{

const ConstantSPD<>     *CONSTANT_SPD   { new ConstantSPD<>()  };

const NormalSPD<>       *NORMAL_RED     { new NormalSPD<>(680) };
const NormalSPD<>       *NORMAL_ORANGE  { new NormalSPD<>(620) };
const NormalSPD<>       *NORMAL_YELLOW  { new NormalSPD<>(600) };
const NormalSPD<>       *NORMAL_GREEN   { new NormalSPD<>(545) };
const NormalSPD<>       *NORMAL_BLUE    { new NormalSPD<>(460) };
const NormalSPD<>       *NORMAL_PURPLE  { new NormalSPD<>(420) };
const NormalSPD<>       *NORMAL_UV      { new NormalSPD<>(360) };

const BlackBodySPD<>    *BLACK_BODY_A   { new BlackBodySPD<>(2856) }; // Incandescent / Tungstene
const BlackBodySPD<>    *BLACK_BODY_D50 { new BlackBodySPD<>(5003) }; // Horizon daylight
const BlackBodySPD<>    *BLACK_BODY_D55 { new BlackBodySPD<>(5503) }; // Mid-morning / mid-afternoon daylight
const BlackBodySPD<>    *BLACK_BODY_D65 { new BlackBodySPD<>(6504) }; // Noon daylight
const BlackBodySPD<>    *BLACK_BODY_D75 { new BlackBodySPD<>(7504) }; // Northern sky daylight

const BlackBodySPD<>    *BLACK_BODY_E   { new BlackBodySPD<>(5454) }; // Equal energy

const BlackLight<>      *BLACK_LIGHT    { new BlackLight<>() }; // Black Light (peak 370nm, width 20nm)

const CauchySkewed<>    *ABSORP_RED     { new CauchySkewed<>(680, 20.f, -10.f) };
const CauchySkewed<>    *ABSORP_ORANGE  { new CauchySkewed<>(620, 20.f, -10.f) };
const CauchySkewed<>    *ABSORP_YELLOW  { new CauchySkewed<>(600, 20.f, -10.f) };
const CauchySkewed<>    *ABSORP_GREEN   { new CauchySkewed<>(545, 20.f, -10.f) };
const CauchySkewed<>    *ABSORP_BLUE    { new CauchySkewed<>(460, 20.f, -10.f) };
const CauchySkewed<>    *ABSORP_PURPLE  { new CauchySkewed<>(420, 20.f, -10.f) };
const CauchySkewed<>    *ABSORP_UV      { new CauchySkewed<>(370, 20.f, -10.f) };

const Spectrum<>* spectrum(std::string name)
{
    // Normal Spectra
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

    if (name == "uv")
        return Spectra::NORMAL_UV;

    // Absorption Spectra
    if (name == "ared")
        return Spectra::ABSORP_RED;

    if (name == "aorange")
        return Spectra::ABSORP_ORANGE;

    if (name == "ayellow")
        return Spectra::ABSORP_YELLOW;

    if (name == "agreen")
        return Spectra::ABSORP_GREEN;

    if (name == "ablue")
        return Spectra::ABSORP_BLUE;

    if (name == "apurple")
        return Spectra::ABSORP_PURPLE;

    if (name == "auv")
        return Spectra::ABSORP_UV;

    // Emission Spectra (Black Body)
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

    if (name == "bl")
        return Spectra::BLACK_LIGHT;

    // Constant Spectra, all samples at 1.0f
    if (name == "const")
        return Spectra::CONSTANT_SPD;

    if (name == "null")
        return new ConstantSPD<>(0.0f);

    /* If we arrive at this point, maybe the emission spectrum passed in was
     * just a scalar emission term (i.e. a constant SPD) */
    return nullptr;
}

}
