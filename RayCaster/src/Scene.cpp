#include "Scene.h"

using namespace std;

Scene::Scene(double width, double height)
{
	m_camera.reset(new GLModule::Camera());
	resizeWindow(width, height);
}

Scene::Scene()
{
	m_camera.reset(new GLModule::Camera());
}

bool Scene::initialize()
{
	for (auto& it : m_loadedShaders)
	{
		if (it.second->initialize() == false)
			return false;
	}
	for (auto& it : m_renderObjects)
	{
		if (it.modelIns->initialize() == false)
			return false;
	}
	if (!m_camera->initilize())
		return false;
	return true;
}

void Scene::resizeWindow(double width, double height)
{
	m_winSize = {width, height};
	resetProjectionMat4(width, height);
}

void Scene::moveControl(int direction)
{
	m_camera->moveControl(direction);
}

void Scene::pressControl(int button, double xpos, double ypos)
{
	if (button == 0)
		pickTrigger(xpos, ypos); // ´¥·¢Ê°È¡
}

void Scene::directionControl(double xpos, double ypos)
{
	m_camera->directionControl(xpos, ypos);
}

void Scene::wheelControl(double xpos, double ypos)
{
	if (ypos > 0)
		m_camera->zoomIn();
	else
		m_camera->zoomOut();
	resetProjectionMat4(m_winSize.x, m_winSize.y);

}

void Scene::fillRayCaster(glm::vec3 startPos, glm::vec3 endPos)
{
	for (auto& it : m_renderObjects)
	{
		if (it.modelID == "raycaster")
		{
			it.modelIns->setMeshByPosition({ startPos ,endPos }, GL_LINES);
			break;
		}
	}
}

glm::vec3 Scene::screenPos2WorldPos(double xpos, double ypos, double z)
{
	auto ndcXPos = xpos / m_winSize.x * 2 - 1;
	auto ndcYPos = -(ypos / m_winSize.y * 2 - 1);

	glm::vec4 cursorPoint{ ndcXPos, ndcYPos, z, 1.0 };
	auto viewCoord =
		glm::inverse(m_projectionMat4) *
		cursorPoint;
	//glm::vec3 viewCoord = glm::normalize(cursorWorldPoint / cursorWorldPoint.w);
	auto worldCoord = glm::mat4(glm::inverse(m_camera->getViewMat4())) * viewCoord;
	auto worldCoordVec3 = glm::vec3(worldCoord);
	if (worldCoord.w != 0)
	{
		worldCoordVec3 /= worldCoord.w;
	}

	return worldCoordVec3;
}

std::vector<glm::vec3> Scene::genRay(double xpos, double ypos)
{
	auto rayDir = screenPos2WorldPos(xpos, ypos, 1.0);
	auto startPos = screenPos2WorldPos(xpos, ypos, -1.0);
	return { startPos, rayDir };
}

void Scene::pickTrigger(double xpos, double ypos)
{
	auto rays = genRay(xpos, ypos);
	auto rayStartPos = rays[0];
	auto rayDir = glm::normalize(rays[1]);
	fillRayCaster(rayStartPos, rayDir);
	for(auto& it : m_renderObjects)
	{
		if (it.modelID == "raycaster" || 
			it.modelID == "xAxis" || 
			it.modelID == "yAxis" || 
			it.modelID == "zAxis")
			continue;
		auto& meshs = it.modelIns->getMeshs();
		it.bIsPicked = false;
		for (auto& mesh : meshs)
		{
			auto vertices = mesh.getVertices();
			auto indicies = mesh.getIndices();
			int loopTimes = indicies.size() == 0? vertices.size(): indicies.size();
			// traval all mesh all triangle
			glm::vec3 p1, p2, p3;
			for (auto i = 0; i < loopTimes; i += 3)
			{
				if (indicies.size() == 0)
				{
					p1 = vertices[i].position;
					p2 = vertices[i + 1].position;
					p3 = vertices[i + 2].position;
				}
				else
				{
					p1 = vertices[indicies[i]].position;
					p2 = vertices[indicies[i + 1]].position;
					p3 = vertices[indicies[i + 2]].position;
				}
				float t, u, v;
				if (intersectTriangle(rayStartPos, rayDir,
					p1, p2, p3,
					&t, &u, &v))
				{
					glm::vec3 intpos = 
						rayStartPos + 
						glm::vec3(rayDir.x * t, rayDir.y * t, rayDir.z * t);
					if (pointInTriangle(p1, p2, p3, intpos))
					{
						it.bIsPicked = true;
						break;
					}
				}
			}
			
			if (it.bIsPicked)
				break;
		}
	}
}

// Determine whether a ray intersect with a triangle
// Parameters
// orig: origin of the ray
// dir: direction of the ray
// v0, v1, v2: vertices of triangle
// t(out): weight of the intersection for the ray
// u(out), v(out): barycentric coordinate of intersection
bool Scene::intersectTriangle(const Vector3& orig, const Vector3& dir,
	Vector3& v0, Vector3& v1, Vector3& v2,
	float* t, float* u, float* v)
{
	// E1
	Vector3 E1 = v1 - v0;

	// E2
	Vector3 E2 = v2 - v0;

	// P
	Vector3 P = glm::cross(dir, E2);

	// determinant
	float det = glm::dot(E1, P);

	// keep det > 0, modify T accordingly
	Vector3 T;
	if (det > 0)
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0001f)
		return false;

	// Calculate u and make sure u <= 1
	*u = glm::dot(T, P);
	if (*u < 0.0f || *u > det)
		return false;

	// Q
	Vector3 Q = glm::cross(T,E1);

	// Calculate v and make sure u + v <= 1
	*v = glm::dot(dir, Q);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = glm::dot(E2,Q);

	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}

bool Scene::pointInTriangle(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
{
	Vector3 v0 = C - A;
	Vector3 v1 = B - A;
	Vector3 v2 = P - A;

	float dot00 = glm::dot(v0,v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly
	{
		return false;
	}

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly
	{
		return false;
	}

	return u + v <= 1;
}

bool Scene::pickTriangle(std::vector<GLModule::VertexNS::Vertex> trangle, glm::vec3 rayDir, glm::vec3 startPos)
{
	// 1¡¢rayInterectPoint = rayStartPos + t * rayDir;
	// 2¡¢normal dot (rayStartPos + t * rayDir - v1) = 0
	// =>n.rayStartPos + n.(t*rayDir) - n.v1 = 0
	// =>t = n.(v1 - rayStartPos) / n.rayDir
	// t >= 0? (in plane) : (not in plane)
	auto normal = trangle[0].normal;
	auto p = trangle[0].position;
	float s = glm::dot(normal, rayDir);
	if (s == 0)
		return false;
	auto rayRationVariables = glm::dot(startPos - p, -normal) / s;
	if (rayRationVariables < 0)
		return false;
	auto interactPoint = startPos + rayRationVariables * rayDir;
	// judge in triangle
	auto p1 = trangle[0].position;
	auto p2 = trangle[1].position;
	auto p3 = trangle[2].position;
	auto direction1 = glm::dot(interactPoint - p1, p2 - p1);
	auto direction2 = glm::dot(interactPoint - p2, p3 - p2);
	auto direction3 = glm::dot(interactPoint - p3, p1 - p3);
	bool res1 = direction1 + std::abs(direction1) == 0;
	bool res2 = direction2 + std::abs(direction2) == 0;
	bool res3 = direction3 + std::abs(direction3) == 0;
	if (res1 == res2 && res1 == res3 && res2 == res3)
		return true;
	return false;
}

void Scene::resetProjectionMat4(double width, double height)
{
	double halfH = (height / 2) / m_camera->getZoom();
	double ratio = width / height;
	double halfW = halfH * ratio;
	double zFar = 10000;
	m_projectionMat4 = glm::ortho(-halfW, halfW, -halfH, halfH, 1.0, zFar);
}

void Scene::setShaderSource(const std::string& filePath, const std::string& shaderName)
{
	auto findIter = m_loadedShaders.find(filePath);
	// not loaded shader
	if (findIter == m_loadedShaders.end())
	{
		std::unique_ptr<GLModule::Shader> shader(new GLModule::Shader());
		shader->setShaderSource(filePath);
		m_loadedShaders.insert({shaderName, std::move(shader)});
	}
}

void Scene::appendModel(const std::string& modelPath, const std::string& shaderName, const std::string modelType)
{	
	std::unique_ptr<GLModule::Model> modelIns(new GLModule::Model(modelPath));
	auto renderObj = RenderObject{ std::move(modelIns), shaderName };
	renderObj.modelID = modelType;
	m_renderObjects.push_back(std::move(renderObj));
}

void Scene::appendMutableModel(const std::string& modelID, const std::string& shaderName)
{
	std::unique_ptr<GLModule::Model> modelIns(new GLModule::Model("CustomModel"));
	auto renderObj = RenderObject{ std::move(modelIns), shaderName };
	renderObj.modelID = modelID;
	m_renderObjects.push_back(std::move(renderObj));
}

void Scene::appendModel(
	vector<GLModule::VertexNS::Vertex> vertices,
	vector<unsigned int> indices,
	const std::string& shaderName,
	GLenum drawType)
{
	std::unique_ptr<GLModule::Model> modelIns(new GLModule::Model(vertices, indices, {}, drawType));
	m_renderObjects.push_back({ std::move(modelIns), shaderName });
}

void Scene::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMat4 = m_camera->getViewMat4();
	for (auto& renderObject : m_renderObjects)
	{
		auto& pShader = m_loadedShaders[renderObject.useShaderName];
		pShader->use();
		auto modelViewMat4 = viewMat4 * renderObject.transform;
		pShader->setUniformMat4("modelViewMat4", modelViewMat4);
		pShader->setUniformMat4("projectionMat4", m_projectionMat4);
		pShader->setUniformMat4("normalMat4", glm::transpose(glm::inverse(modelViewMat4)));
		pShader->setUniformInt("picked", renderObject.bIsPicked? 1:0);
		renderObject.modelIns->draw(*m_loadedShaders[renderObject.useShaderName]);
	}
}
