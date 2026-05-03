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
            std::ifstream file(aPath);
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

    /* Widget */
    if (!Settings[SHOW_WINDOW].is_null())
        Settings[SHOW_WINDOW].get_to<bool>(ShowWindow);

    if (!Settings[CENTER_MOUSE].is_null())
        Settings[CENTER_MOUSE].get_to<bool>(CenterMouse);
}

void Save(std::filesystem::path aPath)
{
    Settings::Mutex.lock();
    {
        Settings[SHOW_WINDOW] = ShowWindow;
        Settings[CENTER_MOUSE] = CenterMouse;

        std::ofstream file(aPath);
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
} // namespace Settings
