cmake_minimum_required(VERSION 3.5)
project(ioniq5_ecan LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(IONIQ5_ECAN_CORE_ONLY "Build the platform-independent core without ROS 2" OFF)

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
  return()
endif()

find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(diagnostic_msgs REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_srvs REQUIRED)
pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ActuationCommand.msg"
  "msg/VehicleState.msg"
  DEPENDENCIES builtin_interfaces
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
target_include_directories(ioniq5_ecan_node PRIVATE include)
target_link_libraries(ioniq5_ecan_node PRIVATE ioniq5_ecan_core ioniq5_ecan_panda)
target_link_libraries(ioniq5_ecan_node PRIVATE Threads::Threads)
ament_target_dependencies(ioniq5_ecan_node rclcpp diagnostic_msgs std_srvs)
rosidl_target_interfaces(ioniq5_ecan_node ${PROJECT_NAME} "rosidl_typesupport_cpp")

install(TARGETS
  ioniq5_ecan_core
  ioniq5_ecan_panda
  ioniq5_ecan_panda_protocol
  ioniq5_ecan_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY config launch DESTINATION share/${PROJECT_NAME})
install(PROGRAMS scripts/check_environment.sh DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)

  ament_add_gtest(test_core
    test/test_command_adapter.cpp
    test/test_hyundai_canfd_codec.cpp
    test/test_safety_supervisor.cpp
    test/test_vehicle_state_parser.cpp
  )
  target_link_libraries(test_core ioniq5_ecan_core)
  target_include_directories(test_core PRIVATE include)

  ament_add_gtest(test_panda_protocol test/test_panda_protocol.cpp)
  target_link_libraries(test_panda_protocol ioniq5_ecan_panda_protocol)
  target_include_directories(test_panda_protocol PRIVATE include)
endif()

ament_export_include_directories(include)
ament_export_dependencies(builtin_interfaces diagnostic_msgs rclcpp std_srvs)
ament_package()
