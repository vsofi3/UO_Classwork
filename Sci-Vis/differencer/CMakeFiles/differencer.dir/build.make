# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.14.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.14.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Owen/Desktop/Classes/410/differencer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Owen/Desktop/Classes/410/differencer

# Include any dependencies generated for this target.
include CMakeFiles/differencer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/differencer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/differencer.dir/flags.make

CMakeFiles/differencer.dir/differencer.cxx.o: CMakeFiles/differencer.dir/flags.make
CMakeFiles/differencer.dir/differencer.cxx.o: differencer.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Owen/Desktop/Classes/410/differencer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/differencer.dir/differencer.cxx.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/differencer.dir/differencer.cxx.o -c /Users/Owen/Desktop/Classes/410/differencer/differencer.cxx

CMakeFiles/differencer.dir/differencer.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/differencer.dir/differencer.cxx.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Owen/Desktop/Classes/410/differencer/differencer.cxx > CMakeFiles/differencer.dir/differencer.cxx.i

CMakeFiles/differencer.dir/differencer.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/differencer.dir/differencer.cxx.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Owen/Desktop/Classes/410/differencer/differencer.cxx -o CMakeFiles/differencer.dir/differencer.cxx.s

# Object files for target differencer
differencer_OBJECTS = \
"CMakeFiles/differencer.dir/differencer.cxx.o"

# External object files for target differencer
differencer_EXTERNAL_OBJECTS =

differencer.app/Contents/MacOS/differencer: CMakeFiles/differencer.dir/differencer.cxx.o
differencer.app/Contents/MacOS/differencer: CMakeFiles/differencer.dir/build.make
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkDomainsChemistryOpenGL2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersFlowPaths-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersGeneric-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersHyperTree-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersParallelImaging-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersPoints-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersProgrammable-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersSMP-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersSelection-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersTexture-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersTopology-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersVerdict-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkGeovisCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOAMR-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOAsynchronous-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOCityGML-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOEnSight-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOExodus-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOExportOpenGL2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOExportPDF-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOImport-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOInfovis-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOLSDyna-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOMINC-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOMovie-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOPLY-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOParallel-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOParallelXML-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOSQL-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOSegY-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOTecplotTable-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOVeraOut-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOVideo-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingMorphological-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingStatistics-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingStencil-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkInteractionImage-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingContextOpenGL2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingImage-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingLOD-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingVolumeOpenGL2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkViewsContext2D-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkViewsInfovis-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkDomainsChemistry-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkverdict-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkproj-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersAMR-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkpugixml-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOExport-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingGL2PSOpenGL2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkgl2ps-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtklibharu-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtklibxml2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtktheora-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkogg-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersParallel-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkexodusII-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOGeometry-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIONetCDF-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkNetCDF-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkjsoncpp-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkParallelCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOLegacy-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtksqlite-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkhdf5_hl-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkhdf5-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingOpenGL2-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkglew-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingMath-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkChartsCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingContext2D-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersImaging-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkInfovisLayout-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkInfovisCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkViewsCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkInteractionWidgets-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersHybrid-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingGeneral-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingSources-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersModeling-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingHybrid-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOImage-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkDICOMParser-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkmetaio-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkpng-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtktiff-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkjpeg-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkInteractionStyle-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersExtraction-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersStatistics-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingFourier-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingAnnotation-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingColor-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingVolume-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkImagingCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOXML-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOXMLParser-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkIOCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkdoubleconversion-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtklz4-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtklzma-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkexpat-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingLabel-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingFreeType-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkRenderingCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonColor-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersGeometry-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersSources-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersGeneral-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonComputationalGeometry-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkFiltersCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonExecutionModel-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonDataModel-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonMisc-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonSystem-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonTransforms-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonMath-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkCommonCore-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtksys-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkfreetype-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: /Users/Owen/Desktop/Classes/410/build/lib/libvtkzlib-8.2.1.dylib
differencer.app/Contents/MacOS/differencer: CMakeFiles/differencer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Owen/Desktop/Classes/410/differencer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable differencer.app/Contents/MacOS/differencer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/differencer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/differencer.dir/build: differencer.app/Contents/MacOS/differencer

.PHONY : CMakeFiles/differencer.dir/build

CMakeFiles/differencer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/differencer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/differencer.dir/clean

CMakeFiles/differencer.dir/depend:
	cd /Users/Owen/Desktop/Classes/410/differencer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Owen/Desktop/Classes/410/differencer /Users/Owen/Desktop/Classes/410/differencer /Users/Owen/Desktop/Classes/410/differencer /Users/Owen/Desktop/Classes/410/differencer /Users/Owen/Desktop/Classes/410/differencer/CMakeFiles/differencer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/differencer.dir/depend

