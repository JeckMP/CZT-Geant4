cmake_minimum_required(VERSION 3.16)

project(neutron_czt_sim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(neutron_czt_sim neutron_czt_sim.cc ${sources} ${headers})

target_link_libraries(neutron_czt_sim ${Geant4_LIBRARIES})

file(COPY macros DESTINATION ${CMAKE_BINARY_DIR})
