#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "Settings.h"
#include "Shared.h"

const char *SHOW_WINDOW = "ShowWindow";
const char *CENTER_MOUSE = "CenterMouse";
const char *EXPERIMENTAL_FIGHT_LOGIC = "ExperimentalFightLogic";

namespace Settings
{
std::mutex Mutex;
json Settings = json::object();

void Load(std::filesystem::path aPath)
{
    if (!std::filesystem::exists(aPath))
        return;

    Settings::Mutex.lock();
    {
        try
        {
            auto file = std::ifstream(aPath);
            Settings = json::parse(file);
            file.close();
        }
        catch (json::parse_error &ex)
        {
            Globals::APIDefs->Log(LOGL_WARNING, "GW2_GolemRespawn", "Settings.json could not be parsed.");
            Globals::APIDefs->Log(LOGL_WARNING, "GW2_GolemRespawn", ex.what());
        }
    }
    Settings::Mutex.unlock();

    if (!Settings[SHOW_WINDOW].is_null())
        Settings[SHOW_WINDOW].get_to<bool>(ShowWindow);

    if (!Settings[CENTER_MOUSE].is_null())
        Settings[CENTER_MOUSE].get_to<bool>(CenterMouse);

    if (!Settings[EXPERIMENTAL_FIGHT_LOGIC].is_null())
        Settings[EXPERIMENTAL_FIGHT_LOGIC].get_to<bool>(ExperimentalFightLogic);
}

void Save(std::filesystem::path aPath)
{
    Settings::Mutex.lock();
    {
        Settings[SHOW_WINDOW] = ShowWindow;
        Settings[CENTER_MOUSE] = CenterMouse;
        Settings[EXPERIMENTAL_FIGHT_LOGIC] = ExperimentalFightLogic;

        auto file = std::ofstream(aPath);
        file << Settings.dump(1, '\t') << std::endl;
        file.close();
    }
    Settings::Mutex.unlock();

    Load(aPath);
}

void ToggleShowWindow(std::filesystem::path SettingsPath)
{
    ShowWindow = !ShowWindow;
    Settings[SHOW_WINDOW] = ShowWindow;
    Save(Globals::SettingsPath);
}

bool ShowWindow = true;
bool CenterMouse = true;
bool ExperimentalFightLogic = false;
} // namespace Settings
