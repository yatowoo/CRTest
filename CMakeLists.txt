# CRTest CMakeLists.txt

# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(SimplePhys)

# Find Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup include directories for project
include(${Geant4_USE_FILE})

# Locate source and headers for project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc}
    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

# Add the executable, and link it to the libraries
add_executable(CRTest CRTest.cxx ${sources} ${headers})
target_link_libraries(CRTest ${Geant4_LIBRARIES})

# Add program to the project targets
# add_custom_target(SimplePhys DEPENDS CRTest)