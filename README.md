#GLTF Viewer

###Build
To Build GLFTViewer, there are a few options. You'll need the following installed:
- CMake 3.10
- GCC

####Linux/MacOS
This project supports loading libraries via `find_package` and by default, will look for `GLM` and `GLFW3 3.2.1`. 
You can disable this behaviour and load a local version instead by setting the options `USE_GLM_PACKAGE` and `USE_GLFW_PACKAGE` to `OFF` respectively.

If loading local versions of the libraries, you'll need to install them by running `setup.sh` or manually downloading them.