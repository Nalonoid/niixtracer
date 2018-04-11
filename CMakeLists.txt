project(raytracer)

#---------------------------------------#
#---  Configure CMake and GCC flags  ---#
#---------------------------------------#

cmake_minimum_required(VERSION 2.8.9) # Minimal version compatible QT5
CMAKE_POLICY(SET CMP0043 NEW) # This will silence the Cmake Warning "Policy CMP0043 is not set"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#find_package(OpenGL REQUIRED) # defines OPENGL_LIBRARIES
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5OpenGL REQUIRED)
find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#------------------------------------------#
#---  Define project sources & headers  ---#
#------------------------------------------#

# project_sources variable contains all the .cpp/.inl files
FILE(GLOB_RECURSE project_sources ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.inl)

# project_headers variable contains all the .h/.hpp files
FILE(GLOB_RECURSE project_headers ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.hpp)

# project_ui variable contains all the .ui files used for the Qt interface
FILE(GLOB_RECURSE project_ui ${CMAKE_SOURCE_DIR}/src/*.ui)

include_directories(
   ${CMAKE_SOURCE_DIR}/src
)

SOURCE_GROUP("Source Files" FILES ${project_sources})
SOURCE_GROUP("Header Files" FILES ${project_headers})

#----------------------------------#
#---  Configure Qt environment  ---#
#----------------------------------#

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
   ${Qt5Widgets_INCLUDES}
#   ${Qt5OpenGL_INCLUDES}
)

add_definitions(${Qt5Widgets_DEFINITIONS})
#add_definitions(${Qt5OpenGL_DEFINITIONS})

#---------------------------------#
#---  Build target executable  ---#
#---------------------------------#

add_executable(raytracer
               ${project_sources}
               ${project_headers}
               ${project_ui})

qt5_use_modules(raytracer Widgets) #OpenGL)
set(EXT_LIBS ${QT_LIBRARIES}) #${OPENGL_LIBRARIES})
target_link_libraries(raytracer ${EXT_LIBS})
