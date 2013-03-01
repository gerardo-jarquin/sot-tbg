CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/lapack.cmake)
INCLUDE(cmake/cpack.cmake)

SET(PROJECT_NAME sot-tbg)
SET(PROJECT_DESCRIPTION "")
SET(PROJECT_URL "https://github.com/gerardo-jarquin/sot-tbg")

SET(CUSTOM_HEADER_DIR "${PROJECT_NAME}")

# Disable -Werror on Unix for now.
#SET(CXX_DISABLE_WERROR True)

SET(PKG_CONFIG_ADDITIONAL_VARIABLES
  ${PKG_CONFIG_ADDITIONAL_VARIABLES}
  plugindirname
  plugindir
  )

SETUP_PROJECT()

# Search for dependencies.
ADD_REQUIRED_DEPENDENCY("dynamic-graph >= 2.5.0")
ADD_REQUIRED_DEPENDENCY("sot-core >= 2.5")

# List plug-ins that will be compiled.
SET(libs time-based-generator)

# Search for dependencies.
# Boost
SEARCH_FOR_BOOST()

# Add subdirectories.
#ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(test)

SETUP_PROJECT_FINALIZE()
