# OpenGL, SOIL, GLFW playground

![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl) ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

## Maintenace Status :wrench:
![maintenance-status](https://img.shields.io/badge/maintenance-passively--maintained-yellowgreen.svg)
## CI Build Status :hammer:
[![GLFW Build](https://github.com/OudomMunint/GLFW/actions/workflows/main.yml/badge.svg)](https://github.com/OudomMunint/GLFW/actions/workflows/main.yml) 

## CodeQL :mag:
[![CodeQL](https://github.com/OudomMunint/GLFW/actions/workflows/codeql.yml/badge.svg)](https://github.com/OudomMunint/GLFW/actions/workflows/codeql.yml)

## GLFW &amp; SOIL Playground
# Dependencies / Requirements
- `GLFW` = `OpenGL 3+`
- `GLEW` = `OpenGL 3+`
- `SOIL` = `OpenGL 3+`
- `GLSL 3.3+`
- GPU that supports `OpenGL 3+` i.e. NVIDIA GeForce 8 series+ or ATI Radeon HD 2000 series+
- GPU with `shader model 4.0+`
- MSBuild: Any version of MSBuild.

## Getting started
### !!! This project will not run on mac
### First time setup
- Clone the repository
- Download [GLFW](https://www.glfw.org/download.html) and [SOIL](https://www.lonesock.net/soil.html)
- Extract the files and copy the `include` folder to `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\include`
- Copy the `lib` folder to `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib`
- Copy the `dll` folder to `C:\Windows\System32`
- Open the project in Visual Studio and build the project
- If MSBuild is not installed, install it from [here](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)
- Or modify visual studio installer and install MSBuild
- Open the project in Visual Studio and build the project
- Set the startup project to `GLFW`
- Run the project
- If project fails to build with dependency errors, check the `include` and `lib` directories or try running NuGet package restore with `Ctrl + Shift + R`
- If build is successful, there should be a window opened with a triangle in it. <br> <br>
<img src="triangle.png"/>
