#include <windows.h>

#include <chrono>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include "imgui.h"

#include "mumble/Mumble.h"

#include "MumbleUtils.h"
#include "Render.h"
#include "Settings.h"
#include "Shared.h"
#include "Types.h"
#include "Utils.h"
#include "Version.h"

namespace
{
static const std::string CHAT_MESSAGE = "/g Golem respawned";

constexpr int BASE_WIDTH = 2160;
constexpr int BASE_HEIGHT = 1440;

constexpr float REMOVE_X_RATIO = 1000.0f / BASE_WIDTH;  // ≈ 0.463
constexpr float REMOVE_Y_RATIO = 390.0f / BASE_HEIGHT;  // ≈ 0.271
constexpr float RESPAWN_X_RATIO = 1000.0f / BASE_WIDTH; // ≈ 0.463
constexpr float RESPAWN_Y_RATIO = 450.0f / BASE_HEIGHT; // ≈ 0.313

std::pair<POINT, POINT> GetScaledClickPositions()
{
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    POINT remove_pos, respawn_pos;
    remove_pos.x = static_cast<int>(screen_width * REMOVE_X_RATIO);
    remove_pos.y = static_cast<int>(screen_height * REMOVE_Y_RATIO);
    respawn_pos.x = static_cast<int>(screen_width * RESPAWN_X_RATIO);
    respawn_pos.y = static_cast<int>(screen_height * RESPAWN_Y_RATIO);

    return std::make_pair(remove_pos, respawn_pos);
}

std::string GetLogMessage()
{
    const auto curr_time = std::chrono::system_clock::now();
    const std::time_t curr_time_t = std::chrono::system_clock::to_time_t(curr_time);

    std::tm tm_info{};
    localtime_s(&tm_info, &curr_time_t);
    char time_buf[9];
    std::strftime(time_buf, sizeof(time_buf), "%H:%M:%S", &tm_info);
    const auto log_message = std::string("Golem reset triggered at ") + time_buf;

    return log_message;
}
} // namespace

RenderType::~RenderType()
{
}

void RenderType::render(ID3D11Device *pd3dDevice)
{
    Globals::RenderData.pd3dDevice = pd3dDevice;

    const auto map_valid = IsValidMap();

    if (!Settings::ShowWindow || !map_valid)
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

        const auto window_flags =
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

        const auto window_width = 300.0f;
        ImGui::SetNextWindowSize(ImVec2(window_width, 0), ImGuiCond_Always);
        if (ImGui::Begin("Golem Respawn Tool", &show_golem_window, window_flags))
        {

            const auto screen_width = GetSystemMetrics(SM_CXSCREEN);
            const auto screen_height = GetSystemMetrics(SM_CYSCREEN);
            auto [remove_pos, respawn_pos] = GetScaledClickPositions();

            // #if _DEBUG
            //             if (currently_in_fight)
            //                 ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Status: IN COMBAT");
            //             else
            //                 ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: Out of combat");
            //             ImGui::Text("Screen: %dx%d", screen_width, screen_height);
            //             ImGui::Text("Remove pos: (%d, %d)", remove_pos.x, remove_pos.y);
            //             ImGui::Text("Respawn pos: (%d, %d)", respawn_pos.x, respawn_pos.y);
            //             ImGui::Spacing();
            // #endif

            ImGui::Spacing();

            const auto button_width1 = (window_width - ImGui::GetStyle().ItemSpacing.x) * 0.35f;
            const auto button_width2 = (window_width - ImGui::GetStyle().ItemSpacing.x) * 0.65f;

            if (ImGui::Button("Respawn", ImVec2(button_width1, 40)))
            {
                const auto log_message = GetLogMessage();
                auto [remove_pos, respawn_pos] = GetScaledClickPositions();

                std::thread([=]() {
                    UseInteractionKey();
                    Sleep(200);
                    SimulateMouseClick(respawn_pos.x, respawn_pos.y);
                    Sleep(200);
                    SendChatMessage(log_message);
                }).detach();
            }

            ImGui::SameLine();

            if (ImGui::Button("Reset->Respawn", ImVec2(button_width2, 40)))
            {
                const auto log_message = GetLogMessage();
                auto [remove_pos, respawn_pos] = GetScaledClickPositions();

                std::thread([=]() {
                    UseInteractionKey();
                    Sleep(200);
                    SimulateMouseClick(remove_pos.x, remove_pos.y);
                    Sleep(200);
                    SimulateMouseClick(respawn_pos.x, respawn_pos.y);
                    Sleep(200);
                    SendChatMessage(log_message);
                }).detach();
            }
        }

        ImGui::End();
    }

    was_in_fight = currently_in_fight;
}
