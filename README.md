# BTAS - A BitTree Algorithm Set to Process Large-Scale Dataset Efficiently

*Up-to 1000x faster than the performance of industrial standard databases, able to handle 1 billion elements with randomness of 4,294,967,296!*

# 1. Technical Review

We strongly suggest you to read the [Technical Review](./TECHNICAL_REVIEW.md).

# 2. Brief Intro

**Program Name**: Filter-Uniq-Ints

**Purpose**: filter out unique integers from an **unsorted** integer array. E.g. {1,2,1,3} should be filtered to {1,2,3}

**License**: MIT

# 3. How-To: Build, Run, and Use

## 3.1 Build

### 3.1.1 Prerequisites

You need a C compiler to build the `c-branch`. For the `cpp-branch`, you also need [CMake](https://cmake.org/) > **3.20**.

- For Microsoft Windows users, [mingw-w64](https://sourceforge.net/projects/mingw-w64/) is recommended
- For GNU/Linux Distro or other *nix users, the [GNU Compiler Collections](https://gcc.gnu.org/), known as gcc, is a perfect one
- For macOS users, [clang](https://clang.llvm.org/) is easy to install and use (brew is not needed to install clang on macOS).

### 3.1.2 Build Guide

**NOTE**: Please choose a branch before build this project.

1. Use git to clone this code: `git clone https://github.com/zhenrong-wang/filter-uniq-ints.git`
2. Change your directory: `cd filter-uniq-ints`

**For the c-branch**

3. a. Build with the C compiler: `gcc *.c -Ofast -Wall -o YOUR_PROGRAM_NAME`

**For the cpp-branch:**

3. b. Configure CMake: `cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release`
4. b. Build command example: `cmake --build cmake-build-release --config Release --target all`
5. b. Change your directory: `cd cmake-build-release`
6. b. Build with make: `make`

## 3.2 Run

Command Format: `cmd argv[1] argv[2]`

- `argv[1]`: A string to specify an integer as the number of elems input. E.g. 10032 
- `argv[2]`: A string to specify an integer as the maximum random number generated. E.g. 1000

# 4 Bugs and Communications

Any bugs or problems found, please submit issues to this repo. We'd be glad to communicate on any issues.
