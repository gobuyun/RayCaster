#ifndef _UICONTROL_H_
#define _UICONTROL_H_

#include <string>

#include "GL.h"
#include <GLFW/glfw3.h>

class Scene;

using KeyboardControl = void (int);
using MouseMoveControl = void(double, double);
using WheelControl = void(double, double);
using MousePressControl = void(int, double, double);


class UIControl
{
public:
	UIControl(const char* title, int width = SCR_WIDTH, int height = SCR_HEIGHT);
	~UIControl();

	// life cycle
	bool initialize();
	void exec();
	// set status
	void setTitle(const std::string& title);
	void setScene(Scene* scene);
	void setKeyboardControlCallBack(std::function<KeyboardControl> callback);
	void setMouseMoveControl(std::function<MouseMoveControl> callback);
	void setMousePressControl(std::function<MousePressControl> callback);
	void setWheelControl(std::function<WheelControl> callback);
	

private:
	void processInput(GLFWwindow* window);
	// mode 为0则是rotate模式，为1则是click模式
	void switchMouseInputMethod(bool bRotateMode); 

private:
	// settings
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	GLFWwindow* m_window = nullptr;
	std::function<KeyboardControl> m_keyboardControl = nullptr;
	std::string m_title;
	bool m_isInitialize = false;
	int m_initWidth;
	int m_initHeight;
};


#endif