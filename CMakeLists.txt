# CMakeList.txt : CMake project for JonTool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("JonTool")

add_subdirectory("Lib/raylib" EXCLUDE_FROM_ALL)
add_subdirectory("Lib/imgui" EXCLUDE_FROM_ALL)
#dont add rlimgui cause it doesnt have a cmake ill just include it directly

# Add source to this project's executable.
add_executable (JonTool 
	"Source/Main.cpp"

	"Lib/rlImGui/rlImGui.cpp"
)

target_include_directories(JonTool PUBLIC "Include" "Lib/rlImGui")
target_link_libraries(JonTool PRIVATE imgui raylib_static)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET JonTool PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
