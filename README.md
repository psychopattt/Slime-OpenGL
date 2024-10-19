# Slime Mold
[Slime mold](https://en.wikipedia.org/wiki/Physarum_polycephalum) simulation using [OCSFW](https://github.com/psychopattt/OCSFW) (GLFW, OpenGL and Dear ImGui)<br>
Demo video available [here](https://youtu.be/3Hl5o2H3vxo)

## Features
- Modify up to 3 slime mold species
  - Amount of cells
  - Spawn mode
  - Move speed and turn speed
  - Trail weight, diffusion rate and decay rate
  - Sensors size, offset and angle
  - Main color and edge color
- Load colony presets
- Copy and load custom colonies
- Generate random colonies (R)
- Modify display wrap mode and filtering
- All features of [OCSFW](https://github.com/psychopattt/OCSFW?tab=readme-ov-file#features)

## Requirements
- An OpenGL 4.6 capable GPU

## How to use
Windows
1. Download the [latest release](https://github.com/psychopattt/Slime-OpenGL/releases/latest) (Slime-Mold.exe)
2. Run the executable

Linux or MacOS
1. Download the project files
3. Replace the included Windows GLFW binaries
   1. Get the [GLFW binaries](https://www.glfw.org/download.html) for your OS
   2. Replace the files in "OCSFW/Libraries/include/GLFW"
   3. Replace the files in "OCSFW/Libraries/lib"
4. Compile and run the project

## Credits
- [OCSFW](https://github.com/psychopattt/OCSFW?tab=readme-ov-file#credits)
- [SebLague](https://github.com/SebLague/Slime-Simulation)
