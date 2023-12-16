[![ci](https://github.com/michaeladler/aoc-2023/actions/workflows/ci.yml/badge.svg)](https://github.com/michaeladler/aoc-2023/actions/workflows/ci.yml)

# 🎄 Advent of Code 2023 Solutions 🎄

Welcome to my repository where I share my solutions to the [Advent of Code 2023](https://adventofcode.com/2023) challenges, tackled using the **good old C programming language**.
Advent of Code is an annual December event where programmers solve fun, holiday-themed puzzles.

## 🛠 Usage

Each puzzle solution is compiled into its own standalone executable, named in accordance with its corresponding day.
For instance, the executable for the first puzzle is `day01`.
By default, these executables search for their respective input files located at `input/dayXX.txt`, where `XX` represents the two-digit day number (e.g., `01` for Day 1).
Alternatively, you have the option to provide input via a command-line argument.
For example, you can run `./day01 mine.txt` to specify a different input file.

## 🏗 Building and Running Tests

Each puzzle in this repository is accompanied by tests, which are designed using both the examples provided in the puzzle and the actual `input.txt` file.
Since sharing the puzzle input data is discouraged, the **inputs are not contained in this repository**.

**Note**: Tests using the actual `input.txt` have to be enabled at compile time with the `-Dhave-inputs=true` option (this only make sense if you have my `input.txt` files since your answers will differ from mine).

### Release Build

```bash
meson setup --buildtype=release -Db_lto=true -Dhave-inputs=true build
ninja -C build test
```

### Debug Build

```bash
meson setup --buildtype=debugoptimized -Db_sanitize=address -Db_lundef=false -Dhave-inputs=true build_debug
ninja -C build_debug test
```

## ⏱ Performance Benchmarks

The solutions were tested on different processors using [hyperfine](https://github.com/sharkdp/hyperfine) and/or [poop](https://github.com/andrewrk/poop).
Compiled using clang 16 and LTO.


| Day | Intel i7-11850H | AMD Ryzen 5 PRO 4650U |
| --- | --------------- | --------------------- |
| 1   | 276 µs          | 520 µs                |
| 2   | 185 µs          | 434 µs                |
| 3   | 231 µs          | 501 µs                |
| 4   | 368 µs          | 679 µs                |
| 5   | 1.4 s           | 2.6 s                 |
| 6   | 200 µs          | 472 µs                |
| 7   | 2.6 ms          | 3.54 ms               |
| 8   | 462.4 µs        | 1 ms                  |
| 9   | 256 µs          | 464 µs                |
| 10  | 5.8 ms          | 16.4 ms               |
| 11  | 980 µs          | 1.7 ms                |
| 12  | 347 ms          | 554 ms                |
| 13  | 248 µs          | 486 µs                |
| 14  |                 | 53.4 ms               |
| 15  |                 | 1 ms                  |
| 16  |                 | 33.5 ms               |

## 🙏 Acknowledgments and Resources

Special thanks to the authors of these very useful C resources:

- [Cauldron](https://github.com/camel-cdr/cauldron) - A collection of useful C code snippets and utilities.
- [C Algorithms](https://github.com/fragglet/c-algorithms) - A library of common data structures and algorithms written in C.
- [CTL](https://github.com/glouw/ctl/) - A library for container types for C.
- [CTest](https://github.com/bvdberg/ctest) - A unit testing framework for C.
- [Log.c](https://github.com/rxi/log.c) - A simple logging library implemented in C.
- [Sort](https://github.com/swenson/sort/) - A collection of sorting algorithms implemented in C.
- [str](https://github.com/maxim2266/str) - yet another string library for C language.
- [xxhash](https://github.com/Cyan4973/xxHash) - Extremely fast non-cryptographic hash algorithm.
- [slice99](https://github.com/Hirrolot/slice99) -  Memory slices for C99.
