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
	bool HandleMouseClick(Engine::MouseButtonPressedEvent& e);
	bool HandleMouseMove(Engine::MouseMovedEvent& e);
	void SetTriangleFourPointColor(std::vector<Engine::Ref<Engine::Triangle>>& triangles);
	void DepthFirstColor(Engine::Ref<Engine::Triangle> triangle, std::unordered_map<Engine::Ref<Engine::Triangle>, int>& colors, std::unordered_set<Engine::Ref<Engine::Triangle>>& visited);
	void CalculateCentroid(const Engine::Ref<Engine::Triangle>& triangle, Engine::Vertex& center);
	void AssignDistanceBasedColors(const std::vector<Engine::Ref<Engine::Triangle>>& triangles);
	static bool CompareEdges(const Engine::Ref<Engine::Edge>& a, const Engine::Ref<Engine::Edge>& b);
private:
	Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> vertices = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();
	bool triangles_drawn = false;
	Engine::ShaderLibrary shaders{};
	std::unordered_map<std::string, Engine::Ref<Engine::Object>> objects{};
	std::vector<Engine::Ref<Engine::Object>> triangle_objects{};
	float MouseX, MouseY;
};
