# Produces dependencies of Klampt
# Given KLAMPT_ROOT (optional)
# Produces
# - KLAMPT_LIBRARIES
# - KLAMPT_INCLUDE_DIRS
# - KLAMPT_DEFINITIONS
#

IF(NOT KLAMPT_ROOT)
  MESSAGE("KLAMPT_ROOT not defined, setting to .")
  SET(KLAMPT_ROOT .)
ENDIF( )

SET(KLAMPT_CPP_ROOT ${KLAMPT_ROOT}/Cpp)
SET(KLAMPT_PYTHON_ROOT ${KLAMPT_ROOT}/Python)
SET(KLAMPT_DEPENDENCIES "${KLAMPT_CPP_ROOT}/Dependencies" CACHE PATH "Klamp't C++ dependencies" FORCE)

IF(WIN32)
  #Assume binaries for KrisLibrary, ODE, GLPK, GLEW, tinyxml are in the library
  #directory as given by the prebuilt binary package.
  #Assume ode is double
  SET(KRISLIBRARY_ROOT ${KLAMPT_DEPENDENCIES})
  SET(KLAMPT_DEFINITIONS -DHAVE_GLUI=0 -DHAVE_GLUT=0 -DHAVE_TIXML=1 -DTIXML_USE_STL -DdDOUBLE -DNOMINMAX -DHAVE_GLEW=1)

  include(FindPackageHandleStandardArgs)
  FIND_PATH(KRISLIBRARY_INCLUDE_DIR KrisLibrary/myfile.h
    PATHS ${KRISLIBRARY_ROOT}  )
  FIND_LIBRARY(KRISLIBRARY_LIBRARY_DEBUG 
	NAMES KrisLibraryd
	PATHS ${KRISLIBRARY_ROOT})
  FIND_LIBRARY(KRISLIBRARY_LIBRARY_RELEASE
	NAMES KrisLibrary
	PATHS ${KRISLIBRARY_ROOT})
  find_package_handle_standard_args(KRISLIBRARY
	DEFAULT_MSG
	KRISLIBRARY_INCLUDE_DIR
	KRISLIBRARY_LIBRARY_DEBUG
	KRISLIBRARY_LIBRARY_RELEASE)
  if(NOT KRISLIBRARY_FOUND)
    MESSAGE("KrisLibrary not found!")
  endif( )

  FIND_PATH(ODE_INCLUDE_DIR ode/ode.h
    PATHS ${KRISLIBRARY_ROOT}/ode-0.14/include  )
  FIND_LIBRARY(ODE_LIBRARY_DEBUG 
	NAMES ode_doubled
	PATHS ${KRISLIBRARY_ROOT})
  FIND_LIBRARY(ODE_LIBRARY_RELEASE
	NAMES ode_double
	PATHS ${KRISLIBRARY_ROOT})
  find_package_handle_standard_args(ODE
	DEFAULT_MSG
	ODE_INCLUDE_DIR
	ODE_LIBRARY_DEBUG
	ODE_LIBRARY_RELEASE)
  if(NOT ODE_FOUND)
    MESSAGE("ODE not found!")
  endif( )

  FIND_PATH(GLPK_INCLUDE_DIR glpk.h
	PATHS ${KRISLIBRARY_ROOT}/glpk-4.61/src  )
  FIND_LIBRARY(GLPK_LIBRARY
	NAMES glpk_4_61
	PATHS ${KRISLIBRARY_ROOT})
  find_package_handle_standard_args(GLPK
	DEFAULT_MSG
	GLPK_INCLUDE_DIR
	GLPK_LIBRARY)
  if(NOT GLPK_FOUND)
    MESSAGE("GLPK not found!")
  endif( )

  FIND_PATH(TINYXML_INCLUDE_DIR tinyxml.h
	PATHS ${KRISLIBRARY_ROOT}/tinyxml )
  FIND_LIBRARY(TINYXML_LIBRARY_DEBUG
	NAMES tinyxmld_STL
	PATHS ${KRISLIBRARY_ROOT})
  FIND_LIBRARY(TINYXML_LIBRARY_RELEASE
	NAMES tinyxml_STL
	PATHS ${KRISLIBRARY_ROOT})
  find_package_handle_standard_args(TINYXML
	DEFAULT_MSG
	TINYXML_INCLUDE_DIR
	TINYXML_LIBRARY_DEBUG
	TINYXML_LIBRARY_RELEASE)
  if(NOT TINYXML_FOUND)
    MESSAGE("TinyXML not found!")
  endif( )

  SET(KLAMPT_LIBRARIES debug ${KRISLIBRARY_LIBRARY_DEBUG} optimized ${KRISLIBRARY_LIBRARY_RELEASE} debug ${ODE_LIBRARY_DEBUG} optimized ${ODE_LIBRARY_RELEASE} ${GLPK_LIBRARY} debug ${TINYXML_LIBRARY_DEBUG} optimized ${TINYXML_LIBRARY_RELEASE} ${KRISLIBRARY_ROOT}/glut32.lib opengl32 Ws2_32 winmm Gdiplus )
  SET(KLAMPT_INCLUDE_DIRS ${KRISLIBRARY_INCLUDE_DIR} ${ODE_INCLUDE_DIR} ${GLPK_INCLUDE_DIR}  ${TINYXML_INCLUDE_DIR})

  #GL Extension Wrangler package (GLEW)
  SET(GLEW_INCLUDE_DIR ${KRISLIBRARY_ROOT}/glew-2.0.0/include CACHE PATH "GLEW include directory" FORCE)
  SET(GLEW_LIB ${KRISLIBRARY_ROOT}/glew32.lib CACHE PATH "GLEW library" FORCE)
  SET(KLAMPT_INCLUDE_DIRS ${KLAMPT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${KRISLIBRARY_ROOT}/log4cxx/include)
  SET(KLAMPT_LIBRARIES ${KLAMPT_LIBRARIES} ${GLEW_LIB})

  #Optional: Assimp package
  IF(NOT ASSIMP_ROOT)
    SET(ASSIMP_ROOT "${KRISLIBRARY_ROOT}/assimp--3.0.1270-sdk" 
       CACHE PATH
       "Root of Assimp package"
       FORCE
    )
  ENDIF(NOT ASSIMP_ROOT)
  FIND_PACKAGE(Assimp)
  IF(ASSIMP_FOUND)
    SET(KLAMPT_INCLUDE_DIRS ${KLAMPT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})
    SET(KLAMPT_LIBRARIES ${KLAMPT_LIBRARIES} ${ASSIMP_LIBRARY})
  ENDIF(ASSIMP_FOUND)

  IF(NOT LOG4CXX_ROOT)
    SET(LOG4CXX_ROOT "${KRISLIBRARY_ROOT}/log4cxx" 
       CACHE PATH
       "Root of Log4Cxx package"
       FORCE
    )
  ENDIF(NOT LOG4CXX_ROOT)
  FIND_PACKAGE(LOG4CXX REQUIRED)
  SET(KLAMPT_LIBRARIES ${KLAMPT_LIBRARIES} ${LOG4CXX_LIBRARIES})


ELSE(WIN32)

  SET(KRISLIBRARY_ROOT ${KLAMPT_DEPENDENCIES})
  FIND_PACKAGE(KrisLibrary REQUIRED)
  SET(KLAMPT_DEFINITIONS ${KRISLIBRARY_DEFINITIONS})
  SET(KLAMPT_INCLUDE_DIRS ${KRISLIBRARY_INCLUDE_DIRS})
  SET(KLAMPT_LIBRARIES ${KRISLIBRARY_LIBRARIES})

  # ODE
  SET(ODE_ROOT "${KLAMPT_DEPENDENCIES}/ode-0.14" CACHE PATH "Open Dynamics Engine path" FORCE)
  FIND_PACKAGE(ODE REQUIRED)
  IF(ODE_FOUND)
    MESSAGE("Open Dynamics Engine library found")
    MESSAGE("  Compiler definitions: ${ODE_DEFINITIONS}") 
    SET(KLAMPT_DEFINITIONS ${KLAMPT_DEFINITIONS} ${ODE_DEFINITIONS})
    SET(KLAMPT_INCLUDE_DIRS ${KLAMPT_INCLUDE_DIRS} ${ODE_INCLUDE_DIRS})
    SET(KLAMPT_LIBRARIES ${KLAMPT_LIBRARIES} ${ODE_LIBRARIES})

    #pthreads is needed -- but for some reason ODE does not report it
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    FIND_LIBRARY(PTHREAD_LIBRARY "pthread")
    SET(KLAMPT_LIBRARIES ${KLAMPT_LIBRARIES} ${PTHREAD_LIBRARY})
  ENDIF(ODE_FOUND)

ENDIF(WIN32)

SET(ROSDEPS tf rosconsole roscpp roscpp_serialization rostime )
FIND_PACKAGE(ROS)
IF(ROS_FOUND)
  MESSAGE("ROS found, version " ${ROS_VERSION})
  LIST(APPEND KLAMPT_INCLUDE_DIRS ${ROS_INCLUDE_DIR})
  LIST(APPEND KLAMPT_LIBRARIES ${ROS_LIBRARIES})
  LIST(APPEND KLAMPT_DEFINITIONS "-DHAVE_ROS=1")
ENDIF(ROS_FOUND)

LIST(REMOVE_DUPLICATES KLAMPT_INCLUDE_DIRS)