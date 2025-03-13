#ifndef __SCENE_H__
#define __SCENE_H__

#include "GL.h"

#include <memory>
#include <string>
#include <map>

using Vector3 = glm::vec3;
struct RenderObject
{
	std::unique_ptr<GLModule::Model> modelIns;
	std::string useShaderName;
	glm::mat4 transform;
	bool bIsPicked = false;
	std::string modelID = "CustomModel";
};

class Scene
{
public:
	Scene();
	Scene(double width, double height);
	bool initialize();
	void setShaderSource(const std::string& filePath, const std::string& shaderName);
	void appendModel(const std::string& modelPath, const std::string& shaderName, const std::string modelType);
	void appendMutableModel(const std::string& modelID, const std::string& shaderName);
	void appendModel(std::vector<GLModule::VertexNS::Vertex> vertices, 
		std::vector<unsigned int> indices,
		const std::string& shaderName,
		GLenum drawType = GL_TRIANGLES);
	void draw();
	void resizeWindow(double width, double height);
	void moveControl(int direction);
	void directionControl(double xpos, double ypos);
	void wheelControl(double xpos, double ypos);
	void pressControl(int button, double xpos, double ypos);

private:
	std::vector<glm::vec3> genRay(double xpos, double ypos);
	glm::vec3 screenPos2WorldPos(double xpos, double ypos, double z);
	bool pointInTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 interactPoint);
	bool intersectTriangle(const Vector3& orig, const Vector3& dir,
		Vector3& v0, Vector3& v1, Vector3& v2,
		float* t, float* u, float* v);
	void resetProjectionMat4(double width, double height);
	bool pickTriangle(std::vector<GLModule::VertexNS::Vertex> trangle, glm::vec3 rayDir, glm::vec3 startPos);
	void pickTrigger(double xpos, double ypos);
	void fillRayCaster(glm::vec3 startPos, glm::vec3 endPos);

private:
	std::map<std::string, std::unique_ptr<GLModule::Shader>> m_loadedShaders;
	std::vector<RenderObject> m_renderObjects;
	std::unique_ptr<GLModule::Camera> m_camera;
	glm::mat4 m_projectionMat4;
	glm::vec2 m_winSize;
};

#endif