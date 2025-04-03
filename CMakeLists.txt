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
)

# Add executable target
add_executable(icp-proj ${PROJECT_SOURCES})

# Link required Qt5 modules
target_link_libraries(icp-proj PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Scxml Qt5::Qml Qt5::Xml Qt5::Test)

# Generate FSM code
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/TOF5s_generated.cpp
    COMMAND $<TARGET_FILE:icp-proj>
    DEPENDS icp-proj
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating FSM code"
)

# The test executable
add_executable(TOF5s_test EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/TOF5s_generated.cpp)
set_target_properties(TOF5s_test PROPERTIES AUTOMOC ON)
target_link_libraries(TOF5s_test PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)

# Make the test dependent on the generated code
add_dependencies(TOF5s_test icp-proj)
add_custom_target(generate_fsm DEPENDS ${CMAKE_SOURCE_DIR}/TOF5s_generated.cpp)
add_dependencies(TOF5s_test generate_fsm)

# Simple test target
add_custom_target(test
    COMMAND $<TARGET_FILE:TOF5s_test>
    DEPENDS TOF5s_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running FSM test"
)

# Install the target
install(TARGETS icp-proj
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
