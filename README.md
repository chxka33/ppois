# PPOIS Labs

Репозиторий лабораторных работ по дисциплине «Проектирование программного обеспечения интеллектуальных систем» (PPOIS).

## Structure

| Lab | Language | Contents                                       | Documentation |
| --- | -------- | ---------------------------------------------- | ------------- |
| 1   | C++17    | [Set](./lab1/Set), [Multiset](./lab1/Multiset) | Doxygen       |

## Requirements

- C++17 compatible compiler (`g++` or Apple `clang++`)
- [CMake](https://cmake.org/) 3.14+
- [Catch2 v3](https://github.com/catchorg/Catch2) — unit testing (downloaded automatically by CMake)
- [gcovr](https://gcovr.com/) — code coverage analysis
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) — static analysis
- [Doxygen](https://www.doxygen.nl/) — documentation generation

On macOS the tools can be installed with Homebrew:

```bash
brew install cmake gcovr llvm doxygen
```

## Lab 1

### Set

Implementation of a mathematical set with nested elements. Supported operations: add and remove an element, membership check, union (`+`), intersection (`*`), difference (`-`), power set, parsing from a string such as `{a, {b, c}}`, stream input and output.

Source code is located in `lab1/Set/`: `include/` (header), `src/` (implementation), `tests/` (unit tests), `main.cpp` (console menu).

### Multiset

Implementation of a multiset (a set with repeated elements). It reuses `Set::Element` and supports multiplicity of elements, add and remove (one occurrence or all), sum, intersection, difference, power set, parsing from a string such as `{a, a, b}`, stream input and output.

Source code is located in `lab1/Multiset/` and has the same layout as `Set`.

## Build and run

Both projects are built together from the `lab1` directory using the root `CMakeLists.txt`.

```bash
cd lab1
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Run the console applications:

```bash
./build/Set/set_app
./build/Multiset/multiset_app
```

## Code coverage

The project uses `gcovr`. The CI workflow requires at least 90% line coverage for `Set/src` and `Multiset/src`.

Example:

```bash
gcovr -r . --gcov-executable "xcrun llvm-cov gcov" \
  --filter 'Set/src/' --filter 'Multiset/src/' \
  --print-summary --fail-under-line 90 build
```

Remove old build directories before measuring coverage, otherwise `gcovr` may find stale `.gcno`/`.gcda` files.

## Static analysis

`clang-tidy` analyzes the C++ source code.

Example:

```bash
cd lab1
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
clang-tidy -p build --checks='-*,clang-analyzer-*' \
  Set/src/set.cpp Set/main.cpp \
  Multiset/src/multiset.cpp Multiset/main.cpp
```

On macOS `clang-tidy` from Homebrew is not in `PATH`; call it as `"$(brew --prefix llvm)/bin/clang-tidy"`.

## Documentation

Documentation is generated with [Doxygen](https://www.doxygen.nl/) using the `lab1/Doxyfile` configuration, which covers both `Set` and `Multiset`.

```bash
cd lab1
doxygen Doxyfile
```

Open the generated `html/index.html` file in a browser. Generated files (`html/`, `latex/`) are not stored in the repository.

## CI/CD

GitHub Actions (`.github/workflows/lab1.yml`) runs on macOS and automates the following tasks:

- Configuring and building both projects with CMake.
- Running static analysis with `clang-tidy`.
- Running unit tests with `ctest`.
- Checking line coverage against the 90% threshold.
- Building release executables (`set`, `multiset`) and packing them into an archive.
- Publishing a GitHub Release after a successful workflow run on `main`.

## Releases

Downloadable builds are published on the repository's [GitHub Releases](https://github.com/chxka33/ppois/releases) page.

Release archives contain prebuilt macOS (Apple silicon) executables `set` and `multiset`. Extract the archive and run an executable from a terminal. If macOS blocks an unsigned binary, remove the quarantine attribute first:

## Repository

[GitHub repository](https://github.com/chxka33/ppois)
