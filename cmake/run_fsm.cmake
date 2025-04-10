# This script runs FSM executables
# Find all generated cpp files to determine FSM names
file(GLOB FSM_GENERATED_FILES "${GENERATED_DIR}/*_generated.cpp")

foreach(FSM_FILE ${FSM_GENERATED_FILES})
    # Extract the FSM name from the filename
    get_filename_component(FSM_FILENAME ${FSM_FILE} NAME)
    string(REGEX REPLACE "_generated\\.cpp$" "" FSM_NAME ${FSM_FILENAME})
    
    # Run the FSM executable
    message(STATUS "Running FSM: ${FSM_NAME}")
    message(STATUS "Executing: ${CMAKE_CURRENT_BINARY_DIR}/fsm")
    
    # Use CMake's built-in command executor to avoid shell parsing issues
    execute_process(
        COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fsm"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RESULT_VARIABLE RESULT
    )
    
    if(RESULT EQUAL 0)
        message(STATUS "FSM ${FSM_NAME} completed successfully")
    else()
        message(WARNING "FSM ${FSM_NAME} failed with code: ${RESULT}")
    endif()
endforeach()
