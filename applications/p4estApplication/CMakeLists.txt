set(CMAKE_INCLUDE_CURRENT_DIR ON)

message("**** configuring Kratosp4estApplication ****")

include(pybind11Tools)

include_directories( ${KRATOS_SOURCE_DIR}/kratos )

# TODO: FindSC.cmake module
set(P4EST_LIBRARIES sc p4est)
set(P4EST_INCLUDE_DIR "/usr/local/include")


set(CMAKE_FIND_PACKAGE_RESOLVE_SYMLINKS TRUE)

FIND_LIBRARY(SC_LIBRARY sc
    /usr/local/lib
    /usr/lib
)

FIND_LIBRARY(P4EST_LIBRARY p4est
    /usr/local/lib
    /usr/lib
)


MESSAGE(STATUS "P4EST library found")
MESSAGE(STATUS "P4EST_INCLUDE_DIR: ${P4EST_INCLUDE_DIR}")
MESSAGE(STATUS "P4EST_LIBRARY: ${P4EST_LIBRARY}")
MESSAGE(STATUS "SC_LIBRARY: ${SC_LIBRARY}")

# find_package(P4EST REQUIRED)
include_directories(${P4EST_INCLUDE_DIR})
# link_directories(${P4EST_LIBRARY}) 
# link_directories("/usr/local/lib") 
# install(FILES ${SC_LIBRARY} DESTINATION libs)
# install(FILES ${P4EST_LIBRARY} DESTINATION libs)
desymlink_install(${SC_LIBRARY} libs)
desymlink_install(${P4EST_LIBRARY} libs)


## p4est Core sources
file( GLOB KRATOS_P4EST_APPLICATION_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/p4est_application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/custom_modeler/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/custom_processes/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/custom_utilities/*.cpp
)

## p4est python interface sources
file(GLOB_RECURSE KRATOS_P4EST_APPLICATION_PYTHON_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/custom_python/*.cpp)

## define test sources (cpp tests)
#   if(${KRATOS_BUILD_TESTING} MATCHES ON)
#    	file(GLOB KRATOS_P4EST_APPLICATION_TESTING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/cpp_tests/*.cpp)
#   endif(${KRATOS_BUILD_TESTING} MATCHES ON)


add_library(Kratosp4estCore SHARED ${KRATOS_P4EST_APPLICATION_CORE})
# add_library(Kratosp4estCore SHARED ${KRATOS_P4EST_APPLICATION_CORE} ${KRATOS_P4EST_APPLICATION_TESTING_SOURCES})
target_link_libraries(Kratosp4estCore PUBLIC KratosCore KratosMPICore  ${P4EST_LIBRARIES})
set_target_properties(Kratosp4estCore PROPERTIES COMPILE_DEFINITIONS "P4EST_APPLICATION=EXPORT,API")

###############################################################
## define library Kratos which defines the basic python interface
pybind11_add_module(Kratosp4estApplication MODULE THIN_LTO ${KRATOS_P4EST_APPLICATION_PYTHON_INTERFACE})
target_link_libraries(Kratosp4estApplication PRIVATE Kratosp4estCore)
set_target_properties(Kratosp4estApplication PROPERTIES PREFIX "")

# Set batch size in the unity build
IF(CMAKE_UNITY_BUILD MATCHES ON)
    set_target_properties(Kratosp4estCore PROPERTIES UNITY_BUILD_BATCH_SIZE ${KRATOS_UNITY_BUILD_BATCH_SIZE})
    set_target_properties(Kratosp4estApplication PROPERTIES UNITY_BUILD_BATCH_SIZE ${KRATOS_UNITY_BUILD_BATCH_SIZE})
ENDIF(CMAKE_UNITY_BUILD MATCHES ON)

# changing the .dll suffix to .pyd
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set_target_properties(Kratosp4estApplication PROPERTIES SUFFIX .pyd)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# changing the .dylib suffix to .so
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_target_properties(Kratosp4estApplication PROPERTIES SUFFIX .so)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Add to the KratosMultiphisics Python module
kratos_python_install(${INSTALL_PYTHON_USING_LINKS} ${CMAKE_CURRENT_SOURCE_DIR}/p4estApplication.py KratosMultiphysics/p4estApplication/__init__.py )

# Install python files
get_filename_component (CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
kratos_python_install_directory(${INSTALL_PYTHON_USING_LINKS} ${CMAKE_CURRENT_SOURCE_DIR}/python_scripts KratosMultiphysics/${CURRENT_DIR_NAME} )

# Kratos Testing. Install everything except sources to ensure that reference and configuration files are copied.
#    if(${INSTALL_TESTING_FILES} MATCHES ON )
#        get_filename_component (CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
#        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests DESTINATION applications/${CURRENT_DIR_NAME}
#            PATTERN "*.git" EXCLUDE
#            PATTERN "*.c" EXCLUDE
#            PATTERN "*.h" EXCLUDE
#            PATTERN "*.cpp" EXCLUDE
#            PATTERN "*.hpp" EXCLUDE
#      )
#    endif(${INSTALL_TESTING_FILES} MATCHES ON)

# Install targets
install(TARGETS Kratosp4estCore DESTINATION libs )
install(TARGETS Kratosp4estApplication DESTINATION libs )

# Define custom targets
set(KRATOS_KERNEL "${KRATOS_KERNEL};Kratosp4estCore" PARENT_SCOPE)
set(KRATOS_PYTHON_INTERFACE "${KRATOS_PYTHON_INTERFACE};Kratosp4estApplication" PARENT_SCOPE)
