#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Types.h"

ProfessionID _string_to_profession(const std::string &lower_name)
{
    if (lower_name == "guardian")
        return ProfessionID::GUARDIAN;
    if (lower_name == "warrior")
        return ProfessionID::WARRIOR;
    if (lower_name == "engineer")
        return ProfessionID::ENGINEER;
    if (lower_name == "ranger")
        return ProfessionID::RANGER;
    if (lower_name == "thief")
        return ProfessionID::THIEF;
    if (lower_name == "elementalist")
        return ProfessionID::ELEMENTALIST;
    if (lower_name == "mesmer")
        return ProfessionID::MESMER;
    if (lower_name == "necromancer")
        return ProfessionID::NECROMANCER;
    if (lower_name == "revenant")
        return ProfessionID::REVENANT;

    return ProfessionID::UNKNOWN;
}

ProfessionID string_to_profession(const std::string &profession_name, const std::string &filename)
{
    auto lower_name = profession_name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

    const auto profession_id = _string_to_profession(lower_name);
    if (profession_id != ProfessionID::UNKNOWN)
        return profession_id;

    return _string_to_profession("");
}

EliteSpecID _string_to_elite_spec(const std::string &lower_name)
{
    // Elementalist
    if (lower_name == "catalyst")
        return EliteSpecID::Catalyst;
    if (lower_name == "evoker")
        return EliteSpecID::Evoker;
    if (lower_name == "tempest")
        return EliteSpecID::Tempest;
    if (lower_name == "weaver")
        return EliteSpecID::Weaver;

    // Engineer
    if (lower_name == "amalgam")
        return EliteSpecID::Amalgam;
    if (lower_name == "holosmith")
        return EliteSpecID::Holosmith;
    if (lower_name == "mechanist")
        return EliteSpecID::Mechanist;
    if (lower_name == "scrapper")
        return EliteSpecID::Scrapper;

    // Guardian
    if (lower_name == "dragonhunter")
        return EliteSpecID::Dragonhunter;
    if (lower_name == "firebrand")
        return EliteSpecID::Firebrand;
    if (lower_name == "luminary")
        return EliteSpecID::Luminary;
    if (lower_name == "willbender")
        return EliteSpecID::Willbender;

    // Mesmer
    if (lower_name == "chronomancer")
        return EliteSpecID::Chronomancer;
    if (lower_name == "mirage")
        return EliteSpecID::Mirage;
    if (lower_name == "troubadour")
        return EliteSpecID::Troubadour;
    if (lower_name == "virtuoso")
        return EliteSpecID::Virtuoso;

    // Necromancer
    if (lower_name == "harbinger")
        return EliteSpecID::Harbinger;
    if (lower_name == "reaper")
        return EliteSpecID::Reaper;
    if (lower_name == "ritualist")
        return EliteSpecID::Ritualist;
    if (lower_name == "scourge")
        return EliteSpecID::Scourge;

    // Ranger
    if (lower_name == "druid")
        return EliteSpecID::Druid;
    if (lower_name == "galeshot")
        return EliteSpecID::Galeshot;
    if (lower_name == "soulbeast")
        return EliteSpecID::Soulbeast;
    if (lower_name == "untamed")
        return EliteSpecID::Untamed;

    // Revenant
    if (lower_name == "conduit")
        return EliteSpecID::Conduit;
    if (lower_name == "herald")
        return EliteSpecID::Herald;
    if (lower_name == "renegade")
        return EliteSpecID::Renegade;
    if (lower_name == "vindicator")
        return EliteSpecID::Vindicator;

    // Thief
    if (lower_name == "antiquary")
        return EliteSpecID::Antiquary;
    if (lower_name == "daredevil")
        return EliteSpecID::Daredevil;
    if (lower_name == "deadeye")
        return EliteSpecID::Deadeye;
    if (lower_name == "specter" || lower_name == "spectre")
        return EliteSpecID::Specter;

    // Warrior
    if (lower_name == "berserker")
        return EliteSpecID::Berserker;
    if (lower_name == "bladesworn")
        return EliteSpecID::Bladesworn;
    if (lower_name == "paragon")
        return EliteSpecID::Paragon;
    if (lower_name == "spellbreaker")
        return EliteSpecID::Spellbreaker;

    return EliteSpecID::Unknown;
}

EliteSpecID string_to_elite_spec(const std::string &spec_name, const std::string &filename)
{
    auto lower_name = spec_name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

    const auto elite_spec_id = _string_to_elite_spec(lower_name);
    if (elite_spec_id != EliteSpecID::Unknown)
        return elite_spec_id;

    const auto class_idx = filename.find("class_");
    return _string_to_elite_spec(lower_name);
}

std::string profession_to_string(ProfessionID profession_id)
{
    switch (profession_id)
    {
    case ProfessionID::GUARDIAN:
        return "Guardian";
    case ProfessionID::WARRIOR:
        return "Warrior";
    case ProfessionID::ENGINEER:
        return "Engineer";
    case ProfessionID::RANGER:
        return "Ranger";
    case ProfessionID::THIEF:
        return "Thief";
    case ProfessionID::ELEMENTALIST:
        return "Elementalist";
    case ProfessionID::MESMER:
        return "Mesmer";
    case ProfessionID::NECROMANCER:
        return "Necromancer";
    case ProfessionID::REVENANT:
        return "Revenant";
    case ProfessionID::UNKNOWN:
    default:
        return "Unknown";
    }
}

std::string elite_spec_to_string(EliteSpecID elite_spec_id)
{
    switch (elite_spec_id)
    {
    // Elementalist
    case EliteSpecID::Catalyst:
        return "Catalyst";
    case EliteSpecID::Evoker:
        return "Evoker";
    case EliteSpecID::Tempest:
        return "Tempest";
    case EliteSpecID::Weaver:
        return "Weaver";

    // Engineer
    case EliteSpecID::Amalgam:
        return "Amalgam";
    case EliteSpecID::Holosmith:
        return "Holosmith";
    case EliteSpecID::Mechanist:
        return "Mechanist";
    case EliteSpecID::Scrapper:
        return "Scrapper";

    // Guardian
    case EliteSpecID::Dragonhunter:
        return "Dragonhunter";
    case EliteSpecID::Firebrand:
        return "Firebrand";
    case EliteSpecID::Luminary:
        return "Luminary";
    case EliteSpecID::Willbender:
        return "Willbender";

    // Mesmer
    case EliteSpecID::Chronomancer:
        return "Chronomancer";
    case EliteSpecID::Mirage:
        return "Mirage";
    case EliteSpecID::Troubadour:
        return "Troubadour";
    case EliteSpecID::Virtuoso:
        return "Virtuoso";

    // Necromancer
    case EliteSpecID::Harbinger:
        return "Harbinger";
    case EliteSpecID::Reaper:
        return "Reaper";
    case EliteSpecID::Ritualist:
        return "Ritualist";
    case EliteSpecID::Scourge:
        return "Scourge";

    // Ranger
    case EliteSpecID::Druid:
        return "Druid";
    case EliteSpecID::Galeshot:
        return "Galeshot";
    case EliteSpecID::Soulbeast:
        return "Soulbeast";
    case EliteSpecID::Untamed:
        return "Untamed";

    // Revenant
    case EliteSpecID::Conduit:
        return "Conduit";
    case EliteSpecID::Herald:
        return "Herald";
    case EliteSpecID::Renegade:
        return "Renegade";
    case EliteSpecID::Vindicator:
        return "Vindicator";

    // Thief
    case EliteSpecID::Antiquary:
        return "Antiquary";
    case EliteSpecID::Daredevil:
        return "Daredevil";
    case EliteSpecID::Deadeye:
        return "Deadeye";
    case EliteSpecID::Specter:
        return "Specter";

    // Warrior
    case EliteSpecID::Berserker:
        return "Berserker";
    case EliteSpecID::Bladesworn:
        return "Bladesworn";
    case EliteSpecID::Paragon:
        return "Paragon";
    case EliteSpecID::Spellbreaker:
        return "Spellbreaker";

    // Default/Unknown
    case EliteSpecID::Unknown:
    default:
        return "Unknown";
    }
}

std::vector<std::string> get_elite_specs_for_profession(ProfessionID profession)
{
    std::vector<std::string> elite_specs;

    switch (profession)
    {
    case ProfessionID::GUARDIAN:
        elite_specs = {"dragonhunter", "firebrand", "willbender", "luminary"};
        break;
    case ProfessionID::WARRIOR:
        elite_specs = {"berserker", "spellbreaker", "bladesworn", "paragon"};
        break;
    case ProfessionID::ENGINEER:
        elite_specs = {"scrapper", "holosmith", "mechanist", "amalgam"};
        break;
    case ProfessionID::RANGER:
        elite_specs = {"druid", "soulbeast", "untamed", "galeshot"};
        break;
    case ProfessionID::THIEF:
        elite_specs = {"daredevil", "deadeye", "specter", "antiquary"};
        break;
    case ProfessionID::ELEMENTALIST:
        elite_specs = {"tempest", "weaver", "catalyst", "evoker"};
        break;
    case ProfessionID::MESMER:
        elite_specs = {"chronomancer", "mirage", "virtuoso", "troubadour"};
        break;
    case ProfessionID::NECROMANCER:
        elite_specs = {"reaper", "scourge", "harbinger", "ritualist"};
        break;
    case ProfessionID::REVENANT:
        elite_specs = {"herald", "renegade", "vindicator", "conduit"};
        break;
    default:
        break;
    }

    return elite_specs;
}
