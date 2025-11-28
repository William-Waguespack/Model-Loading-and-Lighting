#include "Game.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include <fstream>
#include <string>
#include "Cubemap.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"

Game::Game() : m_GameWindow(1366, 768), deltaTime(0.f)
{
	m_GameWindow.initialize();
}

void Game::Run()
{
	NewShader ModelShaderTest;
	ModelShaderTest.CompileShaders("BasicShading.glsl");

	NewShader sky;
	sky.CompileShaders("Skybox.glsl");
	std::shared_ptr<Cubemap> Skybox = std::make_shared<Cubemap>("Textures/right.jpg", "Textures/left.jpg", "Textures/top.jpg", "Textures/bottom.jpg", "Textures/front.jpg", "Textures/back.jpg");

	Model BackFaceBox("Models/BackFaceCube.fbx", Skybox);
	Model backpack("Models/sponza/sponza.obj");
	Model other("Models/backpack/backpack.obj");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	// Enable face culling
	glEnable(GL_CULL_FACE);
	// Specify which face to cull
	glCullFace(GL_BACK); // Can be GL_FRONT or GL_BACK or GL_FRONT_AND_BACK
	// Define the winding order of front faces
	glFrontFace(GL_CCW); // Default is GL_CCW (counter-clockwise). Use GL_CW if needed.



	glm::mat4 projection(1.0);
	projection = glm::perspective(glm::radians(90.0f), m_GameWindow.getBufferWidth() / m_GameWindow.getBufferHeight(), 0.1f, 500.f);
	Camera camera(projection);

	double lastTime = 0;
	glm::mat4 model(1.0f);




	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(m_GameWindow.GetmainWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	float lightDir[3] = { 0.0f, -1.0f, 0.0f };
	float lightAmbient[3] = { 0.5f, 0.5f, 0.5f };
	float lightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
	float lightSpecular[3] = { 0.8f, 0.8f, 0.8f };
	float speed = 5;
	float fov = 90.f;

	glfwSwapInterval(0);
	while (!m_GameWindow.GetShouldClose())
	{
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();
		camera.SetMovementSpeed(speed);
		camera.keyControl(m_GameWindow.GetKeys(), deltaTime);
		camera.mouseControl(m_GameWindow.getXChange(), m_GameWindow.getYChange(), deltaTime);

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ModelShaderTest.UseShader();
		ModelShaderTest.setUniformMatrix4fv("projection", camera.GetProjection());
		ModelShaderTest.setUniformMatrix4fv("view", camera.calculateViewMatrix());

		glm::mat4 othermodel(1.0f);
		ModelShaderTest.setUniformMatrix4fv("model", othermodel);
		glm::vec3 cameraPos = camera.getCameraPosition();


		ModelShaderTest.setUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
		ModelShaderTest.setUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
		ModelShaderTest.setUniform3f("lightDir", lightDir[0], lightDir[1], lightDir[2]);
		ModelShaderTest.setUniform3f("lightAmbient", lightAmbient[0], lightAmbient[1], lightAmbient[2]);
		ModelShaderTest.setUniform3f("lightDiffuse", lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
		ModelShaderTest.setUniform3f("lightSpecular", lightSpecular[0], lightSpecular[1], lightSpecular[2]);

		backpack.Draw(ModelShaderTest);
		other.Draw(ModelShaderTest);

		for (unsigned int i = 0; i < 10; i++)
		{
			othermodel = glm::translate(othermodel, glm::vec3(0, 10, 0));
			ModelShaderTest.setUniformMatrix4fv("model", othermodel);
			other.Draw(ModelShaderTest);
		}




		glDepthFunc(GL_LEQUAL);
		sky.UseShader();
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(camera.calculateViewMatrix()));

		sky.setUniformMatrix4fv("proj", projection);
		sky.setUniformMatrix4fv("vieww", view);
		BackFaceBox.Draw(sky);
		glDepthFunc(GL_LESS);

		glUseProgram(0);



		ImGui::Begin("Info");
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::SliderFloat("Camera Speed", &speed, 0.05f, 20.f);
			ImGui::SliderFloat("Camera FOV", &fov, 1.0f, 180.f);
		}

		if (ImGui::CollapsingHeader("Lighting"))
		{
			ImGui::SliderFloat3("Light Direction", lightDir, -180, 180);
			ImGui::ColorPicker3("Ambient Light", lightAmbient);
			ImGui::ColorPicker3("Diffuse Light", lightDiffuse);
			ImGui::ColorPicker3("Specular Light", lightSpecular);

		}

		ImGui::CollapsingHeader("Performance");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);


		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		m_GameWindow.SwapBuffers();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
