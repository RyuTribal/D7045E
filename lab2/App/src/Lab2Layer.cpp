#include <imgui/imgui.h>

#include "GlobalState.h"
#include "Lab2Layer.h"


void Lab2Layer::OnUpdate(float delta_time)
{
	Engine::Renderer::Get()->SetClearColor(GlobalState::Instance().BG_COLOR);

	if (GlobalState::Instance().shouldDrawTriangles) {
		for (size_t i = 0; i < triangle_objects.size(); i++) {
			Engine::Renderer::Get()->Draw(triangle_objects[i]);
		}
	}
	objects["point_cloud"]->AddVertices(vertices);
	Engine::Renderer::Get()->Draw(objects["point_cloud"]);

	Engine::Renderer::Get()->EndFrame();
}

void Lab2Layer::OnAttach()
{
	std::string shader_file = std::string(ROOT_PATH) + "/shaders/simple_shader";
	shaders.Add("simple_shader", shader_file);

	Engine::Ref<Engine::Shader> shader = shaders.Get("simple_shader");

	objects["point_cloud"] = Engine::CreateRef<Engine::Object>(shader);
	objects["point_cloud"]->SetShowVertices(true);
	objects["point_cloud"]->SetShowFaces(false);

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

void Lab2Layer::DepthFirstColor(Engine::Ref<Engine::Triangle> triangle, std::unordered_map<Engine::Ref<Engine::Triangle>, int>& colors, std::unordered_set<Engine::Ref<Engine::Triangle>>& visited) {
	if (!triangle || visited.find(triangle) != visited.end()) {
		return;
	}

	visited.insert(triangle);


	for (auto& neighbor : *triangle->neighbors) {
		DepthFirstColor(neighbor, colors, visited);
	}

	bool available2[4] = {true, true, true, true};
	for (const auto& neighbor : *triangle->neighbors) {
		if (neighbor && colors.find(neighbor) != colors.end()) {
			available2[colors[neighbor]] = false;
		}
	}

	for (int i = 0; i < 4; ++i) {
		if (available2[i]) {
			colors[triangle] = i;
			break;
		}
	}
}

void Lab2Layer::SetTriangleFourPointColor(std::vector<Engine::Ref<Engine::Triangle>>& triangles)
{
	std::unordered_map<Engine::Ref<Engine::Triangle>, int> colors;

	std::unordered_set<Engine::Ref<Engine::Triangle>> visited;
	DepthFirstColor(triangles[0], colors, visited);

	for (auto& triangle : triangles) {
		switch (colors[triangle]) {
		case 0:
			triangle->SetColor(255, 0, 0);
			break;
		case 1:
			triangle->SetColor(0, 0, 255);
			break;
		case 2:
			triangle->SetColor(0, 255, 0);
			break;
		case 3:
			triangle->SetColor(255, 255, 0);
			break;
		}
	}
}

bool Lab2Layer::CompareEdges(const Engine::Ref<Engine::Edge>& a, const Engine::Ref<Engine::Edge>& b) {
	float xminA = std::min(a->v1->x, a->v2->x);
	float xminB = std::min(b->v1->x, b->v2->x);
	if (xminA != xminB) return xminA < xminB;

	float xmaxA = std::max(a->v1->x, a->v2->x);
	float xmaxB = std::max(b->v1->x, b->v2->x);
	if (xmaxA != xmaxB) return xmaxA < xmaxB;

	float yminA = std::min(a->v1->y, a->v2->y);
	float yminB = std::min(b->v1->y, b->v2->y);
	if (yminA != yminB) return yminA < yminB;

	float ymaxA = std::max(a->v1->y, a->v2->y);
	float ymaxB = std::max(b->v1->y, b->v2->y);
	return ymaxA < ymaxB;
}

void Lab2Layer::CalculateCentroid(const Engine::Ref<Engine::Triangle>& triangle, Engine::Vertex& center) {
	float x = (triangle->vertices->at(0)->x + triangle->vertices->at(1)->x + triangle->vertices->at(2)->x) / 3;
	float y = (triangle->vertices->at(0)->y + triangle->vertices->at(1)->y + triangle->vertices->at(2)->y) / 3;
	center = Engine::Vertex(x, y, 0.0f);
}

void Lab2Layer::AssignDistanceBasedColors(const std::vector<Engine::Ref<Engine::Triangle>>& triangles) {
	Engine::Vertex p{};
	float maxDistance = Engine::Algorithms::CalculateMaxDistance(*vertices.get(), p);

	for (const auto& triangle : triangles) {
		Engine::Vertex centroid;
		CalculateCentroid(triangle, centroid);

		// Calculate the distance from p to the centroid
		float distance = Engine::Vertex::DistanceBetweenTwoVerts(p, centroid);

		// Normalize the distance and convert it to a grayscale value
		float intensity = 1.0f - (distance / maxDistance);
		intensity = std::max(0.0f, std::min(1.0f, intensity)); // Clamp between 0 and 1

		// Assign the color to the triangle
		triangle->SetColor(intensity, intensity, intensity);
	}
}

void Lab2Layer::CreateTriangles()
{
	triangle_objects.clear();

	// N Log N
	Engine::Sort::MergeSortArray(vertices->begin(), vertices->end(), [](Engine::Ref<Engine::Vertex> a, Engine::Ref<Engine::Vertex> b) {
		return a->x == b->x ? a->y < b->y : a->x < b->x;
	});

	std::vector<Engine::Ref<Engine::Triangle>> triangles = {};
	std::vector<Engine::Ref<Engine::Edge>> triangle_edges = {};

	// N for pre sorted points
	Engine::Algorithms::TriangulateConvexMesh(vertices, triangles, &triangle_edges, false);

	std::sort(triangle_edges.begin(), triangle_edges.end(), CompareEdges);

	//Set neighbors
	for (size_t i = 0; i < triangle_edges.size() - 1; ++i) {
		if (*triangle_edges[i] == *triangle_edges[i + 1]) {
			// Find the edge index in each triangle and set them as neighbors
			int edgeIndex1 = Engine::Edge::FindEdgeIndex(triangle_edges[i], triangle_edges[i]->origin);
			int edgeIndex2 = Engine::Edge::FindEdgeIndex(triangle_edges[i + 1], triangle_edges[i + 1]->origin);

			triangle_edges[i]->origin->SetNeighbor(edgeIndex1, triangle_edges[i + 1]->origin);
			triangle_edges[i + 1]->origin->SetNeighbor(edgeIndex2, triangle_edges[i]->origin);
		}
	}

	Engine::Ref<Engine::Shader> shader = shaders.Get("simple_shader");

	if (GlobalState::Instance().color_type_choice == 1) {
		AssignDistanceBasedColors(triangles);
	}
	else if (GlobalState::Instance().color_type_choice == 2) {
		SetTriangleFourPointColor(triangles);
	}

	if (triangles.size() == 3) {
		for (auto triangle : triangles) {
			CORE_WARN(triangle->NeighborLength());
			CORE_WARN("Triangle color r: {0}, g: {1}, b: {2}", std::to_string(triangle->vertices->at(0)->r), std::to_string(triangle->vertices->at(0)->g), std::to_string(triangle->vertices->at(0)->b));
		}
	}

	for (size_t i = 0; i < triangles.size(); i++) {
		triangle_objects.push_back(Engine::CreateRef<Engine::Object>(Engine::Object(shader)));

		if (GlobalState::Instance().color_type_choice == 0) {
			triangles[i]->SetColor(GlobalState::Instance().uniform_color[0], GlobalState::Instance().uniform_color[1], GlobalState::Instance().uniform_color[2]);
		}

		Engine::Ref<std::vector<Engine::Ref<Engine::Vertex>>> triangle = Engine::CreateRef<std::vector<Engine::Ref<Engine::Vertex>>>();
		triangle->push_back(triangles[i]->vertices->at(0));
		triangle->push_back(triangles[i]->vertices->at(1));
		triangle->push_back(triangles[i]->vertices->at(2));
		triangle_objects[i]->AddVertices(triangle);
		triangle_objects[i]->SetShowFaces(true);
		triangle_objects[i]->SetShowLines(true);
		triangle_objects[i]->SetShowVertices(false);
	}
}

bool Lab2Layer::HandleMouseClick(Engine::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == MOUSE_BUTTON_LEFT) {
		Engine::Application& app = Engine::Application::Get();
		float normalized_x = (MouseX / app.GetWindow().GetWidth()) * 2.0f - 1.0f;
		float normalized_y = 1.0f - (MouseY / app.GetWindow().GetHeight()) * 2.0f;
		
		Engine::Ref<Engine::Vertex> vertex = Engine::CreateRef<Engine::Vertex>(normalized_x, normalized_y, 0.f);
		vertices->push_back(vertex);
		GlobalState::Instance().POINT_COUNT++;
		if (GlobalState::Instance().shouldDrawTriangles)
		{
			CreateTriangles();
			triangles_drawn = true;
		}

		return true;
	}
	return false;
}

bool Lab2Layer::HandleMouseMove(Engine::MouseMovedEvent& e)
{
	MouseX = e.GetX();
	MouseY = e.GetY();
	return true;
}

void Lab2Layer::OnDetach()
{

}

void Lab2Layer::OnEvent(Engine::Event& event)
{
	Engine::EventDispatcher e(event);
	e.Dispatch<Engine::MouseButtonPressedEvent>(BIND_EVENT_FN(Lab2Layer::HandleMouseClick));
	e.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FN(Lab2Layer::HandleMouseMove));
}

void Lab2Layer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::Text("OBS: click on the screen to create a new vertex at the location!");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

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
	if(ImGui::Button("Generate a random new point"))
	{
		Engine::Ref<Engine::Vertex> vertex = Engine::CreateRef<Engine::Vertex>(Engine::Random::RandomFloat(-1.f, 1.f), Engine::Random::RandomFloat(-1.f, 1.f), 0.f);
		vertices->push_back(vertex);
		GlobalState::Instance().POINT_COUNT++;
		if (GlobalState::Instance().shouldDrawTriangles)
		{
			triangles_drawn = false;
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	if (GlobalState::Instance().shouldDrawTriangles) {
		if (ImGui::RadioButton("Same", &GlobalState::Instance().color_type_choice, 0)) {
			triangles_drawn = false;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Distance", &GlobalState::Instance().color_type_choice, 1)) {
			triangles_drawn = false;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("4 coloring", &GlobalState::Instance().color_type_choice, 2)) {
			triangles_drawn = false;
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (GlobalState::Instance().color_type_choice == 0) {
			if (ImGui::ColorEdit3("Triangle color", GlobalState::Instance().uniform_color)) {
				triangles_drawn = false;
			}
		}

		if (GlobalState::Instance().shouldDrawTriangles && !triangles_drawn)
		{
			CreateTriangles();
			triangles_drawn = true;
		}
	}

	ImGui::End();
}
