#include "Editor.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nfd.h>
#include <fstream>
#include <algorithm>

static std::vector<Jon*> loadedJons = std::vector<Jon*>();
static std::vector<char*> jonNames = std::vector<char*>();
static std::vector<std::string> jonSavePaths = std::vector<std::string>();
static int curJon = 0;

static const char* ZTYPE_Names[] =
{
	"DEFAULT",
	"MOST_FAR",
	"FAR",
	"LITTLE_FAR",
	"LITTLE_NEAR",
	"NEAR",
	"MOST_NEAR"
};

static const char* TRANSTYPE_Names[] =
{
	"DEFAULT",
	"NORMAL",
	"ADD",
	"DEC",
	"REVERSE"
};

//changed these so its easier to understand
static const char* CTYPE_Names[] =
{
	"Hurt",
	"Hit",
	"Ex Point",
	"Ex Rect",
	"Ex Vector",
	"Push",
	"Center Offset",
	"Neck Snap",
	"Waist Snap",
	"Grab",
	"SP Guard",
	"R Leg Snap",
	"L Leg Snap",
	"Private Point 0",
	"Private Point 1",
	"Private Point 2",
	"Private Point 3",
	"Extended Point (Faust Afro Pos)"
};

static enum BoxType
{
	Rect,
	Point,
	Vector
};

static BoxType CTYPE_Types[]
{
	Rect,
	Rect,
	Point,
	Rect,
	Vector,
	Rect,
	Point,
	Point,
	Point,
	Rect,
	Rect,
	Point,
	Point,
	Point,
	Point,
	Point,
	Point,
	Point
};

static const char* LayerType_Names[] =
{
	"Normal",
	"Eyes",
	"Mouth"
};

static void Load()
{
	nfdpathset_t paths;
	nfdresult_t result = NFD_OpenDialogMultiple("jonbin", NULL, &paths);

	if (result == NFD_OKAY)
		for (int i = 0; i < paths.count; i++)
		{
			std::ifstream file = std::ifstream(&paths.buf[paths.indices[i]], std::ios_base::binary);
			file.seekg(0, file._Seekend);
			int fileSize = file.tellg();
			file.seekg(0, file._Seekbeg);

			uint8_t* fileDat = (uint8_t*)malloc(fileSize);
			file.read((char*)fileDat, fileSize);

			std::cout << "Loading Jonbin " << &paths.buf[paths.indices[i]] << "\n";
			Jon* jon = new Jon();
			jon->LoadFromFile(fileDat, fileSize);
			std::cout << "Done!" << "\n";

			std::string jonName = &paths.buf[paths.indices[i]];
			std::replace(jonName.begin(), jonName.end(), '\\', '/');
			jonName = jonName.substr(jonName.find_last_of('/') + 1);
			jonName = jonName.substr(0, jonName.find_last_of('.'));
			jon->jonName = jonName;

			auto iterator = std::find(jonSavePaths.begin(), jonSavePaths.end(), &paths.buf[paths.indices[i]]);
			if (iterator != jonSavePaths.end())
			{
				loadedJons[iterator - jonSavePaths.begin()] = jon;
				jonNames[iterator - jonSavePaths.begin()] = jon->jonName.data();
			}
			else
			{
				loadedJons.push_back(jon);
				jonNames.push_back(jon->jonName.data());
				jonSavePaths.push_back(&paths.buf[paths.indices[i]]);
			}
		}

	std::cout << std::endl;

	curJon = loadedJons.size() - 1;
}

static void Save()
{

}

static void DrawMenu()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Jon"))
		{
			Jon* jon = new Jon();
			loadedJons.push_back(jon);
			jon->jonName = std::string("New Jon ") + std::to_string(loadedJons.size());
			jonNames.push_back(jon->jonName.data());
			jonSavePaths.push_back("");
			curJon = loadedJons.size() - 1;
		}

		if (ImGui::MenuItem("Load Jons"))
			Load();

		if (loadedJons.size() && ImGui::MenuItem("Save Current Jon"))
			Save();

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

static void DrawJonEdit()
{
	if (!ImGui::Begin("JonEdit"))
	{
		ImGui::End();
		return;
	}

	if (!loadedJons.size())
	{
		ImGui::Text("No Jons Loaded");
		ImGui::End();
		return;
	}

	ImGui::SetNextItemWidth(135.0f);
	ImGui::Combo("Current Jon", &curJon, jonNames.data(), jonNames.size());

	Jon* jon = loadedJons[curJon];

	if (ImGui::TreeNode("Used Textures"))
	{
		if (jon->usedTextures.size() < 8)
		{
			ImGui::SetNextItemWidth(70.0f);
			if (ImGui::Button("Add Texture"))
			{
				jon->usedTextures.push_back(std::string());
				jon->usedTextures[jon->usedTextures.size() - 1].resize(32);
			}
		}

		if (!jon->usedTextures.size())
			ImGui::Text("Jon Uses no textures");
		for (int i = 0; i < jon->usedTextures.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::SetNextItemWidth(120.0f);
			//max 28 chars so we can fit file extention 
			ImGui::InputText("", jon->usedTextures[i].data(), 28);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			if (ImGui::Button("Remove"))
			{
				i--;
				jon->usedTextures.erase(jon->usedTextures.begin() + i);
			}
			ImGui::PopID();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprites"))
	{
		static int curSprite = 0;

		if (jon->sprites.size())
		{
			if (curSprite >= jon->sprites.size())
				curSprite = jon->sprites.size() - 1;

			ImGui::SetNextItemWidth(160.0f);
			ImGui::SliderInt("Current Sprite", &curSprite, 0, jon->sprites.size() - 1);

			ImGui::SetNextItemWidth(65.0f);
			if (ImGui::Button("Add Sprite"))
			{
				jon->sprites.push_back(JonSpriteRect());
				curSprite = jon->sprites.size() - 1;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(65.0f);
			if (ImGui::Button("Remove Sprite"))
			{
				jon->sprites.erase(jon->sprites.begin() + curSprite);
				if (curSprite >= jon->sprites.size())
					curSprite = jon->sprites.size() - 1;
			}

			if (curSprite > 0)
			{
				ImGui::SetNextItemWidth(65.0f);
				if (ImGui::Button("Sprite Up"))
				{
					JonSpriteRect spriteTemp = jon->sprites[curSprite];
					jon->sprites.erase(jon->sprites.begin() + curSprite);
					jon->sprites.insert(jon->sprites.begin() + curSprite - 1, spriteTemp);
					curSprite--;
				}
			}
			if (curSprite < jon->sprites.size() - 1)
			{
				if (curSprite > 0)
					ImGui::SameLine();
				ImGui::SetNextItemWidth(65.0f);
				if (ImGui::Button("Sprite Down"))
				{
					JonSpriteRect spriteTemp = jon->sprites[curSprite];
					jon->sprites.erase(jon->sprites.begin() + curSprite);
					jon->sprites.insert(jon->sprites.begin() + curSprite + 1, spriteTemp);
					curSprite++;
				}
			}

			JonSpriteRect& sprite = jon->sprites[curSprite];

			ImGui::SetNextItemWidth(150.0f);
			ImGui::DragFloat2("UV Pos", &sprite.uvRect.x);
			ImGui::SetNextItemWidth(150.0f);
			ImGui::DragFloat2("UV Size", &sprite.uvRect.width);

			ImGui::SetNextItemWidth(150.0f);
			ImGui::DragFloat2("Pos", &sprite.positionRect.x);
			ImGui::SetNextItemWidth(150.0f);
			ImGui::DragFloat2("Size", &sprite.positionRect.width);

			ImGui::SetNextItemWidth(160.0f);
			ImGui::Combo("Z Type", (int*)&sprite.zType, ZTYPE_Names, ZTYPE_NUM);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragFloat("Z Offset", &sprite.zOffset);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragFloat("Alpha", &sprite.alpha, 0.05f, 0.0f, 1.0f);

			ImGui::SetNextItemWidth(160.0f);
			ImGui::Combo("Blend Mode", (int*)&sprite.transType, TRANSTYPE_Names, TTYPE_NUM);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragFloat("Rotation", &sprite.rotZ);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragFloat("Unknown 0", &sprite.unknown0);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragFloat("Unknown 1", &sprite.unknown1);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::SliderInt("Texture", (int*)&sprite.spriteIndex, 0, jon->usedTextures.size());
			if (jon->usedTextures.size())
			{
				if (sprite.spriteIndex)
					ImGui::Text(jon->usedTextures[sprite.spriteIndex - 1].c_str());
				else
					ImGui::Text(jon->usedTextures[std::clamp(sprite.spriteIndex, 0U, (uint32_t)(jon->usedTextures.size() - 1))].c_str());
			}
			else
				ImGui::Text("Jon has no textures to use.");

			ImGui::SetNextItemWidth(160.0f);
			ImGui::Combo("Layer Type", (int*)&sprite.layerType, LayerType_Names, LayerType_NUM);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragInt("Unknown 3", (int*)&sprite.unknown3);

			ImGui::SetNextItemWidth(130.0f);
			ImGui::DragInt("Palette ", (int*)&sprite.palette, 0.25f, 0, 6);
		}
		else
		{
			ImGui::Text("Jon has no Sprites");
			ImGui::SetNextItemWidth(65.0f);
			if (ImGui::Button("Add Sprite"))
			{
				jon->sprites.push_back(JonSpriteRect());
				curSprite = jon->sprites.size() - 1;
			}
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Collision"))
	{
		static int curBox = 0;

		if (jon->collisions.size())
		{
			if (curBox >= jon->collisions.size())
				curBox = jon->collisions.size() - 1;

			ImGui::SetNextItemWidth(160.0f);
			ImGui::SliderInt("Current Collision", &curBox, 0, jon->collisions.size() - 1);

			ImGui::SetNextItemWidth(65.0f);
			if (ImGui::Button("Add Col"))
			{
				jon->collisions.push_back(JonCollisionRect());
				curBox = jon->collisions.size() - 1;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(65.0f);
			if (ImGui::Button("Remove Col"))
			{
				jon->collisions.erase(jon->collisions.begin() + curBox);
				if (curBox >= jon->collisions.size())
					curBox = jon->collisions.size() - 1;
			}

			JonCollisionRect& col = jon->collisions[curBox];

			ImGui::SetNextItemWidth(160.0f);
			ImGui::Combo("Box Type", (int*)&col.collisionType, CTYPE_Names, CTYPE_NUM);
			
			switch (CTYPE_Types[col.collisionType])
			{
			default:
			case Rect:
				ImGui::SetNextItemWidth(150.0f);
				ImGui::DragFloat2("Pos", &col.collisionRect.x);
				ImGui::SetNextItemWidth(150.0f);
				ImGui::DragFloat2("Size", &col.collisionRect.width);
				break;
			case Point:
				ImGui::SetNextItemWidth(150.0f);
				ImGui::DragFloat2("Pos", &col.collisionRect.x);
				break;
			case Vector:
				ImGui::SetNextItemWidth(150.0f);
				ImGui::DragFloat2("Start", &col.collisionRect.x);
				ImGui::SetNextItemWidth(150.0f);
				ImGui::DragFloat2("End", &col.collisionRect.width);
				break;
			}
		}
		else
		{
			ImGui::Text("Jon has no Collisions");
			ImGui::SetNextItemWidth(65.0f);
			if (ImGui::Button("Add Col"))
			{
				jon->collisions.push_back(JonCollisionRect());
				curBox = jon->collisions.size() - 1;
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

static void DrawTimeline(float delta)
{
	if (!ImGui::Begin("Timeline"))
	{
		ImGui::End();
		return;
	}

	ImGui::End();
}

void DrawEditor(float delta)
{
	if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
	{
		if (IsKeyDown(KEY_O))
			Load();
		if (IsKeyDown(KEY_S) && loadedJons.size())
			Save();
	}

	DrawMenu();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Dock Window", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoBackground
	);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace = ImGui::GetID("Dock Space");
	if (!ImGui::DockBuilderGetNode(dockspace))
	{
		ImGui::DockBuilderRemoveNode(dockspace);
		ImGui::DockBuilderAddNode(dockspace, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace, viewport->WorkSize);

		ImGuiID toSplit = dockspace;
		ImGuiID dockLeftId = ImGui::DockBuilderSplitNode(toSplit, ImGuiDir_Left, 0.20f, nullptr, &toSplit);
		ImGuiID dockBottomId = ImGui::DockBuilderSplitNode(toSplit, ImGuiDir_Down, 0.15f, nullptr, &toSplit);

		ImGui::DockBuilderDockWindow("JonEdit", dockLeftId);
		ImGui::DockBuilderDockWindow("Timeline", dockBottomId);
	}
	ImGui::DockSpace(dockspace, ImVec2(0.0f, 0.0f),
		ImGuiDockNodeFlags_PassthruCentralNode |
		ImGuiDockNodeFlags_NoDockingInCentralNode |
		ImGuiDockNodeFlags_AutoHideTabBar
	);

	ImGui::End();

	DrawJonEdit();
	DrawTimeline(delta);
}