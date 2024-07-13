# CG - Solar System Model

## Table of Contents

- [Description](#description)
- [Project Phases](#project-phases)
- [Dependencies](#dependencies)
    - [Ubuntu / Debian-based Linux](#ubuntu--debian-based-linux)
    - [Fedora / Red Hat-based Linux](#fedora--red-hat-based-linux)
    - [Arch-based Linux](#arch-based-linux)
    - [MacOS (via Homebrew)](#macos-via-homebrew)
    - [Windows (via vcpkg)](#windows-via-vcpkg)
- [Compilation](#compilation)
- [Execution](#execution)
- [Reports](#reports)
- [Authors](#authors)

## Description

The purpose of this project was to create a really complete and detailed model of the Solar System. This model includes the Sun, the 8 planets, many of their moons and at least a simple representation of an asteroid. The planets are rotating around the Sun, the moons are rotating around their respective planets and the asteroid is rotating around the Sun. The planets and moons are rotating in the correct direction and at the correct speed and all of these movements are made using some mathematical formulas, like Catmull-Rom splines, to make the movements more realistic.

## Project Phases

The project, as you can see by the files in the repository, is divided in 4 phases. Each phase is a different version of the project, with more features and more complexity than the previous one. What each phase asked to be done is described in the [Project Phases](./Enunciado.pdf) file.

## Dependencies

This project requires a few dependencies to be installed in order to run correctly. First of all, this project was made using OpenGL and C++, so you need to have a C++ compiler and the OpenGL libraries installed in your computer. Then you also need to have libraries like  `GLEW`, `GLM` and many others installed in your computer. To install these libraries you can use the following commands:

### Ubuntu / Debian-based Linux:

```bash
$ sudo apt-get update
$ sudo apt-get install build-essential cmake libglew-dev libglm-dev libdevil-dev libtinyxml2-dev freeglut3-dev
```

### Fedora / Red Hat-based Linux:

```bash
$ sudo dnf install gcc gcc-c++ cmake glew-devel glm-devel devil-devel tinyxml2-devel freeglut-devel
```

### Arch-based Linux:

```bash
$ sudo pacman -S base-devel cmake glew glm devil tinyxml2 freeglut
```

### MacOS (via Homebrew):

```bash
$ brew install cmake glew glm devil tinyxml2 freeglut
```

### Windows (via vcpkg):

```bash
$ vcpkg install glm glew devil tinyxml2 freeglut
```

## Compilation

To compile the project you need to run the following commands:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Execution

To understand how to run the project, some context is needed.
There are 2 main executables in the project: `generator` and `engine`. The `generator` is used to generate the `.3d` files that are used by the `engine` to render the scene. The `engine` is used to render the scene using the `.3d` files generated by the `generator`. The `generator` receives some commands and arguments to generate the `.3d` files and the `engine` receives the path to a `.xml` file that is used to configure the scene.

In the repository there is a folder in each phase called `tests`. In this folder there are some `.xml` files that are used to configure the project. These files are used to configure the initial position of the camera, the initial position of the geometric figures, the colors of the figures, the textures of the figures, the lights of the scene, etc. To run the differents tests for each phase you need to go to each file in the `tests` and see the comments in the file to understand which commands you need to run to see the results of the tests. For example, if you go to the `tests` folder of the first phase and open the [teste1](./fase1/tests/test_files_phase_1/test_1_1.xml) file you will see the following comment: `generator cone 1 2 4 3 cone_1_2_4_3.3d`. This means that you need to run the given command to create the `.3d` file that will be used by the engine to render the scene. So, to run the first test you need to run the following commands (remember that you need to be in the phase folder and build the project before running the commands):

```bash
$ ./bin/generator cone 1 2 4 3 cone_1_2_4_3.3d
$ ./bin/engine /tests/test_files_phase_1/test_1_1.xml
```

In the case of the demos, all the files needed to run the demos are already in the repository. So, to run the demos you just need to run the following commands:

```bash
$ ./bin/engine /demos/solarSystem.xml
```

Do not forget that some of the phases may not have the demos implemented, since the demos were implemented in the last phases of the project.

## Reports

For each phase of the project there is a report that explains what was done in that phase and what was the main goal of that phase. These reports are inside the `reports` folder.

- [Phase 1 Report](./reports/CG-Report-Phase1.pdf)
- [Phase 2 Report](./reports/CG-Report-Phase2.pdf)
- [Phase 3 Report](./reports/CG-Report-Phase3.pdf)
- [Phase 4 Report](./reports/CG-Report-Phase4.pdf)

## Authors

- [João Coelho](https://github.com/JoaoCoelho2003) - **A100596**
- [José Rodrigues](https://github.com/FilipeR13) - **A100692**
- [Duarte Araújo](https://github.com/duartearaujo) - **A100750**
