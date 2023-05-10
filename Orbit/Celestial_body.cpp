#include "Celestial_body.h"

void Celestial_body::load_texture(char const* path)
{
	texture = SOIL_load_OGL_texture(
		path,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
	);
}

Celestial_body::Celestial_body()
{
	trajectory = new glm::vec3 [M + 1];
	vertices = new glm::vec3[stacks_count * sectors_count * 4];
	for (int i = 0; i < M + 1; i++)
	{
		trajectory[i] = glm::vec3();
	}
	position = glm::vec3(0, 0, 0);
	trajectory_radius = 0;
	trajectory_phi = 0;
	trajectory_psi = 0;
	speed = 0;
	texture_image_path = "undefined";
	texture = 0;
	body_radius = 1;

	calculate_vertices();
	calculate_rotation_matrix();
	calculate_trajectory();
}
Celestial_body::Celestial_body(float speed, float trajectory_radius, float body_radius)
{
	trajectory = new glm::vec3 [M + 1];
	vertices = new glm::vec3[stacks_count * sectors_count * 4];
	for (int i = 0; i < M + 1; i++)
	{
		trajectory[i] = glm::vec3();
	}
	trajectory_phi = 0;
	trajectory_psi = 0;
	position = glm::vec3(trajectory_radius, 0, 0);
	base_position = position;
	texture_image_path = "undefined";
	texture = 0;
	this->body_radius = body_radius;
	this->trajectory_radius = trajectory_radius;
	this->speed = speed;

	calculate_vertices();
	calculate_rotation_matrix();
	calculate_trajectory();
}

Celestial_body::~Celestial_body()
{
	delete trajectory;
	delete vertices;
}

void Celestial_body::calculate_rotation_matrix()
{
	glm::mat4 meridian_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(trajectory_psi), glm::vec3(0, 0, 1));
	glm::mat4 equator_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(trajectory_phi), glm::vec3(0, 1, 0));
	rotation_matrix =  meridian_rotation_matrix * equator_rotation_matrix;
}
void Celestial_body::calculate_vertices()
{
	float r = body_radius;
	for (int i = 0, k = 0; i < sectors_count; i++)
	{
		for (int j = 0; j < stacks_count; j++, k += 4)
		{
			vertices[k] = glm::vec3(r * cos((j) * 2 * M_PI / stacks_count) * sin((i + 1) * M_PI / sectors_count),
				r * sin((j) * 2 * M_PI / stacks_count) * sin((i + 1) * M_PI / sectors_count),
				r * cos((i + 1) * M_PI / sectors_count));
			vertices[k + 1] = glm::vec3(r * cos(j * 2 * M_PI / stacks_count) * sin((i) * M_PI / sectors_count),
				r * sin(j * 2 * M_PI / stacks_count) * sin((i) * M_PI / sectors_count),
				r * cos((i) * M_PI / sectors_count));
			vertices[k + 2] = glm::vec3(r * cos((j+1) * 2 * M_PI / stacks_count) * sin(i * M_PI / sectors_count),
				r * sin((j+1) * 2 * M_PI / stacks_count) * sin(i * M_PI / sectors_count),
				r * cos(i * M_PI / sectors_count));
			vertices[k + 3] = glm::vec3(r * cos((j + 1) * 2 * M_PI / stacks_count) * sin((i+1) * M_PI / sectors_count),
				r * sin((j + 1) * 2 * M_PI / stacks_count) * sin((i + 1) * M_PI / sectors_count),
				r * cos((i + 1) * M_PI / sectors_count));
		}
	}
}

void Celestial_body::set_trajectory_phi(float phi)
{
	trajectory_phi = phi;

	calculate_rotation_matrix();
	calculate_trajectory();
}
void Celestial_body::set_trajectory_psi(float psi)
{
	trajectory_psi = psi;

	calculate_rotation_matrix();
	calculate_trajectory();
}
void Celestial_body::set_trajectory_rotation(float phi, float psi)
{
	trajectory_psi = psi;
	trajectory_phi = phi;

	calculate_rotation_matrix();
	calculate_trajectory();
}
void Celestial_body::set_trajectory_radius(float r)
{
	trajectory_radius = r;
	if (glm::length(position) == 0)
	{
		position.x + 0.01;
	}
	position = position * (1.0f / glm::length(position)) * trajectory_radius;

	calculate_trajectory();
}
void Celestial_body::set_trajectory(float phi, float psi, float r)
{
	trajectory_radius = r;
	position = position * (1.0f / glm::length(position)) * trajectory_radius;

	trajectory_psi = psi;
	trajectory_phi = phi;

	calculate_rotation_matrix();
	calculate_trajectory();
}
void Celestial_body::set_speed(float speed)
{
	this->speed = speed;
}
void Celestial_body::set_texture(std::string path)
{
	load_texture(path.c_str());
}

glm::vec3 Celestial_body::get_position()
{
	return position;
}

void Celestial_body::gl_draw_body()
{
	if (texture != 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture);

		glBegin(GL_QUADS);
		for (int i = sectors_count - 1, k = 0; i >= 0; i--)
		{
			for (int j = 0; j < stacks_count; j++, k += 4)
			{
				glTexCoord2f(j * 1.0f / stacks_count, i * 1.0f / sectors_count);
				glVertex3fv(glm::value_ptr(vertices[k] + position));

				glTexCoord2f(j * 1.0f / stacks_count, (i + 1) * 1.0f / sectors_count);
				glVertex3fv(glm::value_ptr(vertices[k + 1] + position));

				glTexCoord2f((j + 1) * 1.0f / stacks_count, (i + 1) * 1.0f / sectors_count);
				glVertex3fv(glm::value_ptr(vertices[k + 2] + position));

				glTexCoord2f((j + 1) * 1.0f / stacks_count, i * 1.0f / sectors_count);
				glVertex3fv(glm::value_ptr(vertices[k + 3] + position));
			}
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		float r = body_radius;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < stacks_count * sectors_count * 4; i++)
		{
			glVertex3fv(glm::value_ptr(vertices[i] + position));
		}
		glEnd();
	}
}
void Celestial_body::gl_draw_trajectory(glm::vec4 color)
{
	glBegin(GL_LINE_STRIP);
	glColor3fv(glm::value_ptr(color));
	for (int i = 0; i < M + 1; i++)
	{
		glVertex3fv(glm::value_ptr(trajectory[i]));
	}
	glEnd();
}

void Celestial_body::calculate_position(float delta_time)
{
	float r = trajectory_radius;
	base_position += glm::vec3(-base_position.y, base_position.x, 0) * (1.0f / r) * delta_time * speed;
	position = rotation_matrix * glm::vec4(base_position, 0);
}

void Celestial_body::calculate_trajectory()
{
	float r = trajectory_radius;
	for (int i = 0; i < M + 1; i++)
	{
		trajectory[i] = rotation_matrix * glm::vec4(r * cos(i * 2 * M_PI / M), r * sin(i * 2 * M_PI / M), 0, 0);
	}
}