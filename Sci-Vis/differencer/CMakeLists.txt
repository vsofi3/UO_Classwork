cmake_minimum_required(VERSION 2.8.12.1)

PROJECT(differencer)
SET(VTK_DIR /Users/Owen/Desktop/Classes/410/build)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(differencer MACOSX_BUNDLE differencer)

if(VTK_LIBRARIES)
  target_link_libraries(differencer ${VTK_LIBRARIES})
else()
  target_link_libraries(differencer vtkHybrid)
endif()

