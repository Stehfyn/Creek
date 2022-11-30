#pragma once
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include "../Graphics/Vertex.h"
#include "Component.h"

namespace Creek
{
	namespace ECS
	{
		class Entity
		{
		public:
			Entity() {}
			void AddComponent(Component* c) { components.emplace(std::make_pair(IDToComponentType.at(c->GetID()), c)); }
			Component* GetComponentOfType(std::type_index ComponentType) { return components.at(ComponentType); }
		private:
			std::unordered_map<std::type_index, Component*> components;
		};
	}
}
