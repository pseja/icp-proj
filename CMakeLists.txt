cmake_minimum_required(VERSION 3.16)
project(icp-proj LANGUAGES CXX)

# Set paths to Qt5 libraries
set(Qt5_DIR "/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/lib/cmake/Qt5/")
set(CMAKE_PREFIX_PATH "/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Export compile commands for Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Scxml Qml Xml Test)

# Source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.hpp
)

# Main executable
add_executable(icp-proj ${SRC_FILES})

set_target_properties(icp-proj PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM_PLUGIN_PATH=/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/plugins/platforms"
)

target_include_directories(icp-proj PRIVATE src)

#target_include_directories(icp-proj PRIVATE
#    ${CMAKE_BINARY_DIR}/icp-proj_autogen/include
#    ${CMAKE_BINARY_DIR}/icp-proj_autogen/include/src/frontend
#    ${CMAKE_BINARY_DIR}/icp-proj_autogen/include/frontend
#)

target_link_libraries(icp-proj PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Xml Qt5::Network)
