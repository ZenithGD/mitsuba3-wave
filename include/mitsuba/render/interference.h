#pragma once

#include <mitsuba/core/fwd.h>

#include <mitsuba/render/plt.h>

NAMESPACE_BEGIN(mitsuba)

template <typename Float, typename Spectrum>
Float phase_shift(const Float eta, const Float dist, const Float k, const dr::Complex<Float>& r) {
    const Float carg = arg(r);

    return sin(dist * k * eta + carg);
}

NAMESPACE_END(mitsuba)