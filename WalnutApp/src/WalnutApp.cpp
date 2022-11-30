#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include <Include/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include "imgui.h"
#include "Walnut/engine/graphics.h"


class Framebuffer
{
public:
	Framebuffer()
	{

	}
	~Framebuffer()
	{

	}
	inline void Construct(unsigned int w, unsigned int h) 
	{
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthID);
		glBindTexture(GL_TEXTURE_2D, m_DepthID);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, w, h);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthID, 0);

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(m_TextureID, buffers);
	}
	inline void Destruct() 
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_TextureID);
		glDeleteTextures(1, &m_DepthID);
	}
	inline void Bind()
	{ 
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		SetScale();
	}
	inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	inline void Invalidate() { }
	inline void Resize() { }
	inline void Rescale(float scale) { if (scale >= 0.0f) m_Scale = scale; }

	inline unsigned int GetRendererID() const { return m_RendererID; }
private:
	inline void SetScale()
	{
		float x = (m_Width / 2.0f) - ((m_Width * m_Scale) / 2.0f);
		float y = (m_Height / 2.0f) - ((m_Height * m_Scale) / 2.0f);
		glViewport(x, y, m_Width * m_Scale, m_Height * m_Scale);
	}
	unsigned int m_RendererID;
	unsigned int m_TextureID;
	unsigned int m_DepthID;
	
	unsigned int m_Width;
	unsigned int m_Height;
	float m_Scale = 1.0f;
};

class SceneView
{
public:
	SceneView() {}

private:
	Framebuffer* m_Framebuffer;
	Graphics* m_Graphics;
};

class SceneLayer : public Walnut::Layer
{
public:
	SceneLayer() : Walnut::Layer()
	{
		ConstructFBO(1920,1080);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind

		m_graphics = new Graphics();
		if (!m_graphics->Initialize(1920,1080))
		{
			printf("The graphics failed to initialize.\n");
			std::exit(1);
		}
	}
	void ConstructFBO(unsigned int w, unsigned int h)
	{
		glGenFramebuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		glCreateTextures(GL_TEXTURE_2D, 1, &mTexId);
		glBindTexture(GL_TEXTURE_2D, mTexId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &mDepthId);
		glBindTexture(GL_TEXTURE_2D, mDepthId);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, w, h);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthId, 0);

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(mTexId, buffers);
	}
	void DestructFBO()
	{
		glDeleteFramebuffers(1, &mFBO);
		glDeleteTextures(1, &mTexId);
		glDeleteTextures(1, &mDepthId);
	}
	void DoSceneEditBar()
	{
		ImGui::PushItemWidth(120);
		int itemz[][2] = { {3840, 2160}, {1920, 1080}, {1768, 992}, {1600, 900}, {1366, 768}, {1280, 720}, {1176, 664}, {1152, 648}, {1024, 576} };
		const char* items[] = { "3840, 2160", "1920, 1080", "1768, 992", "1600, 900", "1366, 768", "1280, 720", "1176, 664", "1152, 648", "1024, 576" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo(" ", combo_preview_value, 0))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
				{
					item_current_idx = n;
					screen = ImVec2(itemz[n][0], itemz[n][1]);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Scale", &slider_f, 0.0f, 5.0f, "%.3f", 0);
	}
	float GetScalingFactor(ImVec2 avail, ImVec2 screen)
	{
		float aspect = screen.x / screen.y;
		float availAspect = avail.x / avail.y;
		return (aspect >= availAspect) ? screen.y / avail.y : screen.x / avail.x;
	}
	virtual void OnUpdate(float ts) override
	{
		m_graphics->Update(ts, glm::vec3(0.0f, 0.0f, 0.0f));
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("Scene");
		{
			DoSceneEditBar();

			DestructFBO(); //can't ensure fbo stays alive until imgui uses it, so destroy it as we start the next draw
			ConstructFBO(screen.x, screen.y);
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
			ImVec2 start((screen.x / 2) - (screen.x * slider_f) / 2, (screen.y / 2) - (screen.y * slider_f) / 2);
			glViewport(start.x, start.y, screen.x * slider_f, screen.y * slider_f);
			m_graphics->Render();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 avail = ImGui::GetContentRegionAvail();

			//fill
			
			ImVec2 scaled;
			scaled.x = pos.x + avail.x;
			scaled.y = pos.y + avail.y;

			//maintain aspect ratio

			ImVec2 aspected = avail;
			float aspect_ratio = screen.x / screen.y;
			aspected.y = aspected.x / aspect_ratio;
			float yOff = (avail.y - aspected.y) / 2;
			if (yOff >= 0.0f)
			{
				pos.y += yOff;
			}
			else
			{
				aspected = avail;
				aspected.x = aspected.y * aspect_ratio;
				float xOff = (avail.x - aspected.x) / 2;
				if (xOff >= 0.0f)
				{
					pos.x += xOff;
				}
			}
			aspected.x += pos.x;
			aspected.y += pos.y;
			auto list = ImGui::GetWindowDrawList();
			if(avail.x >= 0.0f && avail.y >= 0.0f) list->AddImage((void*)mFBO, pos, aspected, ImVec2(0, 1), ImVec2(1, 0));
			//ImGui::Image((void*)mFBO, scaled, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}
private:
	//SceneView* m_SceneView;
	ImVec2 screen = ImVec2(1920, 1080);
	float slider_f = 1.0f;
	unsigned int mFBO = 0;
	unsigned int mTexId = 0;
	unsigned int mDepthId = 0;
	Graphics* m_graphics;
	unsigned int width = 0;
	unsigned int height = 0;
};
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

class DemoLayer : public Walnut::Layer
{
public:
	DemoLayer() : Walnut::Layer()
	{

	}
	virtual void OnUpdate(float ts) override
	{

	}
	virtual void OnUIRender() override
	{
		ImGui::ShowDemoWindow();
	}
};


Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";
	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<SceneLayer>();
	app->PushLayer<DemoLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("About"))
			{
				ImGui::BeginPopup("About");
				ImGui::EndPopup();
			}
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}