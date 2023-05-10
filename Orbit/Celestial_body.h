#pragma once
#define _USE_MATH_DEFINES
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <soil.h>
#include <cmath>
#include <algorithm>
#include <string>

class Celestial_body
{
private:
	glm::vec3* trajectory;
	glm::vec3* vertices;
	glm::vec3 position;
	glm::vec3 base_position;
	glm::mat4 rotation_matrix;
	float trajectory_radius;
	float trajectory_phi;
	float trajectory_psi;
	float speed;
	float body_radius;
	std::string texture_image_path;
	unsigned int texture;

	const int stacks_count = 18;
	const int sectors_count = 36;
	const int M = 100;

	void calculate_trajectory();

	void load_texture(char const* path);

public:
	Celestial_body();
	Celestial_body(float speed, float trajectory_radius, float body_radius);

	~Celestial_body();

	void set_trajectory_phi(float phi);
	void set_trajectory_psi(float psi);
	void set_trajectory_rotation(float phi, float psi);
	void set_trajectory_radius(float r);
	void set_trajectory(float phi, float psi, float r);
	void set_speed(float speed);
	void set_texture(std::string path);

	glm::vec3 get_position();

	void gl_draw_body();
	void gl_draw_trajectory(glm::vec4 color);

	void calculate_vertices();
	void calculate_rotation_matrix();
	void calculate_position(float t);
};

