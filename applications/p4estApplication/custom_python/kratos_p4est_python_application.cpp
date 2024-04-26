// System includes

#if defined(KRATOS_PYTHON)
// External includes
#include <pybind11/pybind11.h>


// Project includes
#include "p4est_application.h"
// #include "custom_python/add_processes_to_python.h"

#include "custom_python/add_p4est_to_python.h"


namespace Kratos::Python {

PYBIND11_MODULE(Kratosp4estApplication,m)
{
    namespace py = pybind11;

    m.def("Initialize", &P4estInitialize );
    m.def("Finalize", &P4estFinalize );

    py::class_<Kratosp4estApplication,
        Kratosp4estApplication::Pointer,
        KratosApplication >(m,"Kratosp4estApplication")
        .def(py::init<>())
        // .def("Initialize", &Kratosp4estApplication::Initialize)
        // .def("Finalize", &Kratosp4estApplication::Finalize)
        ;

    AddP4estToPython(m);
}

}  // namespace Kratos::Python.

#endif // KRATOS_PYTHON defined
