cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(FATAL_ERROR "You must set VCPKG_ROOT env variable or pass CMAKE_TOOLCHAIN_FILE")
endif()

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(pihole-influx-exporter)
set(CMAKE_CXX_STANDARD 20)

find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS json)
find_package(InfluxDB CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(pihole-influx-exporter
  src/main.cpp
)
target_link_libraries(
  pihole-influx-exporter
  PRIVATE 
  unofficial::curlpp::curlpp
  Boost::json
  InfluxData::InfluxDB
  fmt::fmt
  )
