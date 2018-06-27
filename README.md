# GLTF Viewer
A little program to preview GLTF models with a few custom settings

Built with [GLFW3](http://www.glfw.org), [Glad](https://github.com/Dav1dde/glad), [GLM](https://glm.g-truc.net), [imgui](https://github.com/ocornut/imgui) and [nholmann/json](https://github.com/nlohmann/json)

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

### License
This project is provided under the MIT License. See LICENSE.MD for more information.

#### Library Licenses
Please see the libraries respective directories or links for the full license.
- [*nholmann/json*](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) - MIT License
- [*imgui*](https://github.com/ocornut/imgui/blob/master/LICENSE.txt) - MIT License
- [*GLM*](http://glm.g-truc.net/copying.txt) - The Happy Bunny License (Modified MIT License)
- [*GLFW*](http://www.glfw.org/license.html) - zlib/libpng
- [*Glad*](https://github.com/Dav1dde/glad/blob/master/LICENSE) - MIT License (Generated code is CC0)