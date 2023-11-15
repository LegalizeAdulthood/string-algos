# string-algos
Boost string algorithm examples for the video [Boost String Algorithms Library](https://www.youtube.com/watch?v=23eXt2EuMLM).
The code builds a tool called `Stringer` that can operate on text files to perform the following commands:
- `tolower` convert text to lower case
- `toupper` convert text to upper case
- `breakline` fold lines longer than 80 columns intelligently

The video/code demonstrates a little bit of test-driven development advice:
- How to unit test logic in `main`
- How to mock a `std::function<>` with google mock

The code obtains the boost string algorithms library from [`vcpkg`](https://vcpkg.io/en/) with a [manifest](https://learn.microsoft.com/en-us/vcpkg/concepts/manifest-mode),
vcpkg in a [git submodule](https://git-scm.com/docs/git-submodule) and a [CMake preset](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) to specify
the location of the vcpkg toolchain file.

# Building the Code

You need CMake 3.23 or later installed to build this code.

After cloning this repository, the following commands should build the code:

```
git submodule init
git submodule update
cmake --preset default
cmake --build ../build
```

You can specify a generator after `cmake --preset default` if you don't like the default generator for your platform.

The default build configuration (Debug, Release, etc.) varies by the type of generator used.
See the documentation on [CMake](https://cmake.org) for more details.
