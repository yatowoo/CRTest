# CRTest CMakeLists.txt

# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_BUILD_TYPE Debug)
project(SimplePhys)

# Find Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup include directories for project
include(${Geant4_USE_FILE})

# Locate source and headers for project
include_directories(${PROJECT_SOURCE_DIR}/inc
    $ENV{CRYHOME}/src)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

# Add the executable, and link it to the libraries
add_executable(${PROJECT_NAME} CRTest.cxx ${sources} ${headers})
target_link_libraries(${PROJECT_NAME}
    ${Geant4_LIBRARIES}
    $ENV{CRYHOME}/lib/libCRY.a)

# Add program to the project targets
# add_custom_target(SimplePhys DEPENDS CRTest)