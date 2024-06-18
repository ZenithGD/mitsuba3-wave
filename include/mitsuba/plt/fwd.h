#pragma once

#include <mitsuba/core/fwd.h>
// #include <mitsuba/core/spectrum.h>
#include <mitsuba/core/traits.h>

NAMESPACE_BEGIN(mitsuba)

// template <typename Float, typename Spectrum> struct PLTInteraction;
template <typename Float, typename Spectrum> struct GeneralizedRadiance;
template <typename Float, typename Spectrum> class Coherence;

template <typename Float_, typename Spectrum_> struct PLTAliases {
    using Float                     = Float_;
    using Spectrum                  = Spectrum_;

    // using PLTInteraction3f      = PLTInteraction<Float, Spectrum>;
    using Coherence3f           = Coherence<Float, Spectrum>;
    using GeneralizedRadiance3f = GeneralizedRadiance<Float, Spectrum>;
};


#define MI_IMPORT_PLT_BASIC_TYPES()                                            \
    MI_IMPORT_CORE_TYPES()                                                     \
    using PLTAliases            = mitsuba::PLTAliases<Float, Spectrum>;        \
    using Coherence3f           = typename PLTAliases::Coherence3f;            \
    using GeneralizedRadiance3f = typename PLTAliases::GeneralizedRadiance3f;                   

#define MI_IMPORT_PLT_TYPES_MACRO(x) using x = typename PLTAliases::x;

#define MI_IMPORT_PLT_TYPES(...)                         \
    MI_IMPORT_PLT_BASIC_TYPES()                          \
    DRJIT_MAP(MI_IMPORT_PLT_TYPES_MACRO, __VA_ARGS__)


NAMESPACE_END(mitsuba)