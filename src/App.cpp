#include "App.hpp"

App::App(
    Grid &grid,
    Ruleset &ruleset,
    ShapeGenerator &shape,
    WFC &wfc,
    Renderer &renderer)
    : m_grid(grid),
      m_ruleset(ruleset),
      m_shapeGenerator(shape),
      m_wfc(wfc),
      m_renderer(renderer)
{
    setupFSM();
}

void App::setupFSM()
{
    // ---------------------------------------------------------
    // Transitions
    // ---------------------------------------------------------

    m_fsm.addTransition(
        GenerationState::Empty,
        GenerationEvent::GenerateShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::Shape,
        GenerationEvent::GenerateShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::Shape,
        GenerationEvent::DefineBoundary,
        GenerationState::BoundariesDefined);

    m_fsm.addTransition(
        GenerationState::BoundariesDefined,
        GenerationEvent::GenerateShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::BoundariesDefined,
        GenerationEvent::DefineBoundary,
        GenerationState::BoundariesDefined);

    m_fsm.addTransition(
        GenerationState::BoundariesDefined,
        GenerationEvent::GenerateStepByStep,
        GenerationState::Generating);

    m_fsm.addTransition(
        GenerationState::Generated,
        GenerationEvent::GenerateShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::Generated,
        GenerationEvent::RestoreShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::Generated,
        GenerationEvent::RestoreBoundaries,
        GenerationState::BoundariesDefined);

    m_fsm.addTransition(
        GenerationState::BoundariesDefined,
        GenerationEvent::RestoreShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::BoundariesDefined,
        GenerationEvent::GenerateIsland,
        GenerationState::GeneratingInstantly);

    m_fsm.addTransition(
        GenerationState::Generating,
        GenerationEvent::RestoreShape,
        GenerationState::Shape);

    m_fsm.addTransition(
        GenerationState::Generating,
        GenerationEvent::RestoreBoundaries,
        GenerationState::BoundariesDefined);

    // ---------------------------------------------------------
    // Enter Shape
    // ---------------------------------------------------------

    m_fsm.onEnter(
        GenerationState::Shape,
        [this]()
        {
            m_wfc.regenerateMap();
            m_shapeGenerator.generate(m_influencePointCount, m_sharpness, m_roundness);
            m_shapeGenerator.connectPoints();
            m_shapeGenerator.fillOutsideWithWater();
            m_grid.updateUnknownCells();
            m_grid.saveState();
            m_grid.saveShape();
        });

    // ---------------------------------------------------------
    // Enter BoundariesDefined
    // ---------------------------------------------------------

    m_fsm.onEnter(
        GenerationState::BoundariesDefined,
        [this]()
        {
            m_grid.restoreShape();
            m_shapeGenerator.fillOutsideWithWater();
            m_shapeGenerator.defineBoundary();
            m_wfc.resetPossibilities();
            m_grid.updateUnknownCells();
            m_grid.saveState();
        });

    // ---------------------------------------------------------
    // Enter Generating
    // ---------------------------------------------------------

    m_fsm.onEnter(
        GenerationState::Generating,
        [this]()
        {
            m_generationRunning = true;
        });

    // ---------------------------------------------------------
    // Enter GeneratingInstantly
    // ---------------------------------------------------------

    m_fsm.onEnter(
        GenerationState::GeneratingInstantly,
        [this]()
        {
            m_wfc.generateIsland();

            if (!m_wfc.hasContradiction())
            {
                m_generationRunning = false;
                m_fsm.transitionTo(GenerationState::Generated);
            }
        });

    // ---------------------------------------------------------
    // Enter Generated
    // ---------------------------------------------------------

    m_fsm.onEnter(
        GenerationState::Generated,
        [this]()
        {
            m_grid.saveState();
        });
}

void App::handleEvent(GenerationEvent event)
{
    m_fsm.handleEvent(event);
}

void App::update()
{
    if (!m_fsm.is(GenerationState::Generating))
        return;

    if (!m_generationRunning)
        return;

    m_wfc.generateStep();

    if (m_wfc.hasContradiction())
        return;

    if (m_wfc.isFinished())
    {
        m_generationRunning = false;
        m_fsm.transitionTo(GenerationState::Generated);
    }
}

void App::render(int offsetX, int offsetY, bool showGrid, bool showPossibilities)
{
    m_renderer.render(m_grid, offsetX, offsetY, showGrid, showPossibilities);
}

int App::getWidth() const
{
    return m_grid.getWidth();
}

int App::getHeight() const
{
    return m_grid.getHeight();
}

Cell &App::getCell(int x, int y)
{
    return m_grid.get(x, y);
}

//// WFC interface

void App::generateOneStep()
{
    m_wfc.propagateUntilStable();
    m_wfc.collapse();
}

void App::toggleGeneration()
{
    m_generationRunning = !m_generationRunning;
}

bool App::isGeneratingRunning() const
{
    return m_generationRunning;
}

void App::setSeed(unsigned int seed)
{
    m_seed = seed;

    m_seedGenerator.seed(m_seed);
    m_wfc.setSeed(m_seedGenerator());
    m_shapeGenerator.setSeed(m_seedGenerator());
}

unsigned int App::getSeed() const
{
    return m_seed;
}

void App::randomizeSeed()
{
    setSeed(std::random_device{}());
}

void App::generateIsland()
{
    m_wfc.generateIsland();
}

void App::generateStepbyStep()
{
    m_wfc.generateStep();
}

GenerationState App::getGenerationState() const
{
    return m_fsm.getState();
}

int &App::getInfluencePointsCount()
{
    return m_influencePointCount;
}

bool App::exportIslandPNG()
{
    constexpr int exportScale = 8;

    std::vector<Uint8> pngData =
        m_renderer.exportImage(
            m_grid,
            exportScale);

    if (pngData.empty())
        return false;

    const std::string filename =
        "island_" +
        std::to_string(m_seed) +
        ".png";

    return m_fileExporter.save(
        pngData,
        filename);
}

bool App::exportIslandJson()
{
    const std::string filename =
        "island_" +
        std::to_string(m_seed) +
        ".json";

    return m_islandExporter.exportJson(
        m_grid,
        m_seed,
        m_ruleset.getName(),
        filename);
}

//// Ruleset interface
std::vector<Tile> App::getAllowedNeighbors(Tile tile) const
{
    return m_ruleset.getAllowedNeighbors(tile);
}

const std::vector<Tile> &App::getTiles() const
{
    return m_ruleset.getTiles();
}

const std::string &App::getName() const
{
    return m_ruleset.getName();
}

const RulesetType App::getType() const
{
    return m_ruleset.getType();
}

const std::map<std::vector<Tile>, std::vector<int>> &App::getWeights() const
{
    return m_ruleset.getWeights();
}

std::vector<int> &App::getWeights(const std::vector<Tile> &possibilities)
{
    return m_ruleset.getWeights(possibilities);
}

////

const Scope &App::getScope() const
{
    return m_grid.getScope();
}

const ScopeSize &App::getScopeSize() const
{
    return m_grid.getScopeSize();
}

void App::setScopeSize(ScopeSize scope)
{
    m_grid.setScope(scope);
}

float &App::getSharpness()
{
    return m_sharpness;
}

float &App::getRoundness()
{
    return m_roundness;
}

void App::switchRuleset(RulesetType type)
{
    m_wfc.setRuleset(type);
}

void App::reportBug()
{
    constexpr const char *bugReportURL =
        "https://github.com/wabiscus/WFCIsland/issues/new?template=bug_report.yml";

    if (!openURL(bugReportURL))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to open bug report URL: %s",
                     SDL_GetError());
    }
}