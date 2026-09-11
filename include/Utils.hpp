#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"
#include "imgui.h"
#include "App.hpp"
#include <string>

const char* tileToString(Tile tile);
Tile tileFromString(const std::string& name);

const char* rulesetToString(RulesetType type);
RulesetType rulesetFromString(const std::string& name);

ImU32 tileToColor(Tile tile);

const char *stateToString(GenerationState state);