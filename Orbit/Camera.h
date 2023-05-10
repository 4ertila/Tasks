#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	glm::vec3 camera_position;
	glm::vec3 camera_target;
	glm::vec3 camera_up;
	glm::mat4 camera_matrix;

	void set_camera_matrix();

public:
	Camera();
	Camera(glm::vec3 camera_position, glm::vec3 camera_target, glm::vec3 camera_up);

	void rotation(float angle, float x, float y, float z);

	void change_position(glm::vec3 new_postition);

	glm::vec3 get_position();
};

