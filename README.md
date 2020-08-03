# Raytracing
![Travis](https://img.shields.io/travis/florianvazelle/raytracing?style=flat&logo=travis) ![AppVeyor](https://img.shields.io/appveyor/build/florianvazelle/raytracing?style=flat&logo=appveyor) [![License MIT](https://img.shields.io/badge/license-MIT-green)](https://choosealicense.com/licenses/mit/)

A CPU ray tracing library, written in C++, cross-platform, with a matrix representation of entities.
There are two technical parts:
- ray tracing with shadows, ambient, diffuse (Lambert), specular (Phong), reflections, refractions (Snell-Descartes) with Schlick-Fresnel approximation and simple ambient occlusion implementation.
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

Windows : `setup.bat` # or double-click


## Externals

- [Nanogui](https://github.com/wjakob/nanogui) for user interface.
- [Jsoncpp](https://github.com/open-source-parsers/jsoncpp) for scene parser.
- [STB](https://github.com/nothings/stb) for write PNG/JPG images.
- [Googletest](https://github.com/google/googletest) for unit tests.

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

## Roadmap

- Real-time image rendering
- Texture with normal/bump mapping
- Depth of field

- CUDA version
- 3D models support

##  Requirements

- C++11 compiler.
- [CMake](https://cmake.org) for build system creation.
