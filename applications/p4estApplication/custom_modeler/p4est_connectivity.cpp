#include "p4est_connectivity.h"
#include "containers/array_1d.h"
#include "containers/model.h"
#include "includes/data_communicator.h"
#include "includes/model_part.h"
#include "input_output/logger.h"
#include "mpi.h"
#include "mpi/includes/mpi_communicator.h"
#include "processes/tetrahedral_mesh_orientation_check.h"
#include <cassert>
#include <p4est_base.h>
#include <p8est.h>
#include <p8est_vtk.h>
#include <sc.h>

#include <p8est_tets_hexes.h>
#include <sc_containers.h>

namespace Kratos {

P4estConnectivity::P4estConnectivity(ModelPart &modelpart /*, const DataCommunicator& rDataComm */)
{
    KRATOS_INFO("") << " Hello from P4estConnectivity " << std::endl;

    int is_mpi_init;
    MPI_Initialized(&is_mpi_init);

    if (!is_mpi_init) {
        KRATOS_INFO("") << " MPI is not initialized " << std::endl;
        return;
    }

    auto comm = modelpart.GetCommunicator();
    MPI_Comm mpicomm = MPIDataCommunicator::GetMPICommunicator(comm.GetDataCommunicator());

    typedef ModelPart::MeshType Mesh;

    const Mesh mesh = modelpart.GetMesh();
    p8est_tets_t *ptg;
    ptg = P4EST_ALLOC (p8est_tets_t, 1);

    Mesh::NodesContainerType nodes = mesh.Nodes();

    KRATOS_INFO("") << "Number of nodes: " << nodes.size() << std::endl;
    ptg->nodes = sc_array_new_count(sizeof(double), 3*nodes.size());

    std::map<Mesh::IndexType, size_t> idIndexMap;

    // KRATOS_INFO("") << nodes.size() << '\n';
    size_t iz = 0;
    double *pc;
    for (auto nd=nodes.cbegin(); nd < nodes.cend(); ++nd, ++iz) {
      pc = (double *) sc_array_index(ptg->nodes, 3 * iz);
      pc[0] = nd->X();
      pc[1] = nd->Y();
      pc[2] = nd->Z();

      KRATOS_INFO("") << nd->Id() << " -> " << iz << '\n';
      idIndexMap[nd->Id()] = iz;
    }

    Mesh::ElementsContainerType elements = mesh.Elements();
    KRATOS_INFO("") << "Number of elements: " << elements.size() << std::endl;

    ptg->tets = sc_array_new_count(sizeof(p4est_topidx_t), 4*elements.size());

    p4est_topidx_t *pt;
    iz = 0;
    for (auto elm = elements.cbegin(); elm != elements.cend(); ++elm, ++iz) {
        pt = (p4est_topidx_t *) sc_array_index(ptg->tets, 4 * iz);
        Geometry<Node >& geo = elm->GetGeometry();
        KRATOS_ERROR_IF_NOT(geo.size() == 4);

        pt[0] = idIndexMap[geo[0].Id()];
        pt[1] = idIndexMap[geo[1].Id()];
        pt[2] = idIndexMap[geo[2].Id()];
        pt[3] = idIndexMap[geo[3].Id()];

        KRATOS_INFO("") << "tets[" << iz << "] = " <<  "["
            << pt[0] << "," << pt[1] << ","
            << pt[2] << "," << pt[3] << "]\n";
    }

    KRATOS_INFO("") << "Elements loaded\n";

    ptg->tet_attributes = NULL;

    int tnum_flips = p8est_tets_make_righthanded(ptg);
    m_p8est_connectivity = p8est_connectivity_new_tets(ptg);
    p8est_tets_destroy(ptg);

    m_p8est = p8est_new(mpicomm, m_p8est_connectivity, 0, NULL, NULL);

    KRATOS_INFO("") << "ptg Destroyed\n";
}

void P4estConnectivity::VTKWriteFile(std::string filepath)
{
    p8est_vtk_write_file(m_p8est, NULL, filepath.c_str());
}

P4estConnectivity::~P4estConnectivity() {
    if (m_p8est)
      p8est_destroy(m_p8est);

    if (m_p8est_connectivity)
        p8est_connectivity_destroy(m_p8est_connectivity);
    if (m_p4est_connectivity)
        p4est_connectivity_destroy(m_p4est_connectivity);
}

} // namespace Kratos
