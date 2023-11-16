#include <imgui/imgui.h>

#include "GlobalState.h"
#include "Lab2Layer.h"


void Lab2Layer::OnUpdate(float delta_time)
{
	Engine::Renderer::Get()->SetClearColor(GlobalState::Instance().BG_COLOR);

	if(GlobalState::Instance().shouldDrawTriangles)
	{

		for (size_t i = 0; i < triangle_objects.size(); i++) {
			triangle_objects[i]->AddGLDrawType(GL_LINE_LOOP);
			Engine::Renderer::Get()->Draw(triangle_objects[i]);
		}
	}

	objects["point_cloud"]->AddGLDrawType(GL_POINTS);

	
	Engine::Renderer::Get()->Draw(objects["point_cloud"]);

	Engine::Renderer::Get()->EndFrame();
}

void Lab2Layer::OnAttach()
{
	std::string shader_file = std::string(ROOT_PATH) + "/shaders/simple_shader";
	shaders.Add("simple_shader", shader_file);

	Engine::Ref<Engine::Shader> shader = shaders.Get("simple_shader");

	objects["point_cloud"] = Engine::CreateRef<Engine::Object>(Engine::Object(shader));

	InitData();
}


void Lab2Layer::InitData()
{
	vertices->clear();
	int count = GlobalState::Instance().POINT_COUNT >= 3 ? GlobalState::Instance().POINT_COUNT : 3;
	for (int i = 0; i < count; i++) {
		float x = Engine::Random::RandomFloat(-1.f, 1.f);
		float y = Engine::Random::RandomFloat(-1.f, 1.f);
		Engine::Ref<Engine::Vertex> vertex = Engine::CreateRef<Engine::Vertex>(x, y, 0.0f);

		vertices->push_back(vertex);
	}

	objects["point_cloud"]->AddVertices(vertices);
}

void Lab2Layer::CreateTriangles()
{
	triangle_objects.clear();

	Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> triangle_vertices = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();
	Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> hull = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();
	Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> hull_inner_verts = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();

	// Start by sorting the vertices by x (and then y if x's are equal)
	Engine::Sort::MergeSortArray(vertices->begin(), vertices->end(), [](Engine::Ref<Engine::Vertex> a, Engine::Ref<Engine::Vertex> b) {
		return a->x == b->x ? a->y < b->y : a->x < b->x;
		});

	Engine::Algorithms::CreateConvexHull(hull, hull_inner_verts, vertices, false);

	std::vector<Engine::Ref<Engine::Triangle>> triangles = {};

	for (size_t i = 0; i < hull->size(); i++)
	{
		if (i + 1 < hull->size()) {
			Engine::Ref<Engine::Triangle> newTriangle = Engine::CreateRef<Engine::Triangle>(
				hull->at(0), hull->at(i), hull->at(i + 1)
			);

			triangles.push_back(newTriangle);
		}
	}

	for (size_t i = 0; i < hull_inner_verts->size(); i++)
	{
		for (size_t j = 0; j < triangles.size(); j++)
		{
			if (Engine::Triangle::IsPointInTriangle(hull_inner_verts->at(i), triangles.at(j)))
			{
				auto currentTriangle = triangles.at(j);

				// Create three new triangles using the point and each edge of the current triangle
				Engine::Ref<Engine::Triangle> newTriangle1 = Engine::CreateRef<Engine::Triangle>(currentTriangle->vertices->at(0), currentTriangle->vertices->at(1), hull_inner_verts->at(i));
				Engine::Ref<Engine::Triangle> newTriangle2 = Engine::CreateRef<Engine::Triangle>(currentTriangle->vertices->at(1), currentTriangle->vertices->at(2), hull_inner_verts->at(i));
				Engine::Ref<Engine::Triangle> newTriangle3 = Engine::CreateRef<Engine::Triangle>(currentTriangle->vertices->at(2), currentTriangle->vertices->at(0), hull_inner_verts->at(i));

				triangles[j] = newTriangle1;
				triangles.insert(triangles.begin() + j + 1, newTriangle2);
				triangles.insert(triangles.begin() + j + 2, newTriangle3);

				break;
			}
		}
	}



	Engine::Ref<Engine::Shader> shader = shaders.Get("simple_shader");

	for (size_t i = 0; i < triangles.size(); i++) {
		triangle_objects.push_back(Engine::CreateRef<Engine::Object>(Engine::Object(shader)));

		Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> triangle = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();
		triangle->push_back(triangles[i]->vertices->at(0));
		triangle->push_back(triangles[i]->vertices->at(1));
		triangle->push_back(triangles[i]->vertices->at(2));
		triangle_objects[i]->AddVertices(triangle);
	}
}

void Lab2Layer::OnDetach()
{

}

void Lab2Layer::OnEvent(Engine::Event& event)
{

}

void Lab2Layer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	const char* items[] = { "Small set", "Medium set", "Large set" };
	if (ImGui::Combo("Point cloud set", &GlobalState::Instance().choice, items, IM_ARRAYSIZE(items)))
	{
		GlobalState::Instance().POINT_COUNT = GlobalState::Instance().point_sets.at(GlobalState::Instance().choice);
		GlobalState::Instance().shouldDrawTriangles = false;
		triangles_drawn = false;
		InitData();
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	if (ImGui::InputInt("Point Size", &GlobalState::Instance().POINT_COUNT))
	{
		GlobalState::Instance().shouldDrawTriangles = false;
		triangles_drawn = false;
		for (size_t i = 0; i < GlobalState::Instance().point_sets.size(); i++)
		{
			if (GlobalState::Instance().point_sets[i] == GlobalState::Instance().POINT_COUNT)
			{
				GlobalState::Instance().choice = i;
				break;
			}
		}
		InitData();
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Checkbox("Draw triangles", &GlobalState::Instance().shouldDrawTriangles);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if(ImGui::Button("Generate new point"))
	{
		Engine::Ref<Engine::Vertex> vertex = Engine::CreateRef<Engine::Vertex>(Engine::Random::RandomFloat(-1.f, 1.f), Engine::Random::RandomFloat(-1.f, 1.f), 0.f);
		vertices->push_back(vertex);
		if (GlobalState::Instance().shouldDrawTriangles)
		{
			CreateTriangles();
			triangles_drawn = true;
		}
	}

	if (GlobalState::Instance().shouldDrawTriangles && !triangles_drawn)
	{
		CreateTriangles();
		triangles_drawn = true;
	}

	ImGui::End();
}
