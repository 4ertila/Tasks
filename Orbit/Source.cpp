#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "Celestial_body.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <soil.h>
#include <iostream>
#include <fstream>

int main()
{
    glewInit();
    const float rotate_angle = 5.0f;
    int moon_trajectory_psi = 0;
    int moon_trajectory_phi = 0;
    float moon_trajectory_radius = 3;
    float moon_speed = 3;
    GLFWwindow* window;
    glm::vec3 camera_position = glm::vec3(5.0f, 5.0f, 5.0f);
    glm::vec3 camera_target = glm::vec3(0, 0, 0);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1366, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1.0f, 100.0f)));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-10, -10, -10, 10, -4, 4);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Celestial_body earth = Celestial_body();
    earth.set_texture("textures\\earth_texture.bmp");
    Celestial_body moon = Celestial_body(moon_speed, 3, 0.5f);
    moon.set_texture("textures\\moon_texture.bmp");
    Camera camera = Camera(camera_position, camera_target, glm::vec3(0.0f, 0.0f, 1.0f));;

    float dt = 0.01f;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::SliderInt("phi", &moon_trajectory_phi, 0, 360);
        ImGui::SliderInt("psi", &moon_trajectory_psi, 0, 360);
        ImGui::SliderFloat("moon trajectory radius", &moon_trajectory_radius, 1, 10);
        ImGui::SliderFloat("moon speed", &moon_speed, 0, 10);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            glm::vec3 current_camera_position = camera.get_position();
            glm::vec3 move_direction = -0.3f * current_camera_position / (float)current_camera_position.length();
            camera.change_position(camera.get_position() + move_direction);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            glm::vec3 current_camera_position = camera.get_position();
            glm::vec3 move_direction = 0.3f * current_camera_position / (float)current_camera_position.length();
            camera.change_position(camera.get_position() + move_direction);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            glm::vec3 current_camera_position = camera.get_position();
            camera.rotation(rotate_angle, current_camera_position.y, -current_camera_position.x, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            glm::vec3 current_camera_position = camera.get_position();
            camera.rotation(-rotate_angle, current_camera_position.y, -current_camera_position.x, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.rotation(rotate_angle, 0, 0, 1);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.rotation(-rotate_angle, 0, 0, 1);
        }

        earth.gl_draw_body();
        moon.set_speed(moon_speed);
        moon.set_trajectory(moon_trajectory_phi, moon_trajectory_psi, moon_trajectory_radius);
        moon.gl_draw_body();
        moon.gl_draw_trajectory(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        moon.calculate_position(dt);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}