#pragma once

#include "Grid.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"
#include "App.hpp"

/** @brief Class representing the user interface */
class UI
{
public:
    /**
     * @brief Construct a new UI object
     * @param panel The panel to initialize the UI with
     */
    explicit UI(){};

    /**
     * @brief Render the UI
     * @param app The application instance
     */
    void render(App &app);

    /**
     * @brief Check if the grid is visible
     * @return True if the grid is visible, false otherwise
     */
    bool isGridVisible() const;

    /**
     * @brief Check if the possibilities are visible
     * @return True if the possibilities are visible, false otherwise
     */
    bool arePossibilitiesVisible() const;

private:

    void renderLeftPanel(App &app);

    void renderRightPanel(App &app);

    /** @brief Flag indicating if the grid is visible */
    bool m_showGrid = false;
    /** @brief Flag indicating if the possibilities are visible */
    bool m_showPossibilities = false;

    /** @brief Flag indicating if the seed has been initialized */
    bool m_seedInitialized = false;
    /** @brief The seed for the random number generator */
    unsigned int m_seed = 0;
};