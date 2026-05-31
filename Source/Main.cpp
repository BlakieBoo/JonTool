#include "Common.hpp"
#include "Editor.hpp"
#include <rlImGui.h>
#include <imgui.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World!" << std::endl;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "JonTool");
	rlImGuiSetup(true);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();
		ClearBackground({36, 33, 79, 255});
		
		DrawText("Hello World!", 300, 300, 32, RAYWHITE);

		DrawEditor(GetFrameTime());

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
}