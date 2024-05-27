#pragma once
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

void swap(int *a, int *b);

void heapify(int array[], int size, int i);

void insert(int array[], int newNum);

void deleteRoot(int array[], int num);

void printArray(int array[], int size);

#endif