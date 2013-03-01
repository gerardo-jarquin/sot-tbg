INCLUDE(../cmake/python.cmake)

ADD_LIBRARY(time-based-generator SHARED time-based-generator.cpp)
SET_TARGET_PROPERTIES(${lib} PROPERTIES
  PREFIX ""
  SOVERSION ${PROJECT_VERSION})
TARGET_LINK_LIBRARIES(time-based-generator ${Boost_LIBRARIES})

PKG_CONFIG_USE_DEPENDENCY(time-based-generator sot-core)
PKG_CONFIG_USE_DEPENDENCY(time-based-generator dynamic-graph)

INSTALL(TARGETS time-based-generator DESTINATION lib/plugin)

DYNAMIC_GRAPH_PYTHON_MODULE("sot/tbg/time_based_generator"
  time-based-generator
  time_based_generator-wrap
  )

# Install empty __init__.py files in intermediate directories.
INSTALL(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_graph/sot/tbg/__init__.py
  DESTINATION ${PYTHON_SITELIB}/dynamic_graph/sot/tbg
  )
