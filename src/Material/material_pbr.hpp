#ifndef __MATERIAL_BRDF_HPP__
#define __MATERIAL_BRDF_HPP__

#include "Utils/utils.hpp"
#include "Raytracer/spectrum.hpp"

class Intersection;
class BRDF;
class Fluorescence;

class MaterialPBR
{
public:
    MaterialPBR(const Spectrum<> *reflectance, std::string name,
                const Fluorescence *fluorescence = nullptr);

    // Getters
    const std::string& name() const;
    bool fluorescent() const;
    const Fluorescence* fluorescence() const;
    virtual float roughness() const = 0;

    // Methods
    virtual Vec3d wi(const Vec3d &wo, Vec3d &normal) const = 0;

    // BRDF's Probability Density Function given a sampled incident direction
    virtual float pdf(const Vec3d &wi, const Vec3d &wo,
                      const Intersection &i) const = 0;

    float reflectance(const Vec3d &wi, const Vec3d &wo, const Intersection &i,
                      const unsigned wavelength = AVG_WAVELENGTH) const;

    void set_reflectance(const Spectrum<> *reflectance_spctr);
    void set_fluorescence(const Fluorescence *fluo);

private:
    const Spectrum<> *_reflectance;
    std::string _name;
    const Fluorescence *_fluorescence;
};

class Matte : public MaterialPBR
{
public:
    Matte();
    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;
    float pdf(const Vec3d &wi, const Vec3d &wo,
              const Intersection &i) const override;

    float roughness() const override;
};

namespace MaterialsPBR
{

MaterialPBR* material(std::string name);

}

#endif
