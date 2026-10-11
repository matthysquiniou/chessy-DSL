# Code commun : tout sauf main.cpp
add_library(chessy_core STATIC
    Token.cpp
    Lexer.cpp
    Parser.cpp
    Scope.cpp
    Analyser.cpp
    Compiler.cpp
    Chunk.cpp
    ChessNative.cpp
    VM.cpp
)
target_include_directories(chessy_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(chessy_core PUBLIC
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# Programme principal
add_executable(Chessy_Lexer main.cpp)
target_link_libraries(Chessy_Lexer PRIVATE chessy_core)

set_target_properties(Chessy_Lexer PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(chessy_core PUBLIC /arch:AVX2)
else()
    target_compile_options(chessy_core PUBLIC -mavx2)
endif()