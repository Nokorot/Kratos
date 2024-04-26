//
//   Project Name:        Kratos
//   Last Modified by:    $Author: Tor Gjone $
//   Date:                $Date: 2008-07-09 15:09:17 $
//   Revision:            $Revision: 1.2 $
//
//

// System includes

// External includes
#include <p4est.h>
#include <sc.h>
// #include <sc_mpi.h>

#include "includes/define.h"
#include "input_output/logger.h"
#include "mpi/includes/mpi_data_communicator.h"

// Project includes
#include "p4est_application.h"

struct MPIContext {
    sc_MPI_Comm mpicomm;
    int mpisize;
    int mpirank;
};

namespace Kratos {

void P4estInitialize(const DataCommunicator& rDataComm)
{
    if (sc_is_initialized()) {
      KRATOS_THROW_ERROR(std::runtime_error, "p4est (libsc) was already initialized", "");
    }

    int mpi_is_initialized = 0;
    int mpisize, mpirank;

// #ifdef USE_MPI

    MPI_Comm mpicomm = MPIDataCommunicator::GetMPICommunicator(rDataComm);
    MPI_Initialized(&mpi_is_initialized);

    if (!mpi_is_initialized)
    {
        KRATOS_INFO("") << " MPI NOT Initialized  " << std::endl;
      
        int argc = 0;
        char** argv = nullptr;

        sc_MPI_Init(argc, argv);
        // return;
    }

    sc_MPI_Comm_size (mpicomm, &mpisize);
    sc_MPI_Comm_rank (mpicomm, &mpirank);
    
    KRATOS_INFO("") << " MPI Size " << mpisize << ", I'm rank  " << mpirank << std::endl;
    
    KRATOS_INFO("") << " Initializing sc and p4est " << std::endl;
    sc_init(mpicomm, 1, 1, NULL, SC_LP_DEFAULT);
    p4est_init(NULL, SC_LP_DEFAULT);
}

void P4estFinalize()
{
    if (sc_is_initialized())
    {
       sc_finalize();
    }
}

Kratosp4estApplication::Kratosp4estApplication()
    : KratosApplication("p4estApplication") {}

Kratosp4estApplication::~Kratosp4estApplication()
{ 
  P4estFinalize(); 
}

void Kratosp4estApplication::Register()
{
    std::stringstream banner;

    banner << "    KRATOS  __   ___           \n"
                    << "           | _ \\/ _ | ___   __  _____\n"
                    << "           |  _/__  |/ -_) ( _ |_   _|\n"
                    << "           |_|    |_|\\___|  __)  |_|\n"
                    << "Initializing Kratosp4estApplication..." << std::endl;

    // KRATOS_INFO("") << banner.str();
}



}  // namespace Kratos.
