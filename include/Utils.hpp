#pragma once

#include "Grid.hpp"
#include "imgui.h"
#include <string>
#include <stdexcept>

enum class RulesetType
{
    Tropical,
    Desert,
    Forest,
    Volcanic,

    Count
};

enum class GenerationState
{
    Empty,
    Shape,
    BoundariesDefined,
    Generating,
    GeneratingInstantly,
    Generated
};

const char* tileToString(Tile tile);
Tile tileFromString(const std::string& name);

const char* rulesetToString(RulesetType type);
RulesetType rulesetFromString(const std::string& name);

ImU32 tileToColor(Tile tile);

const char *stateToString(GenerationState state);