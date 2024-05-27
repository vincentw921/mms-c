#include <stdio.h>

#include "API.h"
#include "Search.h"
#include "Queue.h"

int main(int argc, char* argv[]) {
    struct Maze maze;
    initialize_maze(maze);
    
    while (1) {
        scanWalls(maze);
        floodfill(maze);
        updateMazeVisuals(maze);

        struct Cell next = getBestCell(maze);

        rotate(&maze, next.dir);
        move(&maze);
    }
}
