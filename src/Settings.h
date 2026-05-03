#ifndef SETTINGS_H
#define SETTINGS_H

#include <filesystem>
#include <mutex>
#include <string>
#include <map>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "mumble/Mumble.h"
#include "nexus/Nexus.h"
#include "rtapi/RTAPI.hpp"

extern const char *SHOW_WINDOW;
extern const char *CENTER_MOUSE;

namespace Settings
{
extern std::mutex Mutex;
extern json Settings;

void Load(std::filesystem::path aPath);
void Save(std::filesystem::path aPath);
void ToggleShowWindow(std::filesystem::path SettingsPath);

extern bool ShowWindow;
extern bool CenterMouse;
} // namespace Settings

#endif
