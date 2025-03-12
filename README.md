# OpenGL / SOIL / GLFW starting project template
## Feel free to use this as a template for your own projects.

![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl) ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

## Maintenance Status :wrench:
![maintenance-status](https://img.shields.io/badge/maintenance-as--is-yellow.svg)
## CI Build Status :hammer:
[![GLFW Build](https://github.com/OudomMunint/GLFW/actions/workflows/main.yml/badge.svg)](https://github.com/OudomMunint/GLFW/actions/workflows/main.yml) 

## CodeQL :mag:
[![CodeQL](https://github.com/OudomMunint/GLFW/actions/workflows/codeql.yml/badge.svg)](https://github.com/OudomMunint/GLFW/actions/workflows/codeql.yml)

# Dependencies / Requirements
- `GLFW` = `OpenGL 3+`
- `GLEW` = `OpenGL 3+`
- `SOIL` = `OpenGL 3+`
- `GLSL 3.3+`
- GPU that supports `OpenGL 3+` i.e. NVIDIA GeForce 8 series+ or ATI Radeon HD 2000 series+
- GPU with `shader model 4.0+`
- MSBuild: Any version of MSBuild.

## Getting started
### This project will not run on Mac, Visual Studio for Mac doesn't support this project
### A CI build pipeline is setup for this project, as well as a create release step, CodeQL C++ code scanning is also setup for this project. Find the workflow files in the `.github/workflows` folder. Refer to the badges above for the build and scan status.
### First time setup
- Clone the repository
- Download [GLFW](https://www.glfw.org/download.html) and [SOIL](https://www.lonesock.net/soil.html)
- Extract the files and copy the `include` folder to `C:\Visual Studio\{Year}\{Version}\VC\Tools\MSVC\{Build}\include`
- Copy the `lib` folder to `C:\Visual Studio\{Year}\{Version}\VC\Tools\MSVC\{Build}\lib`
- Copy the `dll` folder to `C:\Windows\System32`
- Open the project in Visual Studio and build the project
- If MSBuild is not installed, modify visual studio installer and install MSBuild
- Open the project in Visual Studio and build the project
- Set the startup project to `GLFW`
- Run the project in `debug` mode
- If project fails to build with dependency errors, try to run NuGet package restore
- If build is successful, there should be a window opened with a triangle & an fps counter. <br> <br>

### VsCode
- Install [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) extension
- Generate the `launch.json` and `tasks.json` files by pressing `F5` and select `C++ (GDB/LLDB)`
- Run the project in `debug` mode
<br> <br>
<img src="/Media/window.png"/>
<img src="/Media/triangle.png"/>