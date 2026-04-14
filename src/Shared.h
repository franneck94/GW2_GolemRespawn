#ifndef SHARED_H
#define SHARED_H

#include <array>
#include <filesystem>
#include <string>

#include "arcdps/ArcDPS.h"
#include "mumble/Mumble.h"
#include "nexus/Nexus.h"
#include "rtapi/RTAPI.hpp"

#include "Render.h"
#include "Types.h"

namespace Globals
{
extern AddonAPI_t *APIDefs;
extern NexusLinkData_t *NexusLink;
extern RTAPI::RealTimeData *RTAPIData;
extern std::string AccountName;
extern Mumble::Data *MumbleData;
extern ArcDPS::Exports ArcExports;

extern std::filesystem::path SettingsPath;

extern RenderDataType RenderData;
extern RenderType Render;
extern Mumble::Identity Identity;
extern uint32_t SkillIconSize;
}; // namespace Globals

#endif
