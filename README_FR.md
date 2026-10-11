# Chessy - DSL pour les échecs

Chessy est un langage dédié (DSL) permettant d'écrire des programmes pour les échecs. Il a été conçu et implémenté en **10 jours** par **2 développeurs**. Le langage offre des types et des fonctions natives spécifiques aux échecs (bitboards, pièces, cases, coups) tout en restant simple et expressif.

## Fonctionnalités principales

- **Types natifs** : `Bool`, `I8`..`I64`, `U8`..`U64`, `String`, `Bitboard`, `Piece`, `Square`, `Move`, `Board`, ainsi que des vecteurs SIMD (`I32x8`, `I64x4`).
- **Opérations** : arithmétiques, logiques, binaires, comparaisons, décalages.
- **Structures de contrôle** : `if`, `do-while`, `return`.
- **Fonctions utilisateur** avec paramètres et valeur de retour.
- **Builtins pour les échecs** : génération de coups, calcul d'attaques, manipulation de bitboards, affichage, etc. (voir `Builtins.h`).
- **Compilation en bytecode** et exécution sur une machine virtuelle (VM).

## Architecture

Le projet suit un pipeline de compilation classique :

1. **Lexer** : transforme le code source en une liste de tokens.
2. **Parser** : construit un AST (arbre de syntaxe abstraite) à partir des tokens.
3. **Analyser** : effectue l'analyse sémantique (vérification des types, portée des variables, etc.).
4. **Compiler** : génère un bytecode à partir de l'AST.
5. **VM** : exécute le bytecode et appelle les fonctions natives pour les opérations d'échecs.

## Exemple de code

```chessy
func main() {
    Bitboard bb = Bit(0);          // crée un bitboard avec le bit 0
    i32 count = PopCount(bb);      // compte les bits
    Print(count);                  // affiche 1
}
```

## Compilation et exécution

### Prérequis
- CMake (≥ 3.10)
- Compilateur C++20 (MSVC, GCC, Clang)
- Support AVX2 (pour les vecteurs SIMD)

### Étapes
```bash
mkdir build
cd build
cmake ..
cmake --build .
./Chessy_Lexer
```

Le programme lit le fichier `code/main.chessy` et l'exécute.

## Tests

Des tests unitaires sont fournis pour le Lexer, le Parser et l'Analyser.
```bash
./Chessy_Tests
```

Ils vérifient le bon fonctionnement du pipeline sur des fichiers d'exemple situés dans `PipelineTest/`.

## Structure du projet

- `code/` : code source du compilateur et de la VM.
  - `Lexer.*`, `Parser.*` : analyse lexicale et syntaxique.
  - `Analyser.*` : analyse sémantique.
  - `Compiler.*` : génération de bytecode.
  - `VM.*` : machine virtuelle.
  - `ChessNative.*` : implémentation des builtins d'échecs.
  - `Builtins.h` : description des fonctions natives.
- `PipelineTest/` : fichiers `.chessy` utilisés pour les tests.
- `CMakeLists.txt` : configuration de la compilation.

## Auteurs

Projet réalisé en 10 jours par deux développeurs passionnés d'échecs et de compilation.