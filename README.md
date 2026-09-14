# WFCIsland

# WFC Island

A procedural island generator written in **C++20**, using **Wave Function Collapse (WFC)** and **SDL3**.

The project generates island shapes, defines their boundaries, and procedurally fills the terrain using a tile-based Wave Function Collapse algorithm.

## Features

* Procedural island shape generation
* Adjustable island parameters:

  * Number of points
  * Sharpness
  * Roundness
  * Island scope
* Procedural boundary definition
* Tile-based Wave Function Collapse
* Step-by-step generation
* Instant island generation
* Pause / resume during step-by-step generation
* Configurable tile weights and adjacency rules
* Multiple rulesets
* Random seed control
* Island PNG export
* Island JSON export
* ImGui-based debug and configuration interface

## How it works

The generation process is divided into several stages.

### 1. Island shape

The island outline is generated procedurally from a set of influence points.

The shape can be modified using parameters such as:

* **Points** — number of points used to define the island
* **Sharpness** — controls angular variation in the outline
* **Roundness** — controls radial variation
* **Scope** — controls the area occupied by the island

The generated shape can be regenerated without running the WFC algorithm.

### 2. Boundary definition

Once the island shape has been generated, its boundary is defined separately.

The boundary is based on the adjacency rules of the selected ruleset. This allows the same island shape to be kept while experimenting with different boundary configurations.

The boundary can also be refined and regenerated independently from the island shape.

### 3. Wave Function Collapse

After the boundary has been defined, the WFC algorithm generates the terrain.

Each unresolved cell contains a set of possible tiles. The algorithm repeatedly:

1. Selects an unresolved cell with low entropy.
2. Collapses the cell to a tile.
3. Propagates the resulting constraints to neighboring cells.
4. Repeats until the grid is completely resolved or a contradiction occurs.

The current implementation stops when a contradiction is detected rather than performing automatic backtracking.

## Tile system

The grid currently supports the following tile types:

* Water
* Sand
* Grass
* Forest
* Rock
* Snow
* Lava

The grid also contains internal states used during generation:

* Unknown
* Boundary
* Contradiction

These states are not part of the terrain ruleset.

## Rulesets

The terrain generation is driven by configurable rulesets.

A ruleset defines:

* Available tiles
* Tile weights
* Allowed neighboring tiles

The UI allows the rules and tile weights to be inspected and modified while experimenting with the generator.

Current rulesets include:

* Tropical
* Desert
* Forest
* Volcanic

## Generation states

The application uses a small finite state machine to control the generation workflow:

```text
Empty
  ↓
Shape
  ↓
BoundariesDefined
  ↓
Generating
  ↓
Generated
```

An instant generation mode is also available for generating the complete island without the step-by-step animation.

The state machine is responsible for keeping the different generation stages separate and preventing invalid operations from being triggered from the UI.

## Controls

The application provides an ImGui interface for interacting with the generator.

The interface currently provides controls for:

* Generating an island shape
* Setting the random seed
* Changing shape parameters
* Selecting the island scope
* Defining and refining the boundary
* Generating one WFC step
* Generating the complete island
* Generating step by step
* Pausing and resuming generation
* Selecting a ruleset
* Editing tile weights
* Displaying tile possibilities and entropy
* Exporting the generated island

## Project structure

The project is organized around a few main responsibilities:

```text
App
├── Grid
├── WFC
├── ShapeGenerator
├── Ruleset
├── Renderer
└── FSM

UI
└── ImGui interface
```

`App` acts as the main façade of the application and coordinates the different systems.

`Grid` stores the generated map and cell state.

`ShapeGenerator` is responsible for generating the island shape and its boundary.

`WFC` implements the Wave Function Collapse generation process.

`Ruleset` contains the terrain rules and tile weights.

`Renderer` handles SDL3 rendering and image generation.

`UI` contains the ImGui interface and communicates with the application through `App`.

## Technologies

* **C++20**
* **SDL3**
* **Dear ImGui**
* **CMake**
* **nlohmann/json**

SDL3 and the other dependencies are integrated through CMake.

## Building

### Requirements

* A C++20 compatible compiler
* CMake
* Git

### Build

Clone the repository:

```bash
git clone https://github.com/wabiscus/WFCIsland.git
cd WFCIsland
```

Configure and build the project:

```bash
cmake -S . -B build
cmake --build build
```

The project uses CMake to fetch and configure its external dependencies.

## Export

The generated island can be exported in two formats:

### PNG

Exports the rendered island as an image.

### JSON

Exports the island data, including:

* Generator version
* Random seed
* Ruleset
* Grid dimensions
* Tile state of each cell

The JSON format is intended to make generated islands reproducible and usable by other tools in the future.

## Current limitations

This project is still under development.

Current limitations include:

* No automatic WFC backtracking yet
* The generator stops when a contradiction occurs
* The current application is primarily designed as a desktop application
* The ruleset system is still being expanded

## Roadmap

Possible future improvements:

* [ ] Automatic contradiction recovery / backtracking
* [ ] More rulesets and tile types
* [ ] Improved island shape generation
* [ ] WebAssembly build
* [ ] Browser-based version
* [ ] Better export/import tooling
* [ ] More rendering options
* [ ] Performance improvements
* [ ] Additional generation controls

## Why this project?

This project is mainly an exploration of procedural generation and software architecture in C++.

The goal is to experiment with:

* Wave Function Collapse
* Procedural geometry
* Constraint propagation
* Finite state machines
* Modular C++ architecture
* SDL3
* ImGui
* Reproducible procedural generation through seeds

## License

MIT License

Copyright (c) 2026 Omar Wabbi - Wabiscus