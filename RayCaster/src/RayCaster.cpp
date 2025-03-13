#include "RayCaster.h"
#include "Scene.h"
#include "UIControl.h"

#include <iostream>

RayCaster::RayCaster(int width, int height)
{
	m_uicontrol = decltype(m_uicontrol){new UIControl("", width, height)};
	m_scene = decltype(m_scene){new Scene(width, height)};
}

RayCaster::~RayCaster()
{

}

bool RayCaster::initialize()
{
#define CHECK_RESULT(caller, moduleName) \
std::cout << "initialize " << moduleName << " "; \
if (caller() == false) {std::cout << "failed" <<std::endl; return false;} \
else std::cout << "successful" <<std::endl;



	CHECK_RESULT(m_uicontrol->initialize, "UIControl");
	CHECK_RESULT([&]()->bool {return GLModule::initilize((GLADloadproc)glfwGetProcAddress); }, "GLModule");
	CHECK_RESULT(m_scene->initialize, "Scene");

	m_uicontrol->setScene(m_scene.get());
	m_uicontrol->setKeyboardControlCallBack([&](int type){
		m_scene->moveControl(type);
	});
	m_uicontrol->setMouseMoveControl([&](double xpos, double ypos){
		m_scene->directionControl(xpos, ypos);
	});
	m_uicontrol->setMousePressControl([&](int button, double xpos, double ypos) {
		m_scene->pressControl(button, xpos, ypos);
		});
	m_uicontrol->setWheelControl([&](double xpos, double ypos) {
		m_scene->wheelControl(xpos, ypos);
	});

	return true;
}

// set status
void RayCaster::setWinTitle(const std::string& title)
{
	m_uicontrol->setTitle(title);
}

void RayCaster::setShaderSource(const std::string& shaderPath, const std::string& shaderName)
{
	m_scene->setShaderSource(shaderPath, shaderName);
}
void RayCaster::appendMutableModel(const std::string& modelID, const std::string& shaderName)
{
	m_scene->appendMutableModel(modelID, shaderName);
}

void RayCaster::appendModel(const std::string& modelPath, const std::string& shaderName, const std::string& modelType)
{
	m_scene->appendModel(modelPath, shaderName, modelType);
}

void RayCaster::appendModel(
	std::vector<GLModule::VertexNS::Vertex> vertices, 
	std::vector<unsigned int> indices,
	const std::string& shaderName, 
	GLenum drawType)
{
	m_scene->appendModel(vertices, indices, shaderName, drawType);
}

void RayCaster::Run()
{
	m_uicontrol->exec();
}
