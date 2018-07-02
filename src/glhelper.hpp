#ifndef GLTFVIEWER_GLHELPER_H
#define GLTFVIEWER_GLHELPER_H

// Using anon namespace instead of static to prevent violation of ODR and to make it a bit "cleaner"
// See discussion here: http://stackoverflow.com/questions/4726570/deprecation-of-the-static-keyword-no-more
#include <glad/glad.h>
#include <iostream>

namespace {
#ifndef NDEBUG
#define glErrorCheck() errorCheck(__FILE__, __LINE__)
    inline void errorCheck(const char *file, int line) {
        GLuint err = glGetError();
        if (err != GL_NO_ERROR) {
            std::string error;

            switch (err) {
                case GL_INVALID_ENUM:
                    error = "INVALID ENUM";
                    break;
                case GL_INVALID_VALUE:
                    error = "INVALID VALUE";
                    break;
                case GL_INVALID_OPERATION:
                    error = "INVALID OPERATION";
                    break;
                case GL_OUT_OF_MEMORY:
                    error = "OUT OF MEMORY";
                    break;
            }
            std::cerr << "(" << file << ":" << line << ") GL ERROR " << std::hex << err << ": " << error << std::endl;
        }
    }

#else
#define glErrorCheck()
#endif
}

#endif //GLTFVIEWER_GLHELPER_H