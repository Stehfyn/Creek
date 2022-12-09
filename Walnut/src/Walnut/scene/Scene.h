#pragma once
#include <string>
#include <vector>
#include "Walnut/ecs/Entity.h"
#include "Walnut/Graphics/Shader.h"

class Scene
{
public:
	Scene();
	void OnUpdate(float ts);

	void Render();
	void AddEntity();

private:
	std::vector<Creek::ECS::Entity*> objs;
	Creek::ECS::Camera m_Camera;
	
	//make this per object, related: batch rendering
	Creek::Graphics::Shader m_Shader;

};

