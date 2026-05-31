#include <iostream>
#include <raylib.h>
#include <rlImGui.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World!" << std::endl;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "JonTool");
	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();
		ClearBackground(BLACK);
		DrawText("Hello World!", 300, 300, 32, RAYWHITE);
		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
}