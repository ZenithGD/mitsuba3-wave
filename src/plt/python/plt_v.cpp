#include <mitsuba/plt/plt.h>
#include <mitsuba/plt/fwd.h>
#include <mitsuba/python/python.h>


MI_PY_EXPORT(Coherence) {
    MI_PY_IMPORT_TYPES()
    MI_IMPORT_PLT_BASIC_TYPES()
    
    auto it = py::class_<Coherence3f>(m, "Coherence3f", D(Coherence))
        // Members
        .def_field(Coherence3f, dmat, D(Coherence, dmat))
        .def_field(Coherence3f, opl, D(Coherence, opl))

        // Methods
        .def(py::init<>(), D(Coherence, Coherence))
        .def(py::init<const Coherence3f &>(), "Copy constructor")
        .def(py::init<Float, Float>(),
             "diffusivity"_a, "l"_a,
             D(Coherence, Coherence, 2))
        .def(py::init<Float, Float>(),
             "diffm"_a, "l"_a,
             D(Coherence, Coherence, 3))
        .def("rmm", &Coherence3f::rmm, D(Coherence, rmm))
        // .def("inv_coherence_matrix",py::overload_cast<Float>(&Coherence3f::inv_coherence_matrix), 
        //     "k"_a, D(Coherence, inv_coherence_matrix))
        // .def("inv_coherence_matrix",py::overload_cast<>(&Coherence3f::inv_coherence_matrix), 
        //     D(Coherence, inv_coherence_matrix, 2))
        .def_repr(Coherence3f);

    MI_PY_DRJIT_STRUCT(it, Coherence3f, dmat, opl)
}

// MI_PY_EXPORT(GeneralizedRadiance) {
//     MI_PY_IMPORT_TYPES()
//     auto it = py::class_<GeneralizedRadiance3f>(m, "GeneralizedRadiance3f", D(GeneralizedRadiance))
//         // Members
//         .def_field(GeneralizedRadiance3f, L, D(GeneralizedRadiance, L))
//         .def_field(GeneralizedRadiance3f, L1, D(GeneralizedRadiance, L1))
//         .def_field(GeneralizedRadiance3f, L2, D(GeneralizedRadiance, L2))
//         .def_field(GeneralizedRadiance3f, L3, D(GeneralizedRadiance, L3))
//         .def_field(Coherence3f, coherence, D(GeneralizedRadiance, coherence))
//         // Methods
//         .def(py::init<>(), D(GeneralizedRadiance, GeneralizedRadiance))
//         .def(py::init<const GeneralizedRadiance3f &>(), "Copy constructor")
//         .def(py::init<Spectrum>(),
//              "L_"_a, D(GeneralizedRadiance, GeneralizedRadiance, 2))
//         .def_repr(GeneralizedRadiance3f);

//     MI_PY_DRJIT_STRUCT(it, GeneralizedRadiance3f, L, L1, L2, L3, coherence)
// }