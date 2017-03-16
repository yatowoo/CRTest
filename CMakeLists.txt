# CRTest CMakeLists.txt

# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_BUILD_TYPE Debug)
project(SimplePhys)

# Find Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)
# Find ROOT
set(ROOTSYS $ENV{ROOTSYS})
if(DEFINED ROOTSYS AND NOT DEFINED ROOT_DIR)
  set(ROOT_DIR ${ROOTSYS}/cmake)
endif()
find_package(ROOT REQUIRED)

# Setup include directories for project
include(${Geant4_USE_FILE}
     ${ROOT_USE_FILE})
# Locate source and headers for project
include_directories(${PROJECT_SOURCE_DIR}/inc
    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIRS}
    $ENV{CRYHOME}/src)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

# Add the executable, and link it to the libraries
add_executable(${PROJECT_NAME} CRTest.cxx ${sources} ${headers})
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
target_link_libraries(${PROJECT_NAME}
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
    $ENV{CRYHOME}/lib/libCRY.a)

# Add program to the project targets
add_custom_target(LINK_MAC COMMAND ln -s -f ${PROJECT_SOURCE_DIR}/mac . )
add_dependencies(${PROJECT_NAME} LINK_MAC)