
#include "p4est_connectivity.h"
#include "includes/data_communicator.h"
#include "includes/model_part.h"
#include "mpi.h"
#include "mpi/includes/mpi_communicator.h"
#include <p4est_base.h>
#include <sc.h>

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


    m_connectivity = p4est_connectivity_new_unitsquare();
}

P4estConnectivity::~P4estConnectivity() { 
  p4est_connectivity_destroy(m_connectivity);
}


} // namespace Kratos
    
