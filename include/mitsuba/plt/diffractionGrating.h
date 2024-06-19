#pragma once

#include <mitsuba/core/vector.h>
#include <mitsuba/core/math.h>
#include <mitsuba/core/spectrum.h>

#include <mitsuba/plt/plt.h>

NAMESPACE_BEGIN(mitsuba)

enum class DiffractionGratingType : uint32_t {
    Sinusoidal  = 0x00,
    Rectangular = 0x01,
    Linear      = 0x02,
    Radial      = 0x10,

    TypeMask    = 0xF,
};

MI_DECLARE_ENUM_OPERATORS(DiffractionGratingType)

static const int diffractionGratingsMaxLobes = 8;

template <typename Float, typename Spectrum>
class DiffractionGrating {
public:
    MI_IMPORT_TYPES()

    DiffractionGrating(const Float grating_angle, const Vector2f& inv_period, const Float q, 
        const uint32_t lobes, const DiffractionGratingType type, const Float multiplier = 1.0f,
        Vector2f uv = Vector2f(0.0f))
        : m_invPeriod(inv_period), m_q(q), m_lobe_count(lobes), m_multiplier(multiplier)
    {
        // compute grating direction vector
        if ( m_type & DiffractionGratingType::Radial == 0 )
        {
            // linear grating, direction following grating angle
            m_gratingDir = Vector2f(sin(grating_angle), cos(grating_angle));
        }
        else 
        {
            // radial grating following UV vector + rotation angle
            const Vector2f radial = dr::normalize(uv - Vector2f(0.5f, 0.5f));
            m_gratingDir = Matrix2f(
                 cos(grating_angle), sin(grating_angles),
                -sin(grating_angle), sin(grating_angles)) * Vector2f(radial.x(),-radial.y());
        }
    }

    bool is_1D_grating() { return m_inv_period < dr::Epsilon<Float>; }

    Spectrum alpha(const Vector3f& wi, const Spectrum k)
    {
        Float cos_theta_i = Frame3f::cos_theta(si.wi);
        Spectrum a = dr::sqr(cos_theta_i * m_q * k);

        return dr::exp(-a);
    }

    std::tuple<Vector2i, Vector3f, Vector2f> sampleLobe(Sampler<Float, Spectrum> *sampler, const Vector3f& wi, const Float wl)
    {
        // compute intensity of each lobe;
        Float intensities[16];
        Float total = 0.0f;

        for ( uint32_t l = 0; l < m_lobe_count; ++l ) {
            const Float li = lobe_intensity(Vector2i(l+1, 0), wi, wl);
            total += li;
            intensities[l] = li;
        }

        // TODO
    }

    Float lobe_intensity(const Vector2i& lobe, const Vector3f& wi, const Float wl)
    {
        const DiffractionGratingType gtype = m_type & DiffractionGratingType::TypeMask;
        const Float a = dr::FourPi<Float> * m_q / ( wl * abs(wi.z()) );
        Float ix, iy;

        switch (gtype)
        {
            case DiffractionGratingType::Sinusoidal:

                break;
            case DiffractionGratingType::Rectangular:
                ix = lobe.x() == 0 ? 1 : dr::sin(a / 2.0f) * sinc<Float>(dr::Pi<Float> * lobe.x() / 2.0f);
                if ( !is_1D_grating() )
                {
                    iy = lobe.y() == 0 ? 1 : dr::sin(a / 2.0f) * sinc<Float>(dr::Pi<Float> * lobe.y() / 2.0f);
                }
                else {
                    iy = ix;
                }
                break;
            default:
                ix = lobe.x() == 0 ? 1 : 1.f / dr::sqrt(dr::abs(lobe.x()));
                if ( !is_1D_grating() )
                {
                    iy = lobe.y() == 0 ? 1 : 1.f / dr::sqrt(dr::abs(lobe.y()));
                }
                else {
                    iy = ix;
                }
                break;
        }
        return m_multiplier * ix * iy;
    }
    
private:

    /// \brief The normalized direction of the grating in local coordinate space.
    Vector2f m_grating_dir;

    /// \brief The inverse period of the grating
    Float m_inv_period;

    /// @brief Height scale of the grating
    Float m_q;

    /// @brief Number of diffraction lobes
    uint32_t m_lobe_count = 5;
    DiffractionGratingType m_type = DiffractionGratingType::Sinusoidal;

    Float m_multiplier = 1.0f;
};

NAMESPACE_END(mitsuba)