#pragma once

#include <drjit/struct.h>
#include <mitsuba/core/vector.h>
#include <mitsuba/core/math.h>
#include <mitsuba/core/spectrum.h>

NAMESPACE_BEGIN(mitsuba)

template <typename Float, typename Spectrum>
class Coherence {
public:
    MI_IMPORT_TYPES()

    /**
     * \brief Initialize a new coherence object with isotropic diffusivity.
     * 
     * \param diffusivity The isotropic diffusivity 
     * \param l The current optical path length
     */
    Coherence(Float diffusivity, Float l)
        : D(diffusivity, 0.f, 0.f, diffusivity), 
          opl(l)
    {

    }

    /**
     * \brief Initialize a new coherence object with anisotropic diffusivity.
     * 
     * \param diffm The anisotropic diffusivity matrix
     * \param l The current optical path length
     */
    Coherence(Matrix2f diffm, Float l)
        : D(diffm),
          opl(l)
    {}

    /**
     * \brief Return the distance travelled from source in millimeters
     * 
     * \return Float opl * 1e3f
     */
    inline Float rmm() const { return opl * 1e+3f; }

    void propagate(Float rd) {
        opl += rd;
    }

    /**
     * \brief Return the inverse of coherence matrix
     * See eq. 41 in 'A Generalized Ray formulation for wave optics rendering'
     * 
     * \param k The wavenumber
     * \return Matrix2f The inverse coherence matrix
     */
    Matrix2f inv_coherence_matrix(Float k) const {
        return (k / (dr::TwoPi<Float> * rmm())) * D;
    }

    /**
     * \brief Return the inverse of coherence matrix
     * See eq. 41 in 'A Generalized Ray formulation for wave optics rendering'
     * 
     * \return Matrix2f The inverse coherence matrix
     */
    Matrix2f inv_coherence_matrix() const {
        return (1.f / rmm()) * D;
    }

    /**
     * \brief Return the determinant of the inverse of coherence matrix
     * 
     * \param k The wavenumber
     * \return Matrix2f The inverse coherence matrix's determinant
     */
    Matrix2f inv_coherence_det(Float k) const {
        return dr::det(inv_coherence_matrix(k));
    }

    /**
     * \brief Return the determinant of the inverse of coherence matrix
     * 
     * \param k The wavenumber
     * \return Matrix2f The inverse coherence matrix's determinant
     */
    Matrix2f inv_coherence_det() const {
        return dr::det(inv_coherence_matrix());
    }

    void transform(Matrix2f mat) {
        D = dr::transpose(mat) * D * mat;
    }

    /**
     * \brief Diffusivity matrix, which characterizes the WDF's angular variance
     * from the mean propagation direction.
     */
    Matrix2f D;

    // Distance travelled from source in meters
    Float opl;

    DRJIT_STRUCT(Coherence, D, opl)
};

template <typename Float, typename Spectrum>
struct GeneralizedRadiance {

    /**
     * \brief The four vectors representing the Generalized Stokes Parameters
     * (GSp). It serves as a compact representation of the wave packet's state
     */

    /// \brief Intensity of the packet.
    Spectrum L;
    
    /// \brief Polarization state of the packet
    Spectrum L1, L2, L3;

    /**
     * \brief Coherence of the wave packet, which stores the spatial extent in
     * XY in a local frame and the coherence length of the packet. It is stored 
     * in a compact way as a **shape matrix**
     */
    Coherence<Float, Spectrum> coherence;

    GeneralizedRadiance(const Spectrum L_) 
        : L(L_), L1(0.f), L2(0.f), L3(0.f) {} 

    DRJIT_STRUCT(GeneralizedRadiance, L, L1, L2, L3, coherence)
};


/**
 * \brief Simple n-dimensional ray segment data structure
 *
 * Along with the ray origin and direction, this data structure additionally
 * stores a maximum ray position \c maxt, a time value \c time as well a the
 * wavelength information associated with the ray.
 */
template <typename Point_, typename Spectrum_> 
struct GeneralizedRay : Ray<Point_, Spectrum_> {
    using Base = Ray<Point_, Spectrum_>;

    MI_USING_TYPES(Float, Point, Vector, Wavelength)
    MI_USING_MEMBERS(o, d, maxt, time, wavelengths)

    Coherence<Float, Spectrum> coherence;

    GeneralizedRay(const Base &ray) : Base(ray) {}

    /**
     * \brief Apply the propagation operator (Eq. 13) to the generalized ray
     * 
     * \param d The propagation distance.
     */
    GeneralizedRay propagate(const Float d) {
        throw NotImplementedError("propagate");
    }

    DRJIT_STRUCT(GeneralizedRay, o, d, maxt, time, wavelengths, coherence)
};

NAMESPACE_END(mitsuba)