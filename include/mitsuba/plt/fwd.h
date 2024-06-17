#pragma once

#include <mitsuba/core/fwd.h>
// #include <mitsuba/core/spectrum.h>
#include <mitsuba/core/traits.h>

NAMESPACE_BEGIN(mitsuba)

struct BSDFContext;
template <typename Float, typename Spectrum> class PLTInteraction;

template <typename Float_, typename Spectrum_> struct RenderAliases {
    using PLTInteraction3f = PLTInteraction<Float, Spectrum>;
};

NAMESPACE_END(mitsuba)