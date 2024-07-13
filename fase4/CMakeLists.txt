cmake_minimum_required(VERSION 3.5)

# Project Name
project(CG)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add engine.cpp and generator.cpp as separate executables
add_executable(engine src/engine.cpp src/CatmullRom.cpp)
add_executable(generator src/generator.cpp src/Figures/Sphere.cpp src/Figures/Cone.cpp src/Figures/Plane.cpp src/Figures/Box.cpp src/Figures/Torus.cpp src/Figures/Patch.cpp)

# Add the -g flag to enable debugging symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

if(WIN32)
    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    if(NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(FATAL_ERROR "GLUT not found")
    endif(NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")    
    
    include_directories(${TOOLKITS_FOLDER}/glut)
    target_link_libraries(engine ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib tinyxml2 GLEW)
    target_link_libraries(generator ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib tinyxml2 GLEW)
    
    if(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")    
    
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)
    
else(WIN32) # Linux and Mac

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    
    if(NOT GLUT_FOUND)
        message(FATAL_ERROR "GLUT not found!")
    endif(NOT GLUT_FOUND)
    
    target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} tinyxml2 GLEW)
    target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} tinyxml2 GLEW)
    
endif(WIN32) # End of WIN32 check

# Set the output directory for executables
set_target_properties(engine generator
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Add a custom target for cleaning the build
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/bin/engine ${CMAKE_SOURCE_DIR}/bin/generator
)
