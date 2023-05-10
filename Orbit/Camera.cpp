#include "Camera.h"

void Camera::set_camera_matrix()
{
	camera_matrix = glm::lookAt(
		camera_position,
		camera_target,
		camera_up
	);
}

Camera::Camera()
{
	camera_position = glm::vec3(1.0f, 1.0f, 1.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 0.0f, 1.0f);
	set_camera_matrix();
	glLoadMatrixf(glm::value_ptr(camera_matrix));
}

Camera::Camera(glm::vec3 camera_position, glm::vec3 camera_target, glm::vec3 camera_up)
{
	this->camera_position = camera_position;
	this->camera_target = camera_target;
	this->camera_up = camera_up;
	set_camera_matrix();
	glLoadMatrixf(glm::value_ptr(camera_matrix));
}

void Camera::rotation(float angle, float x, float y, float z)
{
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
	glm::vec4 temp_vector = rotation_matrix * glm::vec4(camera_position.x, camera_position.y, camera_position.z, 0);
	camera_position = glm::vec3(temp_vector.x, temp_vector.y, temp_vector.z);
	set_camera_matrix();
	glLoadMatrixf(glm::value_ptr(camera_matrix));
}

void Camera::change_position(glm::vec3 new_position)
{
	camera_position = new_position;
	set_camera_matrix();
	glLoadMatrixf(glm::value_ptr(camera_matrix));
}

glm::vec3 Camera::get_position()
{
	return glm::vec3(camera_position);
}
