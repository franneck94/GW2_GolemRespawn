#include <cmath>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include "imgui.h"
#include "mumble/Mumble.h"
#include "nexus/Nexus.h"
#include "rtapi/RTAPI.hpp"

#include "TEX_GW2_GolemRespawnHOVER_data.h"
#include "TEX_GW2_GolemRespawnNORMAL_data.h"

#include "Constants.h"
#include "Render.h"
#include "Shared.h"
#include "Settings.h"
#include "Version.h"
#include "MumbleUtils.h"

namespace dx = DirectX;

void AddonLoad(AddonAPI_t *aApi);
void AddonUnload();
void AddonRender();
void AddonOptions();

HMODULE hSelf;
AddonDefinition_t AddonDef{};
std::filesystem::path AddonPath;
ID3D11Device *pd3dDevice = nullptr;

void ToggleShowWindowGW2_GolemRespawn(const char *, bool isKeyDown)
{
}

void RegisterQuickAccessShortcut()
{
    Globals::APIDefs->QuickAccess_Add("SHORTCUT_GW2_GolemRespawn",
                                      "TEX_GW2_GolemRespawn_NORMAL",
                                      "TEX_GW2_GolemRespawn_HOVER",
                                      KB_TOGGLE_GW2_GolemRespawn,
                                      "Toggle GW2_GolemRespawn Window");
    Globals::APIDefs->InputBinds_RegisterWithString(KB_TOGGLE_GW2_GolemRespawn, ToggleShowWindowGW2_GolemRespawn, "(null)");
}

void DeregisterQuickAccessShortcut()
{
    Globals::APIDefs->QuickAccess_Remove("SHORTCUT_GW2_GolemRespawn");
    Globals::APIDefs->InputBinds_Deregister(KB_TOGGLE_GW2_GolemRespawn);
}

extern "C" __declspec(dllexport) AddonDefinition_t *GetAddonDef()
{
    AddonDef.Signature = -24435663;
    AddonDef.APIVersion = NEXUS_API_VERSION;
    AddonDef.Name = "GW2_GolemRespawn";
    AddonDef.Version.Major = MAJOR;
    AddonDef.Version.Minor = MINOR;
    AddonDef.Version.Build = BUILD;
    AddonDef.Version.Revision = REVISION;
    AddonDef.Author = "Franneck.1274";
    AddonDef.Description = "Respawn golem without fat fingering the wrong button";
    AddonDef.Load = AddonLoad;
    AddonDef.Unload = AddonUnload;
    AddonDef.Flags = AF_None;
    AddonDef.Provider = UP_GitHub;
    AddonDef.UpdateLink = "https://github.com/franneck94/GW2_GolemRespawn";

    return &AddonDef;
}

void OnAddonLoaded(int *aSignature)
{
    if (!aSignature)
    {
        return;
    }
}
void OnAddonUnloaded(int *aSignature)
{
    if (!aSignature)
    {
        return;
    }
}

void AddonLoad(AddonAPI_t *aApi)
{
    if (!aApi)
        return;

    Globals::APIDefs = aApi;
    ImGui::SetCurrentContext((ImGuiContext *)Globals::APIDefs->ImguiContext);
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void *))Globals::APIDefs->ImguiMalloc,
                                 (void (*)(void *, void *))Globals::APIDefs->ImguiFree);

    Globals::NexusLink = (NexusLinkData_t *)Globals::APIDefs->DataLink_Get("DL_NEXUS_LINK");
    Globals::MumbleData = (Mumble::Data *)Globals::APIDefs->DataLink_Get("DL_MUMBLE_LINK");
    Globals::RTAPIData = (RTAPI::RealTimeData *)Globals::APIDefs->DataLink_Get("DL_RTAPI");

    Globals::APIDefs->GUI_Register(RT_Render, AddonRender);
    Globals::APIDefs->GUI_Register(RT_OptionsRender, AddonOptions);

    AddonPath = Globals::APIDefs->Paths_GetAddonDirectory("GW2_GolemRespawn");
    Globals::SettingsPath = AddonPath / "settings.json";

    Settings::Load(Globals::SettingsPath);

    Globals::SkillIconSize = 64.0F;
    Globals::APIDefs->Textures_LoadFromMemory("TEX_GW2_GolemRespawn_NORMAL",
                                              (void *)ARR_GW2_GolemRespawnNORMAL,
                                              ARR_GW2_GolemRespawnNORMAL_size,
                                              nullptr);
    Globals::APIDefs->Textures_LoadFromMemory("TEX_GW2_GolemRespawn_HOVER",
                                              (void *)ARR_GW2_GolemRespawnHOVER,
                                              ARR_GW2_GolemRespawnHOVER_size,
                                              nullptr);
    RegisterQuickAccessShortcut();

    if (Globals::APIDefs && Globals::APIDefs->DataLink_Get)
    {
        auto *pSwapChain = (IDXGISwapChain *)Globals::APIDefs->SwapChain;
        if (pSwapChain)
        {
            auto hr = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pd3dDevice);
            if (FAILED(hr))
                pd3dDevice = nullptr;
        }
    }

    (void)Globals::APIDefs->Log(LOGL_DEBUG, "GW2_GolemRespawn", "Loaded Addon");
}

void AddonUnload()
{
    if (pd3dDevice)
        pd3dDevice->Release();

    Globals::APIDefs->GUI_Deregister(AddonRender);
    Globals::APIDefs->GUI_Deregister(AddonOptions);

    Globals::NexusLink = nullptr;
    Globals::RTAPIData = nullptr;

    Settings::Save(Globals::SettingsPath);

    DeregisterQuickAccessShortcut();

    (void)Globals::APIDefs->Log(LOGL_DEBUG, "GW2_GolemRespawn", "Unloaded Addon");
}

void TriggerParseMumble()
{
    static auto last_parse_time = std::chrono::steady_clock::now();
    static auto pending_identity = Mumble::Identity{};
    static auto pending_count = 0;

    const auto now = std::chrono::steady_clock::now();
    const auto time_since_last_parse = std::chrono::duration_cast<std::chrono::seconds>(now - last_parse_time).count();

    if (time_since_last_parse >= 1 || time_since_last_parse == 0)
    {
        auto current_identity = ParseMumbleIdentity(Globals::MumbleData->Identity);

        if (current_identity.Profession != Globals::Identity.Profession)
        {
            if (current_identity.Profession != pending_identity.Profession)
            {
                pending_identity = current_identity;
                pending_count = 1;
            }
            else
            {
                pending_count++;

                if (pending_count >= 3)
                {
                    Globals::Identity = current_identity;
                    pending_count = 0;
                    pending_identity = Mumble::Identity{};
                    (void)Globals::APIDefs->Log(LOGL_INFO, "GW2_GolemRespawn", "Detected other profession.");
                }
            }
        }
        else
        {
            Globals::Identity = current_identity;
            pending_count = 0;
            pending_identity = Mumble::Identity{};
        }

        last_parse_time = now;
    }
}

void AddonRender()
{
    static auto profession = ProfessionID::UNKNOWN;

    if ((!Globals::NexusLink) || (!Globals::NexusLink->IsGameplay) || (!Settings::ShowWindow))
        return;

    TriggerParseMumble();
    const auto curr_profession = static_cast<ProfessionID>(Globals::Identity.Profession);
    if (profession != curr_profession)
        profession = curr_profession;

    Globals::Render.render(pd3dDevice);
}

void AddonOptions()
{
}
