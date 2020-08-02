# Raytracing
![AppVeyor](https://img.shields.io/appveyor/build/florianvazelle/raytracing?style=flat&logo=appveyor) [![License MIT](https://img.shields.io/badge/license-MIT-green)](https://choosealicense.com/licenses/mit/)

A from scratch C++ ray tracing library, cross-platform, with a matrix representation of entities.

<h4 align="center">
    <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00000.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00002.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00004.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00003.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00005.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00001.png" width="15%"/>
</h4>

## Getting the code

```bash
git clone --recursive git@github.com:florianvazelle/raytracing.git
```

## Quickstart

Linux : `./setup.sh`

Windows : `setup.bat` # or double-click

## Project Structure

```
.
├── assets
│  ├── samples
│  └── scenes
├── build                   # Compiled files
├── ext                     # Submodules
│  ├── googletest
│  ├── jsoncpp
│  ├── nanogui
│  └── stb
├── include
│  └── rtx
├── src                     # Source files
│  ├── raytracer            # Raytracer implementation
│  └── rtx                  # Raytracing library
├── test                    # Automated tests
└── README.md
```

- [Nanogui](https://github.com/wjakob/nanogui) for user interface.
- [Jsoncpp](https://github.com/open-source-parsers/jsoncpp) for scene parser.
- [STB](https://github.com/nothings/stb) for write PNG/JPG images.
- [Googletest](https://github.com/google/googletest) for unit tests.

##  Requirements

- C++11 compiler.
- [CMake](https://cmake.org) for build system creation.
