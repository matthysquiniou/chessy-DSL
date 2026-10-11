# Chessy - A Chess DSL

Chessy is a domain-specific language (DSL) for writing chess programs. It was designed and implemented in **10 days** by **2 developers**. The language provides chess-specific types and built-in functions (bitboards, pieces, squares, moves) while remaining simple and expressive.

## Key Features

- **Native types**: `Bool`, `I8`..`I64`, `U8`..`U64`, `String`, `Bitboard`, `Piece`, `Square`, `Move`, `Board`, plus SIMD vectors (`I32x8`, `I64x4`).
- **Operations**: arithmetic, logical, bitwise, comparisons, shifts.
- **Control structures**: `if`, `do-while`, `return`.
- **User-defined functions** with parameters and return values.
- **Chess built-ins**: move generation, attack calculation, bitboard manipulation, printing, etc. (see `Builtins.h`).
- **Compilation to bytecode** and execution on a virtual machine (VM).

## Architecture

The project follows a classic compilation pipeline:

1. **Lexer**: transforms source code into a list of tokens.
2. **Parser**: builds an AST (abstract syntax tree) from tokens.
3. **Analyser**: performs semantic analysis (type checking, variable scoping, etc.).
4. **Compiler**: generates bytecode from the AST.
5. **VM**: executes the bytecode and calls native functions for chess operations.

## Example Code

```chessy
func main() {
    Bitboard bb = Bit(0);          // create a bitboard with bit 0 set
    i32 count = PopCount(bb);      // count the bits
    Print(count);                  // prints 1
}
```

## Building and Running

### Prerequisites
- CMake (≥ 3.10)
- C++20 compiler (MSVC, GCC, Clang)
- AVX2 support (for SIMD vectors)

### Steps
```bash
mkdir build
cd build
cmake ..
cmake --build .
./Chessy_Lexer
```

The program reads `code/main.chessy` and executes it.

## Tests

Unit tests are provided for the Lexer, Parser, and Analyser.
```bash
./Chessy_Tests
```

They verify the correct functioning of the pipeline on example files located in `PipelineTest/`.

## Project Structure

- `code/`: source code of the compiler and VM.
  - `Lexer.*`, `Parser.*`: lexical and syntax analysis.
  - `Analyser.*`: semantic analysis.
  - `Compiler.*`: bytecode generation.
  - `VM.*`: virtual machine.
  - `ChessNative.*`: implementation of chess built-ins.
  - `Builtins.h`: description of native functions.
- `PipelineTest/`: `.chessy` files used for tests.
- `CMakeLists.txt`: build configuration.

## Authors

Project created in 10 days by two developers passionate about chess and compilation.