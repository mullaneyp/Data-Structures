/*
 * CS 261 Assignment 5
 * Name: Patrick Mullaney
 * Date: 3/1/17
 */

#include "dynamicArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define TESTING

#ifndef TESTING
static void adjustHeap(DynamicArray* heap, int last, int position, compareFunction compare);
static void buildHeap(DynamicArray* heap, compareFunction compare);
#endif

struct DynamicArray
{
    TYPE* data;
    int size;
    int capacity;
};

// --- Dynamic array ---

static void setCapacity(DynamicArray* array, int capacity)
{
    TYPE* data = malloc(sizeof(TYPE) * capacity);
    for (int i = 0; i < array->size; i++)
    {
        data[i] = array->data[i];
    }
    free(array->data);
    array->data = data;
    array->capacity = capacity;
}

static void init(DynamicArray* array, int capacity)
{
    assert(capacity > 0);
    array->data = NULL;
    array->size = 0;
    setCapacity(array, capacity);
}

DynamicArray* dyNew(int capacity)
{
    DynamicArray* array = malloc(sizeof(DynamicArray));
    init(array, capacity);
    return array;
}

void dyDelete(DynamicArray* array)
{
    free(array->data);
    free(array);
}

void dyAdd(DynamicArray* array, TYPE value)
{
    if (array->size >= array->capacity)
    {
        setCapacity(array, 2 * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

void dyAddAt(DynamicArray* array, TYPE value, int position)
{
    assert(position <= array->size);
    dyAdd(array, value);
    for (int i = array->size - 1; i > position; i--)
    {
        dySwap(array, i, i - 1);
    }
}

void dyPut(DynamicArray* array, TYPE value, int position)
{
    assert(position < array->size);
    array->data[position] = value;
}

void dyRemoveAt(DynamicArray* array, int position)
{
    assert(position < array->size);
    for (int i = position; i < array->size - 1; i++)
    {
        array->data[position] = array->data[position + 1];
    }
    array->size--;
}

TYPE dyGet(DynamicArray* array, int position)
{
    assert(position < array->size);
    return array->data[position];
}

int dySize(DynamicArray* array)
{
    return array->size;
}

void dySwap(DynamicArray* array, int position1, int position2)
{
    assert(position1 < array->size);
    assert(position2 < array->size);
    TYPE temp = array->data[position1];
    array->data[position1] = array->data[position2];
    array->data[position2] = temp;
}

// --- Stack ---

void dyStackPush(DynamicArray* stack, TYPE value)
{
    dyAdd(stack, value);
}

void dyStackPop(DynamicArray* stack)
{
    dyRemoveAt(stack, stack->size - 1);
}

TYPE dyStackTop(DynamicArray* stack)
{
    return dyGet(stack, stack->size - 1);
}

int dyStackIsEmpty(DynamicArray* stack)
{
    return stack->size == 0;
}

// --- Bag ---

static int findFirst(DynamicArray* array, TYPE value, compareFunction compare)
{
    for (int i = 0; i < array->size; i++)
    {
        if (compare(value, array->data[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void dyBagAdd(DynamicArray* bag, TYPE value)
{
    dyAdd(bag, value);
}

void dyBagRemove(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = findFirst(bag, value, compare);
    if (position != -1)
    {
        dyRemoveAt(bag, position);
    }
}

int dyBagContains(DynamicArray* bag, TYPE value, compareFunction compare)
{
    return findFirst(bag, value, compare) != -1;
}

// --- Ordered bag ---

static int binarySearch(DynamicArray* array, TYPE value, compareFunction compare)
{
    int low = 0;
    int high = array->size - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (compare(value, array->data[middle]) < 0)
        {
            high = middle - 1;
        }
        else if (compare(value, array->data[middle]) > 0)
        {
            low = middle + 1;
        }
        else
        {
            return middle;
        }
    }
    return low;
}

void dyOrderedAdd(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    dyAddAt(bag,value, position);
}

void dyOrderedRemove(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    if (compare(value, bag->data[position]) == 0)
    {
        dyRemoveAt(bag, position);
    }
}

int dyOrderedContains(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    return compare(value, dyGet(bag, position)) == 0;
}

// --- Heap ---

/**
 * Adjusts heap to maintain the heap property.
 * @param heap
 * @param last  index to adjust up to.
 * @param position  index where adjustment starts.
 * @param compare  pointer to compare function.
 */
void adjustHeap(DynamicArray* heap, int last, int position, compareFunction compare)
{
    // FIXME: implement
    
    assert(position >= 0);
    
    int leftChild = 2 * position + 1;
    int rightChild = 2 * position + 2;
    
    if(rightChild < last) /* we have two children */
    {
       /* int leftNum = *(int*)dyGet(heap, leftChild);
        int rightNum = *(int*)dyGet(heap, rightChild);
        printf("left: %d right: %d \n", leftNum, rightNum); For testing */
        
        /* get index of smallest child */
        int comparison = compare(dyGet(heap, leftChild), dyGet(heap,rightChild));
        int smallest;
        
        if(comparison == -1)
        {
            smallest = leftChild;
        }
        else if(comparison == 1)
        {
            smallest = rightChild;
        }
        else
            smallest = leftChild;

        /*int smallChild = *(int*)dyGet(heap, smallest);
        printf("smallest: %d\n", smallChild); */
        
        /* if value at pos > value of smallest child */
        if(compare(dyGet(heap, position), dyGet(heap, smallest)) == 1)
        {
            /* swap with smallest child */
            dySwap(heap, position, smallest);
            /* call adjustHeap (max, index of smallest child) */
            adjustHeap(heap, last, smallest, compare);
            /*** decrement last? **/
        }
    }
    else if(leftChild < last) /* we have one child */
    {
        /* if value at pos > value of child swap with smallest child */
        if(compare(dyGet(heap, position), dyGet(heap, leftChild)) == 1)
        {
            dySwap(heap, position, leftChild);
            /* call adjustHeap (max, index of left child) */
            adjustHeap(heap, last, leftChild, compare);
        }
    }
    /*else no children, done */
        /* printf("No children\n"); for testing  */
}

/**
 * Builds a valid heap from an arbitrary array.
 * @param heap  array with elements in any order.
 * @param compare  pointer to compare function.
 */
void buildHeap(DynamicArray* heap, compareFunction compare)
{
    // FIXME: implement
    /* Used lecture slides as guide */
    
    int max = dySize(heap);
    
    int i = (max/2)-1;
    while(i >= 0)
    {
        adjustHeap(heap, max, i, compare);
        i--;
    }
}

/**
 * Adds an element to the heap.
 * @param heap
 * @param value  value to be added to heap.
 * @param compare  pointer to compare function.
 */
void dyHeapAdd(DynamicArray* heap, TYPE value, compareFunction compare)
{
    // FIXME: implement
    int parent;
    int pos = dySize(heap);
    dyAdd(heap, value); /* Add value to array */
    /* Percolate up */
    while(pos != 0)
    {
        parent = (pos-1)/2;
        /* If parent is greater than new element, swap */
        if(compare(dyGet(heap, pos), dyGet(heap,parent)) == -1)
        {
            dySwap(heap, pos, parent);
            pos = parent;
        }
        else
            return;
    }
}

/**
 * Removes the first element, which has the min priority, from the heap.
 * @param heap
 * @param compare  pointer to the compare function.
 */
void dyHeapRemoveMin(DynamicArray* heap, compareFunction compare)
{
    // FIXME: implement
    assert(dySize(heap) > 0);
    /* Get last element */
    int last = dySize(heap) - 1;
    /* Put at min */
    dyPut(heap, dyGet(heap, last), 0);
    /* Remove last element */  //*** Free min?
    dyRemoveAt(heap, last);
    /* Adjust heap */
    adjustHeap(heap, last, 0, compare);
}

/**
 * Returns the first element, which has the min priority, from the heap.
 * @param heap
 * @return  Element at the top of the heap.
 */
TYPE dyHeapGetMin(DynamicArray* heap)
{
    // FIXME: implement
    assert(dySize(heap) > 0);
    TYPE min;
    
    min = dyGet(heap, 0);
    
    return min;
}

/**
 * Sorts arbitrary array in-place.
 * @param heap  array with elements in arbitrary order.
 * @param compare  pointer to the compare function.
 */
void dyHeapSort(DynamicArray* heap, compareFunction compare)
{
    // FIXME: implement
    /* Used lecture slides as guide */
    assert(dySize(heap)> 0);
    /* 1. Build heap */
    buildHeap(heap, compare);
    int last = dySize(heap)-1;
    while(last > 0)
    {
        /* 2. Swap first and last */
        dySwap(heap, 0, last);
        /* 3. Adjust heap 0 to last */
        adjustHeap(heap, last, 0, compare);
        last--; /* Decrement last and repeat until last == 0. */
    }
}

// --- Iterator ---

DynamicArrayIterator* dyIteratorNew(DynamicArray* array)
{
    DynamicArrayIterator* iterator = malloc(sizeof(DynamicArrayIterator));
    iterator->array = array;
    iterator->current = 0;
    return iterator;
}

void dyIteratorDelete(DynamicArrayIterator* iterator)
{
    free(iterator);
}

int dyIteratorHasNext(DynamicArrayIterator* iterator)
{
    return iterator->current < iterator->array->size;
}

TYPE dyIteratorNext(DynamicArrayIterator* iterator)
{
    TYPE value = dyGet(iterator->array, iterator->current);
    iterator->current++;
    return value;
}

void dyIteratorRemove(DynamicArrayIterator* iterator)
{
    iterator->current--;
    dyRemoveAt(iterator->array, iterator->current);
}

// --- Utility ---

void dyPrint(DynamicArray* array, printFunction print)
{
    printf("\nsize: %d\ncapacity: %d\n[\n", array->size, array->capacity);
    for (int i = 0; i < array->size; i++)
    {
        printf("%d : ", i);
        print(array->data[i]);
        printf("\n");
    }
    printf("]\n");
}

void dyCopy(DynamicArray* source, DynamicArray* destination)
{
    free(destination->data);
    init(destination, source->capacity);
    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }
    destination->size = source->size;
}