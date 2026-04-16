#include <windows.h>

#include <commdlg.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "user32.lib")
#include <d3d11.h>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <numbers>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "imgui.h"

#include "mumble/Mumble.h"

#include "Defines.h"
#include "MumbleUtils.h"
#include "Render.h"
#include "Settings.h"
#include "Shared.h"
#include "Textures.h"
#include "Types.h"
#include "Utils.h"
#include "Version.h"

namespace
{
static const std::string CHAT_MESSAGE = "Golem respawned!";

constexpr int total_size_x = 2160;
constexpr int total_size_y = 1440;

constexpr int remove_x = 1000;
constexpr int remove_y = 390;

constexpr int respawn_x = 1000;
constexpr int respawn_y = 450;
} // namespace

RenderType::~RenderType()
{
}

void RenderType::render(ID3D11Device *pd3dDevice)
{
    Globals::RenderData.pd3dDevice = pd3dDevice;

    if (!Settings::ShowWindow)
        return;

    static bool was_in_fight = false;
    static bool show_golem_window = true;
    static ImVec2 window_pos = ImVec2(100, 100);
    static ImVec2 window_size = ImVec2(200, 120);
    static bool first_time = true;

    bool currently_in_fight = IsInfight();

    if (show_golem_window)
    {
        if (first_time)
        {
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);
            first_time = false;
        }

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::Begin("Golem Respawn Tool", &show_golem_window, window_flags))
        {
            ImGui::Text("GW2 Golem Respawn Helper");
            ImGui::Separator();

            if (currently_in_fight)
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Status: IN COMBAT");
            else
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: Out of combat");

            ImGui::Spacing();

            if (ImGui::Button("Reset Golem", ImVec2(150, 40)))
            {
                if (currently_in_fight)
                {
                    std::thread([=]() {
                        SendChatMessage(CHAT_MESSAGE);
                        Sleep(200);
                        SimulateMouseClick(remove_x, remove_y);
                        Sleep(200);
                        SimulateMouseClick(respawn_x, respawn_y);
                    }).detach();
                }
            }
        }

        ImGui::End();
    }

    was_in_fight = currently_in_fight;
}
