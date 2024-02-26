cmake_minimum_required (VERSION 3.11)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)

set(REDCRAFT_VERSION 2.0.20)


if(APPLE)
    set(CMAKE_C_COMPILER "/usr/local/bin/gcc-11")
    set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-11")
endif()
if(WIN32)
    set(CMAKE_C_COMPILER "gcc.exe")
    set(CMAKE_CXX_COMPILER "g++.exe")
    #set(CMAKE_FIND_ROOT_PATH "C:/MinGW")
endif()

project (Redcraft)

# Enable multicore processing
include(FindOpenMP)
if(OPENMP_FOUND)
    message("OpenMP works, here are the flags: ${OpenMP_CXX_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	else()
	message("OpenMP not found! Parallel processing disabled")
endif()
# Enable GProf hooks for profiling
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

# Enable CUDA offloading for OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector -fcf-protection=none")

# Enable testing
enable_testing()
add_subdirectory(googletest)
add_subdirectory(test)


# REDCRAFT binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(EXEC_SOURCES
        src/stage1.cpp
        src/stage_1_point_5.cpp
        src/molan.cpp
        src/umin.cpp
        src/cmin.cpp
        src/exmin.cpp
        src/pdbgen.cpp
        src/pdbgen2.cpp
        src/stage2.cpp
        src/lm-min.cpp)

# Includes the Eigen project
include_directories(src)

# REDCRAFT relevant libraries

set(VALPACK
        src/aminoacid.cpp
        src/atom.cpp
        src/matrix.cpp
        src/polypeptide.cpp
        src/rotation.cpp
        src/tensor.cpp
        src/tstorage.cpp
        src/redcraft.cpp
        src/diagnostics.h
        src/angles.cpp)

add_library(valpack ${VALPACK})

set (REDCAT
        src/redcat.cpp
        src/angles.cpp
        src/tensor.cpp
        src/cluster_collection.cpp)

add_library(redcat ${REDCAT})

file(GLOB RUDE_CONFIG ${PROJECT_SOURCE_DIR}/src/rude_config/*.cpp ${PROJECT_SOURCE_DIR}/src/rude_config/*.h)
add_library(rude_config ${RUDE_CONFIG})

set (LEVMAR
        src/lm/lm.c
        src/lm/Axb.c
        src/lm/misc.c
        src/lm/lmlec.c
        src/lm/lmbc.c
        src/lm/lmblec.c)
add_library(levmar ${LEVMAR})

# Add REDCRAFT shell first, which doesn't depend on others
# add_definitions(-Dinstall_path=${CMAKE_INSTALL_PREFIX})
add_executable(redcraft src/shell.cpp)
target_compile_definitions(redcraft PRIVATE INSTALL_PATH="${CMAKE_INSTALL_PREFIX}")
target_compile_definitions(redcraft PRIVATE INSTALL_VERSION="${REDCRAFT_VERSION}")



# Iterate through other executables and link with directories
foreach(executable_source ${EXEC_SOURCES})
    string( REPLACE ".cpp" "" executable_relpath ${executable_source} )
    string( REPLACE "src/" "" executable_name ${executable_relpath} )
    add_executable(${executable_name} ${executable_relpath})
    target_link_libraries(${executable_name} valpack redcat rude_config levmar)
    install(TARGETS ${executable_name} DESTINATION redcraft/bin)
endforeach()

install(TARGETS redcraft DESTINATION bin)
install(DIRECTORY scripts/ DESTINATION redcraft/scripts USE_SOURCE_PERMISSIONS)
# install(DIRECTORY redcraft_python/ DESTINATION redcraft/py USE_SOURCE_PERMISSIONS)

find_package (Python3 COMPONENTS Interpreter)
if (Python3_FOUND)
    message("Creating virtual environment for use with REDCRAFT")
    execute_process (COMMAND "${Python3_EXECUTABLE}" -m venv "venv")
    execute_process (COMMAND "./venv/bin/pip3" install --upgrade pip)
    execute_process (COMMAND "./venv/bin/pip3" install -r requirements.txt --upgrade)
else()
    message("Unable to find a Python3 interpreter! REDCRAFT scripts written in
    Python not runnable.")
endif()
install(DIRECTORY venv/ DESTINATION redcraft/py USE_SOURCE_PERMISSIONS)

message("CMake install prefix: ${CMAKE_INSTALL_PREFIX}")

# QT support

# Attempt to find Qt5 dependencies
find_package(Qt5 COMPONENTS Core Widgets Charts)
if (Qt5_FOUND)
	# Find includes in corresponding build directories
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	# Instruct CMake to run moc automatically when needed
	set(CMAKE_AUTOMOC ON)
	# Create code from a list of Qt designer ui files
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON)
	
	set(REDCRAFT_GUI_DIR src/gui/redcraft)
	include_directories( ${PROJECT_SOURCE_DIR}/${REDCRAFT_GUI_DIR} )
	set(REDCRAFT_GUI
	    ${REDCRAFT_GUI_DIR}/analysissuite.cpp
	    ${REDCRAFT_GUI_DIR}/analysissuite.h
	    ${REDCRAFT_GUI_DIR}/analysissuite.ui
	    ${REDCRAFT_GUI_DIR}/dialogsearchdepth.cpp
	    ${REDCRAFT_GUI_DIR}/dialogsearchdepth.h
	    ${REDCRAFT_GUI_DIR}/dialogsearchdepth.ui
	    ${REDCRAFT_GUI_DIR}/dynamicprofile.cpp
	    ${REDCRAFT_GUI_DIR}/dynamicprofile.h
	    ${REDCRAFT_GUI_DIR}/histogram.cpp
	    ${REDCRAFT_GUI_DIR}/histogram.h
	    ${REDCRAFT_GUI_DIR}/main.cpp
	    ${REDCRAFT_GUI_DIR}/mainwindow.cpp
	    ${REDCRAFT_GUI_DIR}/mainwindow.h
	    ${REDCRAFT_GUI_DIR}/mainwindow.ui
	    ${REDCRAFT_GUI_DIR}/redcat.cpp
	    ${REDCRAFT_GUI_DIR}/scatter.cpp
	    ${REDCRAFT_GUI_DIR}/scatter.h
	    ${REDCRAFT_GUI_DIR}/config.cpp
	    ${REDCRAFT_GUI_DIR}/io.cpp
	    ${REDCRAFT_GUI_DIR}/stage0.cpp
	    ${REDCRAFT_GUI_DIR}/stage1.cpp
	    ${REDCRAFT_GUI_DIR}/stage2.cpp
	    ${REDCRAFT_GUI_DIR}/breeze.qrc 
	    ${REDCRAFT_GUI_DIR}/icons.qrc 
	    )
	
	add_executable(gui ${REDCRAFT_GUI})
	target_link_libraries(gui Qt5::Widgets Qt5::Core Qt5::Charts)
	install(TARGETS gui DESTINATION redcraft/bin)
	
	# Backbone RMSD GUI
	
	set(RMSD_GUI_DIR src/gui/rmsd)
	include_directories( ${PROJECT_SOURCE_DIR}/${RMSD_GUI_DIR} )
	set(RMSD_GUI
	    ${RMSD_GUI_DIR}/FloatArray.cpp
	    ${RMSD_GUI_DIR}/FloatArray.h
	    ${RMSD_GUI_DIR}/gui.cpp
	    ${RMSD_GUI_DIR}/gui.h
	    ${RMSD_GUI_DIR}/gui.ui
	    ${RMSD_GUI_DIR}/main.cpp
	    ${RMSD_GUI_DIR}/Matrix.cpp
	    ${RMSD_GUI_DIR}/Matrix.h
	    ${RMSD_GUI_DIR}/PDBParser.cpp
	    ${RMSD_GUI_DIR}/PDBParser.h
	    ${RMSD_GUI_DIR}/Vector.cpp
	    ${RMSD_GUI_DIR}/Vector.h)
	add_executable(rmsd ${RMSD_GUI})
	target_link_libraries(rmsd Qt5::Widgets Qt5::Core)
	install(TARGETS rmsd DESTINATION redcraft/bin)
else()
	message("Qt5 dependencies not sufficient. REDCRAFT GUI unavailable.")
endif()

set(CPACK_PACKAGE_NAME "REDCRAFT")
set(CPACK_PACKAGE_VENDOR "Ifestos")
set(CPACK_PACKAGE_CONTACT "hvalafar@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Residual Dipolar Couplings analysis and computation suite")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Redcraft")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

include(CPack)
