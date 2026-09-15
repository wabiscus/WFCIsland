#pragma once
#include <vector>
#include <random>

#include "Ruleset.hpp"

class Grid;

/** @brief Structure representing an influence point for shape generation */
struct InfluencePoint
{
    int x;
    int y;
};

/** @brief Class for generating shapes on the grid */
class ShapeGenerator
{
public:
    /** 
     * @brief Construct a new ShapeGenerator object 
     */
    explicit ShapeGenerator(Grid &grid, Ruleset &ruleset);

    /** 
     * @brief Generate a shape on the grid
     * @param influencePointCount The number of influence points to use
     * @param sharpness The sharpness of the shape
     * @param roundness The roundness of the shape
     */
    void generate(int influencePointCount, float sharpness, float roundness);

    /** 
     * @brief Clear all influence points
     */
    void clearPoints();

    /** 
     * @brief Connect the influence points
     */
    void connectPoints();

    /** 
     * @brief Define the boundary of the shape
     */
    void defineBoundary();

    /** 
     * @brief Fill the area outside the shape with water
     */
    void fillOutsideWithWater();

    /** 
     * @brief Set the seed for the random number generator
     * @param seed The seed to use
     */
    void setSeed(unsigned int seed);

private:
    /** @brief Reference to the grid */
    Grid &m_grid;
    /** @brief Vector of influence points */
    std::vector<InfluencePoint> m_points;
    /** @brief Reference to the ruleset */
    Ruleset &m_ruleset;
    /** @brief Seed for the random number generator */
    unsigned int m_seed;
    /** @brief Random number generator */
    std::mt19937 m_generator;

    /** @brief Sharpness of the shape */
    float m_sharpness = 0.0f;
    /** @brief Roundness of the shape */
    float m_roundness = 1.0f;

    /** @brief Check if a position is inside the shape */
    bool isInside(int x, int y) const;
};
