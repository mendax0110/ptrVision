# PointerVision

PointerVision is a static analysis tool designed to help developers analyze C++ code for pointer and reference usage, specifically focusing on pointer arithmetic, dereferencing, references, memory allocations, and bitwise operations. It utilizes Clang's tooling to provide a detailed analysis of C++ code and detect various operations related to pointers (`*`), references (`&`), memory management (`new`, `delete`), and bitwise operations (`&`).

## Features

- **Pointer Arithmetic Detection:** Detects pointer arithmetic (e.g., `ptr + offset`).
- **Dereference Detection:** Identifies pointer dereferencing (e.g., `*ptr`).
- **Reference Detection:** Detects references and their usage.
- **Memory Management:** Detects the usage of `new` and `delete` for memory allocation/deallocation.
- **Bitwise AND Detection:** Identifies the usage of the bitwise AND operator (`&`).
- **Goto Usage:** Tracks the use of `goto` in code.

## How to Use

### 1. Clone the Repository

``` bash
git clone https://github.com/mendax0110/ptrVision.git
```

### 2. Navigate to the Project Directory

``` bash
cd pointy
```

### 3. Create a Build Directory

``` bash
mkdir build
cd build
```

### 4. Run CMake

``` bash
cmake ..
```

### 5. Build the Project

``` bash
make
```

### 6. Run the Executable

``` bash
./pointy <path-to-source-file> or ./pointy <path-to-source-directory>
```
