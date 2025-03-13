#ifndef __WRAPPER_HPP__
#define __WRAPPER_HPP__

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"
#include "stb_image/stb_image.h"

namespace GLModule
{
    inline bool __initGlad(GLADloadproc proc)
    {
        if (!gladLoadGLLoader(proc))
        {
            //std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }
        return true;
    }
};

#endif