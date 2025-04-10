cmake_minimum_required(VERSION 3.16)
project(icp-proj VERSION 0.1 LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enable automatic processing for Qt features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Scxml Qml Xml Test)

# Define project sources
set(PROJECT_SOURCES
        src/main.cpp
        src/frontend/mainwindow.cpp
        src/frontend/mainwindow.hpp
        src/frontend/mainwindow.ui
        src/backend/MockStateMachine.cpp
        src/backend/CodeGen.cpp
        src/backend/fsm.cpp
        src/backend/state.cpp
        src/backend/transition.cpp
        src/backend/variable.cpp
        src/backend/logger.cpp
        src/backend/xmlparser.cpp
        src/backend/fsm.hpp
        src/backend/state.hpp
        src/backend/transition.hpp
        src/backend/variable.hpp
        src/backend/logger.hpp
        src/backend/xmlparser.hpp
)

# Add executable target
add_executable(icp-proj ${PROJECT_SOURCES})

# Link required Qt5 modules
target_link_libraries(icp-proj PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Scxml Qt5::Qml Qt5::Xml Qt5::Test)

# Define the path for the generated file
set(GENERATED_FILE_PATH "${CMAKE_SOURCE_DIR}/generated/*_generated.cpp")

# Generate FSM code
add_custom_command(
    OUTPUT ${GENERATED_FILE_PATH}
    COMMAND $<TARGET_FILE:icp-proj>
    DEPENDS icp-proj
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating FSM code"
)

# Directory for generated files
set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Generate FSM code target
add_custom_target(generate_fsm
    COMMAND $<TARGET_FILE:icp-proj>
    DEPENDS icp-proj
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating FSM code"
)

# Create a 'gen' target that depends on generate_fsm
add_custom_target(gen
    DEPENDS generate_fsm
    COMMENT "Generating FSM code"
)

# Find all generated FSM files after generation
add_custom_target(fsm_build ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Building FSM executables"
    DEPENDS generate_fsm
)

# Dynamic approach to find and compile FSMs
add_custom_command(
    TARGET fsm_build
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        -DGENERATED_DIR=${GENERATED_DIR}
        -DCMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DQTCORE_LIBRARY=$<TARGET_FILE:Qt5::Core>
        -DQTGUI_LIBRARY=$<TARGET_FILE:Qt5::Gui>
        -DQTWIDGETS_LIBRARY=$<TARGET_FILE:Qt5::Widgets>
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/compile_fsm.cmake
    COMMENT "Compiling FSM executables"
)

# Run the FSM executable
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo "Running FSM" 
    COMMAND ${CMAKE_COMMAND} -E env "${CMAKE_CURRENT_BINARY_DIR}/fsm"
    DEPENDS fsm_build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running FSM"
)

# Add generated directory to clean files
set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_SOURCE_DIR}/generated")

# Install the target
install(TARGETS icp-proj
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)