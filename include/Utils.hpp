#pragma once

#include "Grid.hpp"
#include "imgui.h"
#include <string>
#include <stdexcept>

/**
 * @brief Enum representing the different types of rulesets available
 */
enum class RulesetType
{
    Tropical,
    Desert,
    Forest,
    Volcanic,

    Count
};

/**
 * @brief Enum representing the different states of the generation process
 */
enum class GenerationState
{
    Empty,
    Shape,
    BoundariesDefined,
    Generating,
    GeneratingInstantly,
    Generated
};

/**
 * @brief Convert a tile to a string representation
 * @param tile The tile to convert
 * @return The string representation of the tile
 */
const char* tileToString(Tile tile);
/**
 * @brief Convert a string to a tile
 * @param name The string representation of the tile
 * @return The corresponding tile
 */
Tile tileFromString(const std::string& name);

/**
 * @brief Convert a ruleset type to a string representation
 * @param type The ruleset type to convert
 * @return The string representation of the ruleset type
 */
const char* rulesetToString(RulesetType type);
/**
 * @brief Convert a string to a ruleset type
 * @param name The string representation of the ruleset type
 * @return The corresponding ruleset type
 */
RulesetType rulesetFromString(const std::string& name);

/**
 * @brief Convert a tile to a color
 * @param tile The tile to convert
 * @return The color representing the tile
 */
ImU32 tileToColor(Tile tile);

/**
 * @brief Convert a generation state to a string representation
 * @param state The generation state to convert
 * @return The string representation of the generation state
 */
const char *stateToString(GenerationState state);