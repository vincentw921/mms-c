#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include "Search.h"

struct Queue {
    int front, rear, size;
    unsigned capacity;
    struct Coord* array;
};

struct Queue* createQueue(unsigned capacity);

int isFull(struct Queue* queue);

int isEmpty(struct Queue* queue);

void enqueue(struct Queue* queue, struct Coord);

struct Coord dequeue(struct Queue* queue);

struct Coord front(struct Queue* queue);

struct Coord rear(struct Queue* queue);

#endif