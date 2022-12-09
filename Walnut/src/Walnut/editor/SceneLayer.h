#pragma once
#include "Walnut/Layer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include "imgui.h"
#include "imgui_internal.h"

#include "Walnut/Graphics/Framebuffer.h"
#include "Walnut/engine/graphics.h"

#include <string>
#include <sstream>
#include <utility>

class SceneLayer1 : public Walnut::Layer
{
public:
	SceneLayer1();
	virtual void OnUpdate(float ts);
	virtual void OnUIRender() override;

private:
	void DoSceneEditBar();
	bool ImVec2IsInRect(ImVec2& vec, std::pair<ImVec2, ImVec2>& rect);
	std::string ImVec2ToString(ImVec2& vec);
	std::pair<ImVec2, ImVec2> GetScreenFill();
	std::pair<ImVec2, ImVec2> GetScreenMaintainAspectRatio();
private:
	
	Graphics* m_Graphics;
	Framebuffer* m_Framebuffer;

	float m_ScaleSlider = 1.0f;
	bool m_MaintainAspectRatio = true;
	ImVec2 m_FramebufferResolutionComboSelected;
	std::vector<ImVec2> m_FramebufferResolutionComboOptions;

	float m_LastX;
	float m_LastY;
	int m_MouseWheel;
	bool m_IsSceneFocusedAndMouseInScene = false;
};