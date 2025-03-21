# executables in here
add_executable(oblivion
    main.cpp
    fsm/FSM.cpp
    fsm/State.cpp
    fsm/Transition.cpp
    fsm/Variable.cpp
    parser/Parser.cpp
)

# include external libraries directory
target_include_directories(oblivion PRIVATE ${CMAKE_SOURCE_DIR}/external)

# link external libraries if available
target_link_libraries(oblivion PRIVATE pugixml)
