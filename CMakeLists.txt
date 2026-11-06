cmake_minimum_required(VERSION 3.20)
project(CodeBlooded VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(CODEBLOODED_BUILD_TESTS "Build unit tests for game logic" ON)

add_library(codeblooded_game STATIC
    CodeBlooded/Robot.cpp
    CodeBlooded/ScriptEngine.cpp
    CodeBlooded/Simulation.cpp
)
target_include_directories(codeblooded_game PUBLIC ${CMAKE_SOURCE_DIR}/CodeBlooded)

find_package(wxWidgets CONFIG QUIET COMPONENTS core base)
if(NOT TARGET wx::core)
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    include(${wxWidgets_USE_FILE})
endif()

add_executable(CodeBlooded WIN32
    CodeBlooded/App.cpp
    CodeBlooded/MainFrame.cpp
    CodeBlooded/EditorPanel.cpp
    CodeBlooded/ArenaPanel.cpp
)
target_link_libraries(CodeBlooded PRIVATE codeblooded_game)

if(TARGET wx::core)
    target_link_libraries(CodeBlooded PRIVATE wx::core wx::base)
else()
    target_link_libraries(CodeBlooded PRIVATE ${wxWidgets_LIBRARIES})
endif()

if(MSVC)
    target_compile_definitions(CodeBlooded PRIVATE WXUSINGDLL UNICODE _UNICODE)
    target_compile_options(CodeBlooded PRIVATE /W3 /permissive-)
    set_target_properties(CodeBlooded PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

if(CODEBLOODED_BUILD_TESTS)
    enable_testing()
    add_executable(codeblooded_tests tests/test_game_logic.cpp)
    target_link_libraries(codeblooded_tests PRIVATE codeblooded_game)
    add_test(NAME game_logic COMMAND codeblooded_tests)
endif()
