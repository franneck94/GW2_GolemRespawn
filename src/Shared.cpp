#include <array>
#include <filesystem>
#include <string>

#include "arcdps/ArcDPS.h"
#include "nexus/Nexus.h"
#include "rtapi/RTAPI.hpp"
#include "mumble/Mumble.h"

#include "Render.h"
#include "Shared.h"
#include "Types.h"
#include "Version.h"

namespace Globals
{
AddonAPI_t *APIDefs = nullptr;
NexusLinkData_t *NexusLink = nullptr;
RTAPI::RealTimeData *RTAPIData = nullptr;
Mumble::Data *MumbleData = nullptr;
std::string AccountName;
ArcDPS::Exports ArcExports = {};

std::filesystem::path Globals::SettingsPath;
RenderDataType Globals::RenderData{};
RenderType Globals::Render{};
Mumble::Identity Identity = {};
uint32_t SkillIconSize = 32;
}; // namespace Globals
