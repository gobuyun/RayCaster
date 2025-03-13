#ifndef _GL_H_
#define _GL_H_

#include "CommonDefine.hpp"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Mesh.h"


namespace GLModule
{
	inline bool initilize(GLADloadproc proc)
	{
		return __initGlad(proc);
	}
};

#endif