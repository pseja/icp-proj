# This script finds all generated FSM files and creates executables for them

# Debug info
message(STATUS "GENERATED_DIR: ${GENERATED_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

# Find all generated cpp files
file(GLOB FSM_GENERATED_FILES "${GENERATED_DIR}/*_generated.cpp")
message(STATUS "Found FSM files: ${FSM_GENERATED_FILES}")

# Extract Qt lib directories
get_filename_component(QT_LIB_DIR "${QTCORE_LIBRARY}" DIRECTORY)
message(STATUS "Qt library directory: ${QT_LIB_DIR}")

# Fixed executable name
set(FSM_EXEC_NAME "fsm")

# If there are multiple FSMs, the last one will be used
foreach(FSM_FILE ${FSM_GENERATED_FILES})
    # Extract the FSM name from the filename for logging
    get_filename_component(FSM_FILENAME ${FSM_FILE} NAME)
    string(REGEX REPLACE "_generated\\.cpp$" "" FSM_NAME ${FSM_FILENAME})
    
    # Compile the executable with a fixed name
    message(STATUS "Compiling FSM: ${FSM_NAME} to executable: ${FSM_EXEC_NAME}")
    
    # Build the command with proper linking
    set(COMPILE_CMD
        ${CMAKE_CXX_COMPILER} 
        -o "${CMAKE_CURRENT_BINARY_DIR}/${FSM_EXEC_NAME}" 
        "${FSM_FILE}" 
        -fPIC
        -std=c++17
        -I/usr/include/x86_64-linux-gnu/qt5
        -I/usr/include/qt5
        -L${QT_LIB_DIR}
        -lQt5Core
        -lQt5Gui
        -lQt5Widgets
        -DQT_NO_DEBUG
        -DQT_WIDGETS_LIB
        -DQT_GUI_LIB
        -DQT_CORE_LIB
    )
    
    message(STATUS "Compile command: ${COMPILE_CMD}")
    
    execute_process(
        COMMAND ${COMPILE_CMD}
        RESULT_VARIABLE COMPILE_RESULT
        OUTPUT_VARIABLE COMPILE_OUTPUT
        ERROR_VARIABLE COMPILE_ERROR
    )
    
    # Check compilation result
    if(NOT COMPILE_RESULT EQUAL 0)
        message(WARNING "Compilation failed for ${FSM_EXEC_NAME}")
        message(STATUS "Output: ${COMPILE_OUTPUT}")
        message(STATUS "Error: ${COMPILE_ERROR}")
    else()
        message(STATUS "${FSM_EXEC_NAME} successfully compiled from ${FSM_NAME}")
    endif()
endforeach()
