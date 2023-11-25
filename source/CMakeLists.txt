cmake_minimum_required(VERSION 3.18.0)

set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)

set(BUILD_RELEASE ON)

if(BUILD_RELEASE)
      set(CMAKE_BUILD_TYPE Release)
else()
      set(CMAKE_BUILD_TYPE Debug)
endif()

project(RasFled)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS system thread filesystem REQUIRED)

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_library(WS2811_LIBRARIES NAMES ws2811)
find_library(PTHREAD_LIBRARIES NAMES pthread)
find_library(GLFW_LIBRARIES NAMES glfw3)
find_library(LIBRT rt)

add_executable(rasfled)

target_include_directories(rasfled PRIVATE 
  ../../rpi_ws281x-master
  ../../imgui
  ../../imgui/backends
  ../../imgui/misc/cpp
  ${Boost_INCLUDE_DIR}
)

target_sources(rasfled PRIVATE
  ../../imgui/imgui.cpp
  ../../imgui/imgui_draw.cpp
  ../../imgui/imgui_tables.cpp
  ../../imgui/imgui_widgets.cpp
  ../../imgui/misc/cpp/imgui_stdlib.cpp
  ../../imgui/backends/imgui_impl_glfw.cpp
  ../../imgui/backends/imgui_impl_opengl2.cpp
  alert_system.cpp
  alert_system_2.cpp
  LEDstuff.cpp
  commands.cpp
  comport.cpp
  auto.cpp
  configuration_storage.cpp
  helper.cpp
  stringthings.cpp
  fled_time.cpp
  json_interface.cpp
  rasapi.cpp
  hardware_module_door.cpp
  hardware_status.cpp
  animations_storage.cpp
  animations_handler.cpp
  automobile_handler.cpp
  interface.cpp
  aircraft_coordinator.cpp
  widgets.cpp
  widgets_drawing.cpp
  widgets_automobile.cpp
  widgets_adsb.cpp
  screen4_helper.cpp
  screen4.cpp
  system.cpp
  fledcore.cpp
  main.cpp
)

target_compile_definitions(rasfled PRIVATE BOOST_BIND_GLOBAL_PLACEHOLDERS)

target_link_libraries(rasfled 
  ${WIRINGPI_LIBRARIES} 
  ${WS2811_LIBRARIES}
  glfw
  ${OPENGL_LIBRARIES}
  ${PTHREAD_LIBRARIES}
  ${LIBRT}
  ${Boost_LIBRARIES}
)

# Strict
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(rasfled PRIVATE -Wall -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(rasfled PRIVATE -Weverything -Werror)
endif()
