#pragma once

// System includes

// Project includes
// #include "processes/graph_coloring_process.h"
// #include "custom_processes/morton_partitioning_process.h"

#include "includes/define.h"
#include "includes/model_part.h"
#include "includes/data_communicator.h"

#include <p8est_bits.h>
#include <p4est_connectivity.h>
// #include <p8est_connectivity.h>

namespace Kratos
{

  class P4estConnectivity
    {
    public:
      ///@name Type Definitions
      ///@{

      /// Pointer definition of P4estConnectivity
      KRATOS_CLASS_POINTER_DEFINITION(P4estConnectivity);


      P4estConnectivity(ModelPart &modelpart /*, const DataCommunicator& rDataComm */);

      // Destructor.
      ~P4estConnectivity();

      void VTKWriteFile(std::string filepath);


    private:
      // DataCommunicator m_DataComm;
      p4est_connectivity_t *m_p4est_connectivity = nullptr;
      p8est_connectivity_t *m_p8est_connectivity = nullptr;

      p8est_t *m_p8est = nullptr;

    };

}
