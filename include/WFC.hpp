#pragma once

class Grid;

class WFC
{
public:
    explicit WFC(Grid& grid);

    void generate();

    void reset();

private:
    Grid& m_grid;
};