cmake_minimum_required(VERSION 3.16)
project(icp-proj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Scxml Qml Xml Test)

# Source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.hpp
)

# Main executable
add_executable(icp-proj ${SRC_FILES})

target_include_directories(icp-proj PRIVATE src)
target_include_directories(icp-proj PRIVATE
    ${CMAKE_BINARY_DIR}/icp-proj_autogen/include
    ${CMAKE_BINARY_DIR}/icp-proj_autogen/include/src/frontend
    ${CMAKE_BINARY_DIR}/icp-proj_autogen/include/frontend
)
target_link_libraries(icp-proj PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Xml Qt5::Network)

file(GLOB GENERATED_FSMS generated/*.cpp)
foreach(FSM_SRC ${GENERATED_FSMS})
    get_filename_component(FSM_NAME ${FSM_SRC} NAME_WE)
    add_executable(${FSM_NAME} ${FSM_SRC})
    target_link_libraries(${FSM_NAME} PRIVATE Qt5::Core Qt5::Xml Qt5::Network)
    set_target_properties(${FSM_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endforeach()

file(GLOB GENERATED_FSM_SRC generated/*_generated.cpp)
add_custom_target(run
    COMMAND icp-proj
    COMMAND ${GENERATED_FSM_NAME}
    DEPENDS icp-proj ${GENERATED_FSM_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running icp-proj and ${GENERATED_FSM_NAME}"
)