#include "SceneLayer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

SceneLayer1::SceneLayer1() : Walnut::Layer()
{
	//m_SceneView = new SceneView();

	m_FramebufferResolutionComboOptions = { {1920, 1080}, {1768, 992}, {1600, 900}, {1366, 768}, {1280, 720}, {1176, 664}, {1152, 648}, {1024, 576} };
	m_FramebufferResolutionComboSelected = m_FramebufferResolutionComboOptions.at(0);
	m_Graphics = new Graphics();

	FramebufferSpecification fbSpec{ m_FramebufferResolutionComboSelected.x, m_FramebufferResolutionComboSelected.y };
	m_Framebuffer = new Framebuffer(fbSpec);

	m_Framebuffer->Construct();
	m_Graphics->Initialize(m_FramebufferResolutionComboSelected.x, m_FramebufferResolutionComboSelected.y);

	m_MouseWheel = ImGui::GetIO().MouseWheel;
}

void SceneLayer1::DoSceneEditBar()
{
	ImGui::PushItemWidth(120);
	
	//ImVec2 pos = ImGui::GetCursorPos();
	//ImVec2 avail = ImGui::GetWindowSize();
	//ImVec2 button_size = ImGui::CalcItemSize(ImVec2{ 150, 0 }, 0.0f, 0.0f);
	//ImVec2 centre_position_for_button{
	//	// we have two buttons, so twice the size - and we need to account for the spacing in the middle
	//	(avail.x - button_size.x * 2 - ImGui::GetStyle().ItemSpacing.x) / 2,
	//	pos.y
	//};
	//ImGui::SetCursorPos(centre_position_for_button);

	static int item_current_idx = 0;
	std::string combo_preview = ImVec2ToString(m_FramebufferResolutionComboSelected);
	if (ImGui::BeginCombo(" ", combo_preview.c_str(), 0))
	{
		for (int n = 0; n < m_FramebufferResolutionComboOptions.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			std::string option = ImVec2ToString(m_FramebufferResolutionComboOptions.at(n));
			if (ImGui::Selectable(option.c_str(), is_selected))
			{
				item_current_idx = n;
				m_FramebufferResolutionComboSelected = m_FramebufferResolutionComboOptions.at(n);
			}
			if (is_selected) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SliderFloat("Scale", &m_ScaleSlider, 0.0f, 5.0f, "%.3f", 0);

	ImGui::SameLine();
	ImGui::Checkbox("Maintain Aspect Ratio", &m_MaintainAspectRatio);

}

void SceneLayer1::OnUpdate(float ts)
{
	//m_SceneView->OnUpdate(ts);
	float camSpeed = 3.0f * ts;
	float camSensitivity = 45.0f * ts;

	auto* cam = m_Graphics->getCamera();
	auto input = ImGui::GetIO();

	//translation
	if (input.KeysDown[ImGuiKey_W])
		cam->pos += camSpeed * cam->forward;
	if (input.KeysDown[ImGuiKey_S])
		cam->pos -= camSpeed * cam->forward;
	if (input.KeysDown[ImGuiKey_A])
		cam->pos -= glm::normalize(glm::cross(cam->forward, cam->up)) * camSpeed;
	if (input.KeysDown[ImGuiKey_D])
		cam->pos += glm::normalize(glm::cross(cam->forward, cam->up)) * camSpeed;
	
	//rotation
	static bool init = true;
	if (m_IsSceneFocusedAndMouseInScene && input.MouseDown[0])
	{
		float x, y;
		x = input.MousePos.x;
		y = input.MousePos.y;

		if (init)
		{
			m_LastX = x;
			m_LastY = y;
			init = false;
		}
		
		ImVec2 delta(x - m_LastX, m_LastY - y);

		static float yaw = -90.0f;
		static float pitch;

		yaw += delta.x * camSensitivity;
		pitch += delta.y * camSensitivity;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		glm::vec3 lookTo;
		lookTo.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		lookTo.y = sin(glm::radians(pitch));
		lookTo.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cam->forward = glm::normalize(lookTo);

		m_LastX = x;
		m_LastY = y;
	}
	else init = true;

	//zoom
	if (m_MouseWheel);
	{
		cam->fov -= m_MouseWheel * 10;
		if (cam->fov >= 120.0f) cam->fov = 120.0f;
		if (cam->fov <= 10.0f) cam->fov = 10.0f;
	}

	cam->Update(ts);
	m_Graphics->Update(ts, glm::vec3(0.0f,0.0f,0.0f));


}
void SceneLayer1::OnUIRender()
{
	ImGuiWindowClass window_class;
	//window_class.DockingAllowUnclassed = true;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar);
	{

		m_IsSceneFocusedAndMouseInScene = ImGui::IsWindowFocused(); //assume, then check
		
		//io mousehweel updates inside of imgui frame, and our layer update happens before imgui frame so we gotta cap input here
		m_MouseWheel = ImGui::GetIO().MouseWheel;
		ImVec2 mousePos = ImGui::GetIO().MousePos;

		DoSceneEditBar();
		ImVec2 avail = ImGui::GetContentRegionAvail();
		if (avail.x >= 0.0f && avail.y >= 0.0f)
		{
			
			m_Framebuffer->Destruct(); //for right now still storing rendered texture on gpu, so can only invalidate and destroy the fbo after imgui uses it (start of the next frame)
			m_Framebuffer->Construct();
			m_Framebuffer->Bind();

			m_Framebuffer->Resize(m_FramebufferResolutionComboSelected.x, m_FramebufferResolutionComboSelected.y);
			//don't like doing this here but works for now
			m_Framebuffer->Rescale(m_ScaleSlider);

			m_Graphics->Render();
			m_Framebuffer->Unbind();

			std::pair<ImVec2, ImVec2> window_coordinates;
			window_coordinates = (m_MaintainAspectRatio) ? GetScreenMaintainAspectRatio() : GetScreenFill();
			m_IsSceneFocusedAndMouseInScene = ImVec2IsInRect(mousePos, window_coordinates);

			//ImGui::GetWindowDrawList()->AddImage((void*)m_SceneView->GetFramebufferRendererID(), window_coordinates.first, window_coordinates.second, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::GetWindowDrawList()->AddImage((void*)m_Framebuffer->GetRendererID(), window_coordinates.first, window_coordinates.second, ImVec2(0, 1), ImVec2(1, 0));
		}
	}
	ImGui::End();
}
bool SceneLayer1::ImVec2IsInRect(ImVec2& vec, std::pair<ImVec2, ImVec2>& rect)
{
	if (vec.x >= rect.first.x && vec.x <= rect.second.x)
		if (vec.y >= rect.first.y && vec.y <= rect.second.y)
			return true;
	return false;
}
std::string SceneLayer1::ImVec2ToString(ImVec2& vec)
{
	std::stringstream conversion;
	conversion << vec.x << ", " << vec.y;
	return conversion.str();
}
std::pair<ImVec2, ImVec2> SceneLayer1::GetScreenFill()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 avail = ImGui::GetContentRegionAvail();

	ImVec2 filled;
	filled.x = pos.x + avail.x;
	filled.y = pos.y + avail.y;

	return { pos, filled };
}
std::pair<ImVec2, ImVec2> SceneLayer1::GetScreenMaintainAspectRatio()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 avail = ImGui::GetContentRegionAvail();

	ImVec2 aspected = avail;
	float aspect_ratio = m_FramebufferResolutionComboSelected.x / m_FramebufferResolutionComboSelected.y; //not asking actual framebuffer with a validated state, deffo refactor this guy
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

	return { pos, aspected };
}