# GLTF Viewer
A little program to preview GLTF models with a few custom settings

Built with GLFW3, Glad, GLM and nholmann/json

### Build
GLTF Viewer uses CMake to build, allowing it to be used across multiple systems.
There are a few ways you can build this project depending on what system and toolsets you have available:
- [Vcpkg (Recommended)](#Vcpkg)
- [Windows](#Windows)
- [Linux/MacOS](#Linux/MacOS)

#### Vcpkg
If you use Microsoft's [Vcpkg](https://github.com/Microsoft/vcpkg), first install all the dependencies:
```
.\vcpkg install glfw3 glm nholmann/json 
```
Then build appending `-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake -DUSE_VCPKG` to the command:
```
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake --build . 
```

#### Windows
GLTF Viewer doesn't include some libraries in the repo, instead preferring for the user to download them.

#### Linux/MacOS
By default, this project will load `GLM`, `GLFW3` and `nholmann/json` using `find_package` which require them to be installed. 
please see their respective pages on how to install them.

You can disable this behaviour and load a local version instead by setting the options `USE_GLM_PACKAGE`, `USE_GLFW_PACKAGE`, `USE_JSON_PACKAGE` to `OFF` respectively.
You'll then need to download these files manually however this can be done automatically by running `setup.sh`