# executables in here
add_executable(projectname
    main.cpp
    fsm/FiniteStateMachine.cpp
)

# include external libraries directory
target_include_directories(projectname PRIVATE ${CMAKE_SOURCE_DIR}/external)

# link external libraries if available
target_link_libraries(projectname PRIVATE pugixml)
