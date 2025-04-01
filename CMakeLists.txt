cmake_minimum_required(VERSION 3.16)
project(icp-proj VERSION 0.1 LANGUAGES CXX)

# Set paths to Qt5 libraries
set(Qt5_DIR "/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/lib/cmake/Qt5/")
set(CMAKE_PREFIX_PATH "/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/")

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
)

# Add executable target
add_executable(icp-proj ${PROJECT_SOURCES})

# Link required Qt5 modules
target_link_libraries(icp-proj PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Scxml Qt5::Qml Qt5::Xml Qt5::Test)

# Install the target
install(TARGETS icp-proj
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
