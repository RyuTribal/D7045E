#pragma once

#include "GraphicsNode.h"
#include "Camera.h"

namespace Engine

{

	struct GLFWwindow;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void BeginFrame(const int* rgb);

		void Draw(Ref<GraphicsNode> node);

		void EndFrame();

		static void CreateRenderer()
		{
			if(!s_Instance)
			{
				s_Instance = new Renderer();
			}
		}
		static Renderer* Get() { return s_Instance; }

		Camera& GetCamera() { return m_CurrentCamera; }


	private:

		static Renderer* s_Instance;
		Camera m_CurrentCamera;
	};

	// This is so the spd log library can print this data structure
	inline std::ostream& operator<<(std::ostream& os, const Vertex& v) {
		return os << "{" << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const std::vector<Vertex>& vec) {
		os << "[ \n";
		int row_items_count = 0;
		const int max_row_items = 2;
		for (size_t i = 0; i < vec.size(); ++i) {
			if(row_items_count == 0)
			{
				os << "\t";
			}
			os << vec[i];
			if (i < vec.size() - 1) {
				os << ", ";
				row_items_count++;
				if(row_items_count >= max_row_items)
				{
					os << "\n";
					row_items_count = 0;
				}
			}
		}

		os << "\n";

		return os << "]";
	}

	inline std::ostream& operator<<(std::ostream& os, Ref<Vertex> v) {
		return os << "{" << "x: " << v->x << ", y: " << v->y << ", z: " << v->z << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, Ref<std::vector<Ref<Vertex>>> vec) {
		os << "[ \n";
		int row_items_count = 0;
		const int max_row_items = 2;
		for (size_t i = 0; i < vec->size(); ++i) {
			if (row_items_count == 0)
			{
				os << "\t";
			}
			os << vec->at(i);
			if (i < vec->size() - 1) {
				os << ", ";
				row_items_count++;
				if (row_items_count >= max_row_items)
				{
					os << "\n";
					row_items_count = 0;
				}
			}
		}

		os << "\n";

		return os << "]";
	}

}
