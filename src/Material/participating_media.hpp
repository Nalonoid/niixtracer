#ifndef __PARTICIPATING_MEDIA_HPP__
#define __PARTICIPATING_MEDIA_HPP__

#include "dielectric.hpp"

class ParticipatingMedia : public Dielectric
{
public:
    ParticipatingMedia(float ior = 1.5f,    float roughness = 0.0f,
                       Vec3d sca = Vec3d(), Vec3d absorb = Vec3d(),
                       Vec3d hg = Vec3d(),
                       std::string name = "participating_media",
                       const Spectrum<> *reflectance = new ConstantSPD<>());

private:
    /* Participating media properties
     * See paper "Acquiring Scattering Properties of Participating Media by
     * Dilution", by Srinivasa G. Narasimhan & al. */

    /* These coefficients are represented as Vec3d because they are given
     * according to the three RGB color channel */
    Vec3d scattering;       // Scattering coefficient (Photons that are scattered outside a beam of light)
    Vec3d absorption;       // Absorption coefficient (How many photons are absorbed by the media)
    Vec3d attenuation;      // Attenuation coefficient : att = absorb + scattr (aka. Extinction coefficient)

    /* The g parameter of the H-G phase function that ranges from backward
     * to forward scattering, through isotropic scattering. -1 <= g <= 1 */
    Vec3d henyey_greenstein;
};

#endif
