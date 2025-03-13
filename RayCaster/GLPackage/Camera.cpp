#include "Camera.h"

namespace GLModule
{
	Camera::Camera()
	{
	}

	bool Camera::initilize()
	{
		updateViewMat4();
		return true;
	}

	void Camera::move(glm::vec3 posOffset)
	{
		setPosition(posOffset, false);
		//std::cout << m_position.x << "," << m_position.y << "," << m_position.z << std::endl;
	}

	void Camera::moveTo(glm::vec3 absPos)
	{
		setPosition(absPos, true);
	}

	void Camera::rotate(double angle, glm::vec3 effectAxis)
	{
		if (effectAxis.x != 0)
		{
			m_pitch += angle;
		}
		if (effectAxis.y != 0)
		{
			m_yaw += angle;
		}
		updateViewMat4();
	}

	void Camera::zoom(double ration)
	{
		m_zoom = ration;
		std::cout << "zoom=" << m_zoom << std::endl;
	}

	void Camera::zoomIn()
	{
		zoom(m_zoom * std::pow(0.9, -1));
	}

	void Camera::zoomOut()
	{
		zoom(m_zoom * std::pow(0.9, 1));
	}

	void Camera::setPosition(glm::vec3 pos, bool isAbsMove)
	{
		if (isAbsMove)
			m_position = pos;
		else
			m_position += pos;
		updateViewMat4();
	}

	double Camera::getZoom()
	{
		return m_zoom;
	}

	glm::vec3 Camera::getPosition()
	{
		return m_position;
	}

	const glm::mat4& Camera::getViewMat4()
	{
		return m_viewMat4;
	}

	void Camera::updateViewMat4()
	{
		auto radPictch = glm::radians(m_pitch);
		auto radYaw = glm::radians(m_yaw);
		double x = glm::cos(radYaw) * glm::cos(radPictch);
		double y = glm::sin(radPictch);
		double z = glm::sin(radYaw) * glm::cos(radPictch);
		m_front = {x, y ,z};
		m_front = glm::normalize(m_front);
		
		m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
		m_up = glm::normalize(glm::cross(m_right, m_front));
		
		m_viewMat4 = glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void Camera::directionControl(double xpos, double ypos)
	{
		auto vecocity = 0.5;
		if (m_lastMouse.x == -99999 && m_lastMouse.y == -99999)
			m_lastMouse = { xpos , ypos };
		auto offX = m_lastMouse.x - xpos;
		auto offY = ypos - m_lastMouse.y;
		offX *= vecocity;
		offY *= vecocity;
		m_lastMouse = glm::vec2{ xpos, ypos };
		m_pitch += offY;
		//std::cout << "pitch=" << m_pitch << std::endl;
		if (std::abs(m_pitch + offY) >= 89)
		{
			m_pitch -= offY;
		}
		m_yaw += offX;
		updateViewMat4();
		setPosition(-m_front * m_distance, true);
	}

	void Camera::pressControl(int button)
	{
		if (button == 0) // left
		{

		}
		else // right
		{

		}
	}

	void Camera::moveControl(int mvoeType)
	{
		auto offset = 0.1 * 3;
		if (mvoeType == 0)
		{
			// up
			move({ m_front.x * offset , m_front.y * offset , m_front.z * offset });
		}
		else if (mvoeType == 1)
		{
			// down
			move({ -m_front.x * offset , -m_front.y * offset , -m_front.z * offset });
		}
		else if (mvoeType == 2)
		{
			// left
			move({ -m_right.x * offset , -m_right.y * offset , -m_right.z * offset });
		}
		else if (mvoeType == 3)
		{
			// right
			move({ m_right.x * offset , m_right.y * offset , m_right.z * offset });
		}
		//std::cout << "mvoeType=" << mvoeType << std::endl;
	}
};
