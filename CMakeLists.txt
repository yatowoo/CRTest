# CRTest CMakeLists.txt

# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_BUILD_TYPE Debug)
project(CRTest)

# Find Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup include directories for project
include(${Geant4_USE_FILE})

# Locate source and headers for project
include_directories(${PROJECT_SOURCE_DIR}/inc
    ${Geant4_INCLUDE_DIR}
    $ENV{ROOTSYS}/include
    $ENV{CRYHOME}/src)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

# Add the executable, and link it to the libraries
add_executable(${PROJECT_NAME} CRTest.cxx ${sources} ${headers})
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
target_link_libraries(${PROJECT_NAME}
    ${Geant4_LIBRARIES}
		$ENV{ROOTSYS}/lib/libCore.so
		$ENV{ROOTSYS}/lib/libGeom.so
		$ENV{ROOTSYS}/lib/libGpad.so
		$ENV{ROOTSYS}/lib/libGraf3d.so
		$ENV{ROOTSYS}/lib/libGraf.so
		$ENV{ROOTSYS}/lib/libGui.so
		$ENV{ROOTSYS}/lib/libHist.so
		$ENV{ROOTSYS}/lib/libMathCore.so
		$ENV{ROOTSYS}/lib/libMatrix.so
		$ENV{ROOTSYS}/lib/libMinuit.so
		$ENV{ROOTSYS}/lib/libMinuit2.so
		$ENV{ROOTSYS}/lib/libRint.so
		$ENV{ROOTSYS}/lib/libTMVA.so
		$ENV{ROOTSYS}/lib/libTree.so
		$ENV{ROOTSYS}/lib/libTreeViewer.so
		$ENV{ROOTSYS}/lib/libXMLIO.so
		$ENV{ROOTSYS}/lib/libXMLParser.so
    $ENV{CRYHOME}/lib/libCRY.a)

# Add program to the project targets
add_custom_target(LINK_MAC COMMAND ln -s -f ${PROJECT_SOURCE_DIR}/mac . )
add_dependencies(${PROJECT_NAME} LINK_MAC)
