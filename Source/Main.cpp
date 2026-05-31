#include "Common.hpp"
#include "Editor.hpp"
#include "Scene.hpp"
#include <rlImGui.h>
#include <imgui.h>
#include <rlgl.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World!" << std::endl;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "JonTool");
	rlImGuiSetup(true);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	rlDisableBackfaceCulling();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();
		ClearBackground({36, 33, 79, 255});

		DrawEditor(GetFrameTime());
		DrawScene();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
}