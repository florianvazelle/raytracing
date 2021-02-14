![Travis](https://img.shields.io/travis/florianvazelle/raytracing?style=flat&logo=travis)
![AppVeyor](https://img.shields.io/appveyor/build/florianvazelle/raytracing?style=flat&logo=appveyor)
![Platform](https://img.shields.io/badge/platform-windows%20%7C%20linux-blue) 
[![License](https://img.shields.io/badge/license-Unlicense-blue.svg)](https://github.com/florianvazelle/raytracing/blob/master/LICENSE)
[![Lines of Code](https://tokei.rs/b1/github/florianvazelle/raytracing?category=code)](https://tokei.rs)

# Raytracing

A CPU ray tracing library, written in C++, cross-platform, with a matrix representation of entities.  
There are two technical parts:

- Ray tracing with shadows, ambient, diffuse (Lambert), specular (Phong), reflections, refractions (Snell-Descartes) with Schlick-Fresnel approximation and simple ambient occlusion implementation.
- C++ with super-sampling, gamma correction and multithreading image calculation.

<h4 align="center">
    <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00000.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00002.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00004.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00003.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00005.png" width="15%"/> <img src="https://florianvazelle.github.io/resources/images/raytracing/scene00001.png" width="15%"/>
</h4>

## Getting the code

```bash
git clone --recursive git@github.com:florianvazelle/raytracing.git
```

## Quickstart

Linux : `./setup.sh`

## Building

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -Bbuild
cmake --build build
./build/bin/compute --help  # or ./build/bin/raytracer
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -Bbuild -DRTX_WITH_APP=OFF
cmake --build build
./build/bin/rtx_test
```

## Project Structure

<pre>
.
├── app                     # Raytracer implementation
│  ├── common
│  ├── compute              # Command-line tool
│  └── raytracer            # User interface app
├── assets
│  ├── samples
│  └── scenes
├── build                   # Compiled files
├── external                # Submodules
│  ├── <a href="https://github.com/jarro2783/cxxopts">cxxopts</a>              # for cli options
│  ├── <a href="https://github.com/google/googletest">googletest</a>           # for unit tests
│  ├── <a href="https://github.com/open-source-parsers/jsoncpp">jsoncpp</a>              # for scene parser
│  ├── <a href="https://github.com/wjakob/nanogui">nanogui</a>              # for user interface
│  └── <a href="https://github.com/nothings/stb">stb</a>                  # for write PNG/JPG images
├── include
│  ├── parser
│  └── rtx                  # Raytracing library
├── source                  # Source files
│  └── parser                  
├── test                    # Automated tests
└── README.md
</pre>

## Roadmap

- Real-time image rendering
- Texture with normal/bump mapping
- Depth of field
- CUDA version
- 3D models support

## Requirements

- C++11 compiler
  - gcc 5.0+
  - clang 5.0+
  - MSVC 2015+
- [CMake](https://cmake.org) for build system creation (>= 3.0)
