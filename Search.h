#pragma once
#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdbool.h>
#include <cstdlib>
#include <math.h>

#include "Queue.h"
#include "API.h"

#define MAX_COST 100000;

struct Coord {
	int x;
	int y;
};

enum Direction {
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3
};

struct Cell {
    struct Coord pos;
    int dir;
    bool blocked;
};

struct Maze {
    struct Coord mousePos;
    int mouseDir;
    int distances[16][16];
    bool exploredCells[16][16];
    int cellWalls[16][16];
    struct Coord* goalPos;
};

struct Cell* getNeighborCells(struct Maze maze, struct Coord currentPos);
struct Cell getBestCell(struct Maze maze);
void clockwiseStep(struct Maze maze);
void counterClockWiseStep(struct Maze maze);

void setGoalCell(struct Maze* maze, struct Coord* goalPos);

void rotate(struct Maze* maze, int direction);
void move(struct Maze* maze);

bool in_maze(Coord pos);
void scanWalls(struct Maze& maze);

void floodfill(struct Maze& maze);
void initialize_maze(struct Maze& maze);

void updateMazeVisuals(struct Maze& maze);
void log_text(char* text);


#endif