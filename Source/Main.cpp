#include "Common.hpp"
#include "Editor.hpp"
#include "Scene.hpp"
#include "TextureList.hpp"
#include <rlImGui.h>
#include <imgui.h>
#include <rlgl.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World!" << std::endl;

	SetTraceLogLevel(LOG_ERROR);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "JonTool");
	SetTargetFPS(60);
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

		UploadQueuedTextures();
	}

	DestroyEditor();
	rlImGuiShutdown();
	//dont care about shutting down cause it just makes closing slower
	//CloseWindow();
}