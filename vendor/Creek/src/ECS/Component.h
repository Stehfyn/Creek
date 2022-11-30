#pragma once
#include "Walnut/crkpch.h"
#include "Walnut/Graphics/Vertex.h"
#include "Walnut/Graphics/Index.h"
namespace Creek
{
	namespace ECS
	{
		class Component
		{
		public:
			virtual void process() = 0;
			std::string GetID() const { return id; }
		protected:
			std::string id;
		};

		class MeshComponent : public Component
		{
			using Vertex = Creek::Graphics::Vertex;
			using Index = Creek::Graphics::Index;
		public:
			MeshComponent() { id = "MeshComponent"; }
			void AddVertices(std::vector<Vertex> v) { vertices = v; }
			virtual void process() override { }
		private:
			std::vector<Vertex> vertices;
			std::vector<Index> indices;
		};

		class TextureComponent : public Component
		{
		public:
			TextureComponent() { id = "TextureComponent"; }
			virtual void process() override {}
		private:
			unsigned int textureID;
		};

		class PhysicsComponent : public Component
		{

		};

		const std::unordered_map<std::string, std::type_index> IDToComponentType
		{
			{"MeshComponent",    typeid(MeshComponent)},
			{"TextureComponent", typeid(TextureComponent)},
			{"PhysicsComponent", typeid(PhysicsComponent)}
		};

		const std::unordered_map<std::type_index, std::string> ComponentTypeToID
		{
			{ typeid(MeshComponent), "MeshComponent"},
			{ typeid(TextureComponent), "TextureComponent"},
			{ typeid(PhysicsComponent), "PhysicsComponent"}
		};
	}
}
