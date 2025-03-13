#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "CommonDefine.hpp"


namespace GLModule
{
	class Camera
	{
	public:
		Camera();
		// life cycle
		bool initilize();
		void moveControl(int direction);
		void pressControl(int button);
		void directionControl(double xpos, double ypos);

		// transform
		void move(glm::vec3 posOffset);
		void moveTo(glm::vec3 absPos);
		void rotate(double angle, glm::vec3 effectAxis);
		void zoom(double ration);
		void zoomIn();
		void zoomOut();

		// status
		double getZoom();
		glm::vec3 getPosition();
		const glm::mat4& getViewMat4();

	private:
		void updateViewMat4();
		void setPosition(glm::vec3 pos, bool isAbsMove);

	private:
		double m_zoom = 1.0f;
		double m_yaw = -90;
		double m_pitch = 0;
		float m_distance = 1000.0f;
		glm::mat4 m_viewMat4;

		glm::vec3 m_position = {0, 0, m_distance};
		glm::vec2 m_lastMouse = {-99999, -99999};
		glm::vec3 m_up = WORLD_UP;
		glm::vec3 m_front = WORLD_FRONT;
		glm::vec3 m_right = WORLD_RIGHT;

		const glm::vec3 WORLD_UP{0.0, 1.0, 0.0};
		const glm::vec3 WORLD_FRONT{0.0, 0.0, -1.0};
		const glm::vec3 WORLD_RIGHT{1.0, 0.0, 0.0};
	};
}

#endif