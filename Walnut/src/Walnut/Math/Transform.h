#pragma once
#include "Walnut/crkpch.h"
namespace Creek
{
	namespace Math
	{
		struct Transform
		{
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::mat4 model;
		};
	}
}

