#ifndef _RAYCASTER_H_
#define _RAYCASTER_H_

#include <string>
#include <memory>
#include <functional>

#include "GL.h"

class Scene;
class UIControl;


class RayCaster 
{
public:
	RayCaster(int width, int height);
	~RayCaster();
	
	// set status
	void setWinTitle(const std::string& title);
	void setShaderSource(const std::string& shaderPath, const std::string& shaderName);
	void appendModel(const std::string& modelPath, const std::string& shaderName, const std::string& modelType = "NormalModel");
	void appendMutableModel(const std::string& modelID, const std::string& shaderName);
	void appendModel(
		std::vector<GLModule::VertexNS::Vertex> vertices,
		std::vector<unsigned int> indices,
		const std::string& shaderName,
		GLenum drawType = GL_TRIANGLES);

	// fix flow
	bool initialize();
	void Run();

private:
	std::unique_ptr<UIControl> m_uicontrol;
	std::unique_ptr<Scene> m_scene;
};

#endif