#include "search.h"

#include "API.h"

bool reverse;
struct Coord goalPos;

struct Cell* getNeighborCells(struct Maze maze, struct Coord currentPos) {
    struct Cell* cells = (struct Cell*)malloc(4 * sizeof(struct Cell));
    int walls = maze.cellWalls[currentPos.x][currentPos.y];

    // East Cell
    struct Coord east_pos = {currentPos.x + 1, currentPos.y};
    struct Cell east_cell = {east_pos, EAST, (walls & (1 << EAST))};
    cells[EAST] = east_cell;

    // South Cell
    struct Coord south_pos = {currentPos.x, currentPos.y - 1};
    struct Cell south_cell = {south_pos, SOUTH, (walls & (1 << SOUTH))};
    cells[SOUTH] = south_cell;

    // West Cell
    struct Coord west_pos = {currentPos.x - 1, currentPos.y};
    struct Cell west_cell = {west_pos, WEST, (walls & (1 << WEST))};
    cells[WEST] = west_cell;

    // Check North Wall
    struct Coord north_pos = {currentPos.x, currentPos.y + 1};
    struct Cell north_cell = {north_pos, NORTH, (walls & (1 << NORTH))};
    cells[NORTH] = north_cell;

    return cells;
}

struct Cell getBestCell(struct Maze maze) {
    struct Cell bestCell;
    int bestDist = MAX_COST;
    struct Cell* neighbors = getNeighborCells(maze, maze.mousePos);
    for (int i = 0; i < 4; i++) {
        struct Cell neighbor = neighbors[i];
        if (neighbor.blocked || neighbor.pos.x < 0 || neighbor.pos.y < 0 || neighbor.pos.x >= 16 || neighbor.pos.y >= 16) continue;
        int dist = maze.distances[neighbor.pos.x][neighbor.pos.y];
        if (dist <= bestDist) {
            bestDist = dist;
            bestCell = neighbor;
        }
    }
    return bestCell;
}

void clockwiseStep(struct Maze* maze) {
    maze->mouseDir = (maze->mouseDir + 3) % 4;
}

void counterClockWiseStep(struct Maze* maze) {
    maze->mouseDir = (maze->mouseDir + 1) % 4;
}

void setGoalCell(struct Maze* maze, struct Coord* goalPos) {
    maze->goalPos = goalPos;
}

void rotate(struct Maze* maze, int direction) {
    int curDir = maze->mouseDir;

    if (direction == curDir) {
        // do nothing
    }
    else if (direction == (curDir + 3) % 4) {
        API_turnRight();
        clockwiseStep(maze);
    } else if (direction == (curDir + 1) % 4) {
        API_turnLeft();
        counterClockWiseStep(maze);
    } else {
        API_turnRight();
        clockwiseStep(maze);
        API_turnRight();
        clockwiseStep(maze);
        // turn 180
    }
}

struct Coord getMoveCell(struct Maze* maze) {
    int dir = maze->mouseDir;
    if (dir == NORTH) {
        struct Coord newPos = {maze->mousePos.x, maze->mousePos.y + 1};
        return newPos;
    } else if (dir == WEST) {
        struct Coord newPos = {maze->mousePos.x - 1, maze->mousePos.y};
        return newPos;
    } else if (dir == SOUTH) {
        struct Coord newPos = {maze->mousePos.x, maze->mousePos.y - 1};
        return newPos;
    } else {
        struct Coord newPos = {maze->mousePos.x + 1, maze->mousePos.y};
        return newPos;
    }
}

void move(struct Maze* maze) {
    API_moveForward();
    struct Coord newPos = getMoveCell(maze);
    maze->mousePos = newPos;
}

void scanWalls(struct Maze& maze) {
    int dir = maze.mouseDir;

    if (API_wallFront()) {
        maze.cellWalls[maze.mousePos.x][maze.mousePos.y] |= 1 << dir;
        switch(dir) {
            case 0:
                maze.cellWalls[maze.mousePos.x][maze.mousePos.y + 1] |= 1 << ((dir + 2) % 4);
                break;
            case 1:
                maze.cellWalls[maze.mousePos.x - 1][maze.mousePos.y] |= 1 << ((dir + 2) % 4);
                break;
            case 2:
                maze.cellWalls[maze.mousePos.x][maze.mousePos.y - 1] |= 1 << ((dir + 2) % 4);
                break;
            case 3:
                maze.cellWalls[maze.mousePos.x + 1][maze.mousePos.y] |= 1 << ((dir + 2) % 4);
                break;
        }
    }
    int left = ((dir + 1) % 4);
    if (API_wallLeft()) {
        maze.cellWalls[maze.mousePos.x][maze.mousePos.y] |= 1 << left;
        switch(left) {
            case 0:
                maze.cellWalls[maze.mousePos.x][maze.mousePos.y + 1] |= 1 << ((left + 2) % 4);
                break;
            case 1:
                maze.cellWalls[maze.mousePos.x - 1][maze.mousePos.y] |= 1 << ((left + 2) % 4);
                break;
            case 2:
                maze.cellWalls[maze.mousePos.x][maze.mousePos.y - 1] |= 1 << ((left + 2) % 4);
                break;
            case 3:
                maze.cellWalls[maze.mousePos.x + 1][maze.mousePos.y] |= 1 << ((left + 2) % 4);
                break;
        }
    }
    int right = ((dir + 3) % 4);
    if (API_wallRight()) {
        maze.cellWalls[maze.mousePos.x][maze.mousePos.y] |= 1 << right;
        switch(right) {
            case 0:
                maze.cellWalls[maze.mousePos.x][maze.mousePos.y + 1] |= 1 << ((right + 2) % 4);
                break;
            case 1:
                maze.cellWalls[maze.mousePos.x - 1][maze.mousePos.y] |= 1 << ((right + 2) % 4);
                break;
            case 2:
                maze.cellWalls[maze.mousePos.x][maze.mousePos.y - 1] |= 1 << ((right + 2) % 4);
                break;
            case 3:
                maze.cellWalls[maze.mousePos.x + 1][maze.mousePos.y] |= 1 << ((right + 2) % 4);
                break;
        }
    }
}

void updateMazeVisuals(struct Maze& maze) {
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            char str[80];
            sprintf(str, "%d", maze.distances[x][y]);
            API_setText(x, y, str);

            int walls = maze.cellWalls[x][y];

            if (walls & (1 << NORTH)) {
                API_setWall(x, y, 'n');
            } if (walls & (1 << WEST)) {
                API_setWall(x, y, 'w');
            } if (walls & (1 << SOUTH)) {
                API_setWall(x, y, 's');
            } if (walls & (1 << EAST)) {
                API_setWall(x, y, 'e');
            }
        }
    }
}

bool in_maze(Coord pos) {
    return (pos.x < 16 && pos.x >= 0 && pos.y < 16 && pos.y >= 0);
}

void initialize_maze(struct Maze& maze) {
    struct Coord mousePos = {0, 0};
    maze.mousePos = mousePos;
    maze.mouseDir = 0;

    reverse = false;
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            maze.cellWalls[x][y] = 0;
            maze.distances[x][y] = 0;
        }
    }

    for (int i = 0; i < 16; i++) {
        maze.cellWalls[i][0] |= 1 << SOUTH;
        maze.cellWalls[i][15] |= 1 << NORTH;
    }

    for (int j = 0; j < 16; j++) {
        maze.cellWalls[0][j] |= 1 << WEST;
        maze.cellWalls[15][j] |= 1 << EAST;
    }
    updateMazeVisuals(maze);
    
}

void floodfill(struct Maze& maze) {

    struct Queue* queue = createQueue(225);

    if (maze.mousePos.x == goalPos.x && maze.mousePos.y == goalPos.y) {
        reverse = !reverse;
    }

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            maze.distances[i][j] = MAX_COST;
        }
    }

    if (!reverse) {
        for (int i = 7; i <= 8; i++) {
            for (int j = 7; j <= 8; j++) {
                maze.distances[i][j] = 0;
                struct Coord pos = {i, j};
                goalPos = pos;
                enqueue(queue, pos);
            }
        }
    } else {
        maze.distances[0][0] = 0;
        struct Coord pos = {0, 0};
        goalPos = pos;
        enqueue(queue, pos);
    }

    while (!isEmpty(queue)) {
        struct Coord curPos = dequeue(queue);

        int newCost = maze.distances[curPos.x][curPos.y] + 1;

        struct Cell* neighborCells = getNeighborCells(maze, curPos);

        for (int i = 0; i < 4; i++) {
            struct Cell cell = neighborCells[i];
            if (!cell.blocked && in_maze(cell.pos)) {
                if (maze.distances[cell.pos.x][cell.pos.y] > newCost) {
                    maze.distances[cell.pos.x][cell.pos.y] = newCost;
                    enqueue(queue, cell.pos);
                }
            }
        }

        free(neighborCells);
    }
    free(queue);
}