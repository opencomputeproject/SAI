# Copyright 2017 Mellanox 
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Makefile for a backend that generates code for the Sai
# compiling for the sai_switch target.
# sources for backend executable
set (SAI_SRCS json_stage/sai.cpp)

set (SAI_BACKEND_SRCS
  json_stage/action.cpp
  json_stage/analyzer.cpp
  json_stage/backend.cpp
  json_stage/control.cpp
  json_stage/deparser.cpp
  json_stage/errorcode.cpp
  json_stage/expression.cpp
  json_stage/extern.cpp
  json_stage/extractArchInfo.cpp
  json_stage/header.cpp
  json_stage/helpers.cpp
  json_stage/JsonObjects.cpp
  json_stage/lower.cpp
  json_stage/metermap.cpp
  json_stage/midend.cpp
  json_stage/parser.cpp
  json_stage/sharedActionSelectorCheck.cpp
  json_stage/saiSwitch.cpp
  json_stage/synthesizeValidField.cpp
  )

set (SAI_BACKEND_HDRS
  json_stage/action.h
  json_stage/analyzer.h
  json_stage/backend.h
  json_stage/control.h
  json_stage/deparser.h
  json_stage/errorcode.h
  json_stage/expression.h
  json_stage/extern.h
  json_stage/extractArchInfo.h
  json_stage/header.h
  json_stage/helpers.h
  json_stage/JsonObjects.h
  json_stage/lower.h
  json_stage/metermap.h
  json_stage/midend.h
  json_stage/options.h
  json_stage/parser.h
  json_stage/sharedActionSelectorCheck.h
  json_stage/saiSwitch.h
  json_stage/synthesizeValidField.h
  )

#execute_process(
  #COMMAND sed 's@SAISRCDIR@&${CMAKE_CURRENT_SOURCE_DIR}/@' ${CMAKE_CURRENT_SOURCE_DIR}/json_stage/sai_t.cpp > ${CMAKE_CURRENT_SOURCE_DIR}/json_stage/sai.cpp
#)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/json_stage/p4c_python.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/json_stage/p4c_python.cpp)
set (IR_DEF_FILES ${IR_DEF_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/json_stage/sai.def PARENT_SCOPE)

add_library(saibackend ${SAI_BACKEND_SRCS})
add_dependencies(saibackend genIR)

# for flextrum (json=>c api) add python support:
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(p4c-sai ${SAI_SRCS})
target_link_libraries (p4c-sai saibackend ${P4C_LIBRARIES} ${P4C_LIB_DEPS} ${PYTHON_LIBRARIES})

install(TARGETS p4c-sai RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})
install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/json_stage/p4include
   DESTINATION ${P4C_ARTIFACTS_OUTPUT_DIRECTORY})
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/json_stage/driver/p4c.p4c-sai.cfg
  DESTINATION ${P4C_ARTIFACTS_OUTPUT_DIRECTORY}/p4c_src)

set (SAI_SOURCES ${SAI_BACKEND_SRCS} ${SAI_SRCS} ${SAI_BACKEND_HDRS})
add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${SAI_SOURCES}")

# hack to get around the fact that the test scripts expect the backend
# binary to be in the top level directory. This should go away when we
# remove automake and fix the scripts.
add_custom_target(linksai
  # COMMAND sed "s/SAISRCDIR/${CMAKE_CURRENT_SOURCE_DIR}/" ${CMAKE_CURRENT_SOURCE_DIR}/sai_t.cpp > ${CMAKE_CURRENT_SOURCE_DIR}/sai.cpp
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4c-sai ${P4C_BINARY_DIR}/p4c-sai
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4include ${CMAKE_CURRENT_BINARY_DIR}/p4include
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4_14include ${CMAKE_CURRENT_BINARY_DIR}/p4_14include
  )
add_dependencies(p4c_driver linksai)

# Tests


if (HAVE_SAI_SWITCH)
  p4c_add_tests("sai" ${SAI_DRIVER} "${SAI_TEST_SUITES}" "${XFAIL_TESTS}")
else()
  MESSAGE(WARNING "Sai switch is not available, not adding sai tests")
endif()
