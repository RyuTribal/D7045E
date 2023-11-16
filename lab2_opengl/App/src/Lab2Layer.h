#pragma once
#include <Engine.h>

class Lab2Layer : public Engine::Layer
{
public:
	Lab2Layer() : Layer("Lab2") {}
	~Lab2Layer() = default;
	void OnUpdate(float delta_time) override;
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Engine::Event& event) override;
	void OnImGuiRender() override;

private:
	void InitData();
	void CreateTriangles();
private:
	Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> vertices = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();
	bool triangles_drawn = false;
	Engine::ShaderLibrary shaders{};
	std::unordered_map<std::string, Engine::Ref<Engine::Object>> objects{};
	std::vector<Engine::Ref<Engine::Object>> triangle_objects{};
};
