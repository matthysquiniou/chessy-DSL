add_executable(Chessy_Tests
    main.cpp
    LexerTest.cpp
    ParserTest.cpp
    AnalyserTest.cpp
    CompilationTest.cpp
)
target_link_libraries(Chessy_Tests PRIVATE chessy_core)

# Cible sans fichier de sortie : elle s'exécute à CHAQUE build.
add_custom_target(run_tests
    COMMAND $<TARGET_FILE:Chessy_Tests>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Execution des tests"
    USES_TERMINAL
)
add_dependencies(run_tests Chessy_Tests)

# Le programme ne se construit (donc ne se lance) que si les tests passent.
add_dependencies(Chessy_Lexer run_tests)