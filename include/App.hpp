#pragma once

#include "FSM.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"
#include "Utils.hpp"
#include "Renderer.hpp"
#include "FileExporter.hpp"
#include "IslandExporter.hpp"

/** @brief Enum class representing the events that can trigger state transitions in the generation process */
enum class GenerationEvent
{
    GenerateShape,
    DefineBoundary,
    GenerateStepByStep,
    RestoreShape,
    RestoreBoundaries,
    GenerateIsland
};

/** @brief Class representing the main application */
class App
{
public:
    /** @brief Constructor for the App class
     * @param grid The grid to use for generation
     * @param ruleset The ruleset to use for generation
     * @param shape The shape generator to use
     * @param wfc The WFC instance to use
     * @param renderer The renderer to use
     */
    App(Grid &grid, Ruleset &ruleset, ShapeGenerator &shape, WFC &wfc, Renderer &renderer);

    /** @brief Handle a generation event
     * @param event The event to handle
     */
    void handleEvent(GenerationEvent event);

    /** @brief Get the current generation state
     * @return The current generation state
     */
    GenerationState getGenerationState() const;

    /** @brief Update the application state */
    void update();
    /** @brief Render the application
     * @param offsetX The x offset for rendering
     * @param offsetY The y offset for rendering
     * @param showGrid Whether to show the grid
     * @param showPossibilities Whether to show the possibilities
     */
    void render(int offsetX, int offsetY, bool showGrid, bool showPossibilities);

    //// Grid interface
    /** @brief Get the width of the grid
     * @return The width of the grid
     */
    int getWidth() const;
    /** @brief Get the height of the grid
     * @return The height of the grid
     */
    int getHeight() const;

    /** @brief Get a cell from the grid
     * @param x The x coordinate of the cell
     * @param y The y coordinate of the cell
     * @return A reference to the cell
     */
    Cell &getCell(int x, int y);

    ////
    /** @brief Get the count of influence points
     * @return A reference to the influence point count
     */
    int &getInfluencePointsCount();

    ////
    /** @brief Export the island as a PNG file
     * @return True if the export was successful, false otherwise
     */
    bool exportIslandPNG();
    /** @brief Export the island as a JSON file
     * @return True if the export was successful, false otherwise
     */
    bool exportIslandJson();

    //// WFC interface

    /** @brief Generate one step of the island
     */
    void generateOneStep();
    /** @brief Generate the entire island
     */
    void generateIsland();
    /** @brief Generate the island step by step
     */
    void generateStepbyStep();
    /** @brief Toggle the generation state
     */
    void toggleGeneration();
    /** @brief Check if generation is running
     * @return True if generation is running, false otherwise
     */
    bool isGeneratingRunning() const;
    /** @brief Set the seed for random number generation
     * @param seed The seed to set
     */
    void setSeed(unsigned int seed);
    /** @brief Get the seed for random number generation
     * @return The seed
     */
    unsigned int getSeed() const;
    /** @brief Randomize the seed for random number generation
     */
    void randomizeSeed();

    //// Ruleset interface
    /** @brief Get the allowed neighbors for a given tile
     * @param tile The tile for which to get allowed neighbors
     * @return A vector of allowed neighbor tiles
     */
    std::vector<Tile> getAllowedNeighbors(Tile tile) const;
    /** @brief Get the tiles in the ruleset
     * @return A constant reference to the vector of tiles
     */
    const std::vector<Tile> &getTiles() const;
    /** @brief Get the name of the ruleset
     * @return A constant reference to the ruleset name
     */
    const std::string &getName() const;
    /** @brief Get the type of the ruleset
     * @return The ruleset type
     */
    const RulesetType getType() const;
    /** @brief Get the weights for the ruleset
     * @return A constant reference to the weights map
     */
    const std::map<std::vector<Tile>, std::vector<int>> &getWeights() const;
    /** @brief Get the weights for a specific set of possibilities
     * @param possibilities The set of possibilities for which to get weights
     * @return A reference to the vector of weights
     */
    std::vector<int> &getWeights(const std::vector<Tile> &possibilities);
    /** @brief Get the scope size
     * @return A constant reference to the scope size
     */
    const ScopeSize &getScopeSize() const;
    /** @brief Get the scope
     * @return A constant reference to the scope
     */
    const Scope &getScope() const;
    /** @brief Set the scope size
     * @param scope The scope size to set
     */
    void setScopeSize(ScopeSize scope);

    /** @brief Get the sharpness
     * @return A reference to the sharpness
     */
    float &getSharpness();
    /** @brief Get the roundness
     * @return A reference to the roundness
     */
    float &getRoundness();

    /** @brief Switch the ruleset
     * @param type The ruleset type to switch to
     */
    void switchRuleset(RulesetType type);

    void reportBug();

private:
    /** @brief Setup the finite state machine for generation */
    void setupFSM();
    
    /** @brief The grid for generation */
    Grid &m_grid;
    /** @brief The ruleset for generation */
    Ruleset &m_ruleset;
    /** @brief The shape generator for generation */
    ShapeGenerator &m_shapeGenerator;
    /** @brief The WFC for generation */
    WFC &m_wfc;
    /** @brief The renderer for generation */   
    Renderer &m_renderer;
    /** @brief The seed for random number generation */
    unsigned int m_seed;
    /** @brief The random number generator for randomization */
    std::mt19937 m_seedGenerator;
    /** @brief The file exporter for exporting files */
    FileExporter m_fileExporter;
    /** @brief The island exporter for exporting islands */
    IslandExporter m_islandExporter;

    /** @brief The finite state machine for generation */
    FSM<GenerationState, GenerationEvent> m_fsm{GenerationState::Empty};

    /** @brief The number of influence points */
    int m_influencePointCount = 6;
    /** @brief The sharpness of the generated shapes */
    float m_sharpness = 2.5f;
    /** @brief The roundness of the generated shapes */
    float m_roundness = 0.0f;
    /** @brief A flag indicating whether generation is running */
    bool m_generationRunning = false;
};