#include <windows.h>

#include <chrono>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <tuple>

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
static const std::string CHAT_MESSAGE = "Golem respawned";

constexpr int BASE_WIDTH = 2160;
constexpr int BASE_HEIGHT = 1440;

constexpr float REMOVE_X_RATIO = 0.45;
constexpr float REMOVE_Y_RATIO = 390.0f / BASE_HEIGHT;
constexpr float RESPAWN_X_RATIO = 0.45;
constexpr float RESPAWN_Y_RATIO = 445.0f / BASE_HEIGHT;
constexpr float CLOSE_X_RATIO = 0.45;
constexpr float CLOSE_Y_RATIO = 510.0f / BASE_HEIGHT;

constexpr float MIDDLE_WINDOW_X = 0.5f;
constexpr float MIDDLE_WINDOW_Y = 0.5f;

constexpr int CLICK_DELAY_MS = 250;

auto GetScaledClickPositions()
{
    auto screen_width = GetSystemMetrics(SM_CXSCREEN);
    auto screen_height = GetSystemMetrics(SM_CYSCREEN);

    POINT remove_pos, respawn_pos, close_pos, middle_pos;
    remove_pos.x = static_cast<int>(screen_width * REMOVE_X_RATIO);
    remove_pos.y = static_cast<int>(screen_height * REMOVE_Y_RATIO);
    respawn_pos.x = static_cast<int>(screen_width * RESPAWN_X_RATIO);
    respawn_pos.y = static_cast<int>(screen_height * RESPAWN_Y_RATIO);
    close_pos.x = static_cast<int>(screen_width * CLOSE_X_RATIO);
    close_pos.y = static_cast<int>(screen_height * CLOSE_Y_RATIO);
    middle_pos.x = static_cast<int>(screen_width * MIDDLE_WINDOW_X);
    middle_pos.y = static_cast<int>(screen_height * MIDDLE_WINDOW_Y);

    return std::make_tuple(remove_pos, respawn_pos, close_pos, middle_pos);
}

std::string GetLogMessage()
{
    const auto curr_time = std::chrono::system_clock::now();
    const auto curr_time_t = std::chrono::system_clock::to_time_t(curr_time);

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

    static auto was_in_fight = false;
    static auto show_golem_window = true;
    static auto window_pos = ImVec2(100, 100);
    static auto window_size = ImVec2(200, 120);
    static auto first_time = true;
    static auto was_in_fight_since_reset = false;

    auto currently_in_fight = IsInfight();

    if (Settings::ExperimentalFightLogic)
    {
        if (currently_in_fight && !was_in_fight)
        {
            was_in_fight_since_reset = true;
        }
    }

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

        const auto window_width = 270.0f;
        ImGui::SetNextWindowSize(ImVec2(window_width, 0), ImGuiCond_Always);
        if (ImGui::Begin("Golem Respawn Tool", &show_golem_window, window_flags))
        {

            const auto screen_width = GetSystemMetrics(SM_CXSCREEN);
            const auto screen_height = GetSystemMetrics(SM_CYSCREEN);
            auto [remove_pos, respawn_pos, close_pos, middle_pos] = GetScaledClickPositions();

            const auto button_width1 = (window_width - ImGui::GetStyle().ItemSpacing.x) * 0.35f;
            const auto button_width2 = (window_width - ImGui::GetStyle().ItemSpacing.x) * 0.65f;

            auto show_respawn_button = true;
            auto show_reset_respawn_button = true;

            if (Settings::ExperimentalFightLogic)
            {
                if (currently_in_fight)
                {
                    show_respawn_button = false;
                    show_reset_respawn_button = true;
                }
                else if (was_in_fight_since_reset)
                {
                    show_respawn_button = true;
                    show_reset_respawn_button = false;
                }
            }

            auto button_on_same_line = false;

            if (show_respawn_button && ImGui::Button("Respawn", ImVec2(show_reset_respawn_button ? button_width1 : window_width - ImGui::GetStyle().WindowPadding.x * 2, 40)))
            {
                const auto log_message = GetLogMessage();
                auto [remove_pos, respawn_pos, close_pos, middle_pos] = GetScaledClickPositions();

                if (Settings::ExperimentalFightLogic)
                {
                    was_in_fight_since_reset = false;
                }

                std::thread([=]() {
                    UseInteractionKey();
                    Sleep(CLICK_DELAY_MS);
                    SimulateMouseClick(respawn_pos.x, respawn_pos.y);
                    Sleep(CLICK_DELAY_MS);
                    SimulateMouseClick(close_pos.x, close_pos.y);
                    Sleep(CLICK_DELAY_MS);
                    if (Settings::CenterMouse)
                        MoveMouse(middle_pos.x, middle_pos.y);
                }).detach();
                button_on_same_line = true;
            }

            if (show_respawn_button && show_reset_respawn_button)
            {
                ImGui::SameLine();
            }

            if (show_reset_respawn_button && ImGui::Button("Reset->Respawn", ImVec2(show_respawn_button ? button_width2 : window_width - ImGui::GetStyle().WindowPadding.x * 2, 40)))
            {
                const auto log_message = GetLogMessage();
                auto [remove_pos, respawn_pos, close_pos, middle_pos] = GetScaledClickPositions();

                if (Settings::ExperimentalFightLogic)
                {
                    was_in_fight_since_reset = false;
                }

                std::thread([=]() {
                    UseInteractionKey();
                    Sleep(CLICK_DELAY_MS);
                    SimulateMouseClick(remove_pos.x, remove_pos.y);
                    Sleep(CLICK_DELAY_MS);
                    SimulateMouseClick(respawn_pos.x, respawn_pos.y);
                    Sleep(CLICK_DELAY_MS);
                    SimulateMouseClick(close_pos.x, close_pos.y);
                    Sleep(CLICK_DELAY_MS);
                    if (Settings::CenterMouse) {
                        MoveMouse(middle_pos.x, middle_pos.y);
                    }
                }).detach();
            }
        }

        ImGui::End();
    }

    was_in_fight = currently_in_fight;
}
