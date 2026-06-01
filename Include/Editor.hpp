#pragma once

#include "Common.hpp"
#include "Jon.hpp"

void DrawEditor(float delta);
void DestroyEditor();
bool IsEditorHovered();
Jon* GetCurrentJon();

bool RefLinesShouldBeOnTop();