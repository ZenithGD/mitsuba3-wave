#pragma once

#include <mitsuba/core/vector.h>
#include <mitsuba/core/ray.h>
#include <mitsuba/core/bsphere.h>

#include <mitsuba/render/interaction.h>

NAMESPACE_BEGIN(mitsuba)

/**
 * \brief Structure that stores once bounce and the ray from which the interaction
 * took place.
 */
template <typename Float_, typename Spectrum_>
struct BounceData {
    using Float    = Float_;
    using Spectrum = Spectrum_;
    MI_IMPORT_RENDER_BASIC_TYPES()
    MI_IMPORT_OBJECT_TYPES()
        
    Interaction<Float, Spectrum> interaction;
    const Vector3f& wi;
    const BSDFFlags& bsdf_flags;
    Float path_contrib;
    Float last_nondelta_pdf;
};

/**
 * \brief Structure that stores the bounces from a ray traced through the scene.
 * Works as a conventional stack.
 */
template <typename Float_, typename Spectrum_>
class BounceBuffer {
public:

    using Float    = Float_;
    using Spectrum = Spectrum_;
    using BounceDataTp = BounceData<Float, Spectrum>;

    MI_IMPORT_RENDER_BASIC_TYPES()
    MI_IMPORT_OBJECT_TYPES()

    /**
     * \brief Create a new bounce buffer. 
     * 
     * \param sensor_origin The origin of the backwards-traced ray.
     */
    BounceBuffer(const Vector3f& sensor_origin) 
        : m_sensor_origin(sensor_origin), m_bdata()
    {}

    /**
     * \brief Read the last bounce data. 
     * 
     * \return BounceData& 
     */
    BounceDataTp& read_last_bounce() const {
        return m_bdata.at(m_bdata.end());
    }

    /**
     * \brief Pop one bounce from the stack.
     */
    void pop_bounce() {
        m_bdata.pop_back();
    }

    /**
     * \brief Add one bounce data into the buffer.
     * 
     * \param it The interaction with the scene (which can be an empty interaction.)
     * \param r 
     */
    void push_bounce(
        const Interaction<Float, Spectrum>& it, 
        const Vector3f& wi, 
        const BSDFFlags& bsdf_flags,
        const Float contrib,
        const Float ndpdf) {

        BounceDataTp bd;
        bd.interaction = it;
        bd.wi = wi;
        bd.bsdf_flags = bsdf_flags;
        bd.path_contrib = contrib;
        bd.last_nondelta_pdf = ndpdf;

        m_bdata.push_back(bd);
    }
    
private:
    std::vector<BounceDataTp> m_bdata;
    Vector3f m_sensor_origin;
};

NAMESPACE_END(mitsuba)
