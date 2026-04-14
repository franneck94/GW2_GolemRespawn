#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Types.h"

ProfessionID string_to_profession(const std::string &profession_name, const std::string &filename);

EliteSpecID string_to_elite_spec(const std::string &spec_name, const std::string &filename);

std::string profession_to_string(ProfessionID profession_id);

std::string elite_spec_to_string(EliteSpecID elite_spec_id);

std::vector<std::string> get_elite_specs_for_profession(ProfessionID profession);
