//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//                   Jordi Cotela
//                   Carlos Roig
//


// System includes

// External includes

// Project includes
// #include "includes/parallel_environment.h"

#include "add_p4est_to_python.h"
#include "custom_modeler/p4est_connectivity.h"
#include "pybind11/pytypes.h"

namespace Kratos::Python {

void AddP4estToPython(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<P4estConnectivity, P4estConnectivity::Pointer>(
        m,"P4estConnectivity")
        .def(py::init<ModelPart &>())
        .def("GetConnectivity", &P4estConnectivity::GetConnectivity)
        ;


}

} // Namespace Kratos::Python
