#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "Walnut/Layer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
class DemoSceneLayer : public Walnut::Layer
{
public:
	DemoSceneLayer() : Walnut::Layer()
	{
		GLuint Program;
		GLuint vao[1];

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vShaderSrc = "#version 460 \n void main() { gl_Position = vec4(1.0,0.0,0.0,1.0);} "; // 10/10 gl_position -> gl_Position
		const char* fShaderSrc = "#version 460 \n out vec4 color void main() { color = vec4(1.0,0.0,0.0,0.5);} ";

		glShaderSource(vShader, 1, &vShaderSrc, NULL);
		glShaderSource(fShader, 1, &fShaderSrc, NULL);

		glCompileShader(vShader);
		glCompileShader(fShader);

		Program = glCreateProgram();
		glAttachShader(Program, vShader);
		glAttachShader(Program, fShader);

		glLinkProgram(Program);

		glGenVertexArrays(1, vao);
		glBindVertexArray(vao[0]);
		glUseProgram(Program);
	}

	virtual void OnUpdate(float ts) override
	{

	}
	virtual void OnUIRender() override
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, 800, 600);
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ImGui::Begin("DemoScene");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 avail = ImGui::GetContentRegionAvail();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddImage((void*)mFBO,
			pos,
			ImVec2(pos.x + avail.x, pos.y + avail.y),
			ImVec2(0, 1),
			ImVec2(1, 0));
		ImGui::End();
	}
private:
	unsigned int mFBO = 0;
};