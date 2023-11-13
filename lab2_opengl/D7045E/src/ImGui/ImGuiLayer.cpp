#include "pch.h"
#include "ImGui/ImGuiLayer.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Core/GlobalState.h"

namespace D7045E {

	ImGuiLayer::ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");


	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}



	void ImGuiLayer::OnImGuiRender()
	{
		RenderDockSpace();
		const char* items[] = { "Small set", "Medium set", "Large set" };
		if(ImGui::Combo("Point cloud set", &GlobalState::Instance().choice, items, IM_ARRAYSIZE(items)))
		{
			GlobalState::Instance().POINT_COUNT = GlobalState::Instance().point_sets.at(GlobalState::Instance().choice);
		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if(ImGui::InputInt("Point Size", &GlobalState::Instance().POINT_COUNT))
		{
			for(size_t i = 0; i < GlobalState::Instance().point_sets.size(); i++)
			{
				if(GlobalState::Instance().point_sets[i] == GlobalState::Instance().POINT_COUNT)
				{
					GlobalState::Instance().choice = i;
					break;
				}
			}
		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Checkbox("Draw triangles", &GlobalState::Instance().shouldDrawTriangles);

		// Very important!!!!!!!!!!!
		ImGui::End();
	}

	void ImGuiLayer::RenderDockSpace()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Window", nullptr, window_flags | ImGuiWindowFlags_NoBackground);
		ImGui::PopStyleVar();

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		static bool is_layout_initialized = false;
		if (!is_layout_initialized)
		{
			is_layout_initialized = true;

			ImGui::DockBuilderRemoveNode(dockspace_id); 
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 1.0f / 3.0f, nullptr, &dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}

		ImGui::End();
		ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		ImGui::Begin("Right Dock", nullptr, ImGuiWindowFlags_NoCollapse);
	}



	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		CORE_INFO("Window width: {0}, Window height: {1}", app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}
