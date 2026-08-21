cmake_minimum_required(VERSION 3.10)
project(ioniq5_ecan LANGUAGES C CXX)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(IONIQ5_ECAN_CORE_ONLY "Build the platform-independent core without ROS 1" OFF)

find_package(Threads REQUIRED)

add_library(ioniq5_ecan_core
  src/command_adapter.cpp
  src/hyundai_canfd_codec.cpp
  src/safety_supervisor.cpp
  src/vehicle_state_parser.cpp
)
target_include_directories(ioniq5_ecan_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ioniq5_ecan_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(IONIQ5_ECAN_CORE_ONLY)
  enable_testing()
  add_executable(core_smoke test/core_smoke.cpp)
  target_link_libraries(core_smoke PRIVATE ioniq5_ecan_core)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(core_smoke PRIVATE -Wall -Wextra -Wpedantic -Werror)
  endif()
  add_test(NAME core_smoke COMMAND core_smoke)
  find_package(Python3 QUIET COMPONENTS Interpreter)
  if(Python3_Interpreter_FOUND)
    add_test(
      NAME panda_preflight_unit
      COMMAND ${Python3_EXECUTABLE} -m unittest discover -s test -p test_panda_preflight.py
    )
    set_tests_properties(panda_preflight_unit PROPERTIES
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  endif()
  return()
endif()

find_package(catkin REQUIRED COMPONENTS
  diagnostic_msgs
  message_generation
  roscpp
  std_srvs
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)

add_message_files(
  FILES
  ActuationCommand.msg
  RawCanFrame.msg
  VehicleState.msg
)
generate_messages()

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ioniq5_ecan_core ioniq5_ecan_panda ioniq5_ecan_panda_protocol
  CATKIN_DEPENDS diagnostic_msgs message_runtime roscpp std_srvs
)

add_library(ioniq5_ecan_panda src/panda_usb.cpp)
add_library(ioniq5_ecan_panda_protocol src/panda_protocol.cpp)
target_include_directories(ioniq5_ecan_panda_protocol PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ioniq5_ecan_panda_protocol PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
target_include_directories(ioniq5_ecan_panda PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ioniq5_ecan_panda PUBLIC ioniq5_ecan_panda_protocol PkgConfig::LIBUSB)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ioniq5_ecan_panda PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(ioniq5_ecan_node src/main.cpp src/node.cpp)
target_include_directories(ioniq5_ecan_node PRIVATE include ${catkin_INCLUDE_DIRS})
target_link_libraries(ioniq5_ecan_node
  ioniq5_ecan_core
  ioniq5_ecan_panda
  Threads::Threads
  ${catkin_LIBRARIES}
)
add_dependencies(ioniq5_ecan_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ioniq5_ecan_node PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

install(TARGETS
  ioniq5_ecan_core
  ioniq5_ecan_panda
  ioniq5_ecan_panda_protocol
  ioniq5_ecan_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
install(DIRECTORY include/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
install(DIRECTORY config launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(PROGRAMS
  scripts/build_panda_debug_firmware.sh
  scripts/check_environment.sh
  scripts/flash_panda.py
  scripts/panda_preflight.py
  scripts/recover_panda.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

if(CATKIN_ENABLE_TESTING)
  catkin_add_gtest(test_core
    test/gtest_main.cpp
    test/test_command_adapter.cpp
    test/test_hyundai_canfd_codec.cpp
    test/test_safety_supervisor.cpp
    test/test_vehicle_state_parser.cpp
  )
  if(TARGET test_core)
    target_link_libraries(test_core ioniq5_ecan_core)
    target_include_directories(test_core PRIVATE include)
  endif()

  catkin_add_gtest(test_panda_protocol test/gtest_main.cpp test/test_panda_protocol.cpp)
  if(TARGET test_panda_protocol)
    target_link_libraries(test_panda_protocol ioniq5_ecan_panda_protocol)
    target_include_directories(test_panda_protocol PRIVATE include)
  endif()

  catkin_add_nosetests(test/test_panda_preflight.py)
endif()
