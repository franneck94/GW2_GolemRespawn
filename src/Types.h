#pragma once

#include <cstdint>
#include <d3d11.h>
#include <filesystem>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "arcdps/ArcDPS.h"

#include "Defines.h"

struct RenderDataType
{
    ID3D11Device *pd3dDevice = nullptr;
};

enum class ProfessionID : uint32_t
{
    UNKNOWN = 0,
    GUARDIAN = 1,
    WARRIOR = 2,
    ENGINEER = 3,
    RANGER = 4,
    THIEF = 5,
    ELEMENTALIST = 6,
    MESMER = 7,
    NECROMANCER = 8,
    REVENANT = 9,
};

enum class EliteSpecID : uint32_t
{
    // Elementalist
    Catalyst = 67,
    Evoker = 80,
    Tempest = 48,
    Weaver = 56,

    // Engineer
    Amalgam = 75,
    Holosmith = 57,
    Mechanist = 70,
    Scrapper = 43,

    // Guardian
    Dragonhunter = 27,
    Firebrand = 62,
    Luminary = 81,
    Willbender = 65,

    // Mesmer
    Chronomancer = 40,
    Mirage = 59,
    Troubadour = 73,
    Virtuoso = 66,

    // Necromancer
    Harbinger = 64,
    Reaper = 34,
    Ritualist = 76,
    Scourge = 60,

    // Ranger
    Druid = 5,
    Galeshot = 78,
    Soulbeast = 55,
    Untamed = 72,

    // Revenant
    Conduit = 79,
    Herald = 52,
    Renegade = 63,
    Vindicator = 69,

    // Thief
    Antiquary = 77,
    Daredevil = 7,
    Deadeye = 58,
    Specter = 71,

    // Warrior
    Berserker = 18,
    Bladesworn = 68,
    Paragon = 74,
    Spellbreaker = 61,

    // Default/Unknown
    Unknown = 0
};

struct MetaData
{
    double overall_dps;
    std::string name;
    std::string url;
    std::string benchmark_type;
    std::string profession;
    ProfessionID profession_id;
    std::string elite_spec;
    EliteSpecID elite_spec_id;
    std::string build_type;
    std::string url_name;
    std::string dps_report_url;
    std::string html_file_path;
};
