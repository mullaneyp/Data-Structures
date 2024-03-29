/*
 * CS 261 Assignment 5
 * Name: Patrick Mullaney
 * Date: 3/1/17
 */

#include "dynamicArray.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Loads into heap a list from a file with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listLoad(DynamicArray* heap, FILE* file)
{
    const int FORMAT_LENGTH = 256;
    char format[FORMAT_LENGTH];
    snprintf(format, FORMAT_LENGTH, "%%d, %%%d[^\n]", TASK_NAME_SIZE);
    
    Task temp;
    while (fscanf(file, format, &temp.priority, &temp.name) == 2)
    {
        dyHeapAdd(heap, taskNew(temp.priority, temp.name), taskCompare);
    }
}

/**
 * Writes to a file all tasks in heap with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listSave(DynamicArray* heap, FILE* file)
{
    for (int i = 0; i < dySize(heap); i++)
    {
        Task* task = dyGet(heap, i);
        fprintf(file, "%d, %s\n", task->priority, task->name);
    }
}

/**
 * Prints every task in heap.
 * @param heap
 */
void listPrint(DynamicArray* heap)
{
    DynamicArray* temp = dyNew(1);
    dyCopy(heap, temp);
     while (dySize(temp) > 0)
    {
        Task* task = dyHeapGetMin(temp);
        printf("\n");
        taskPrint(task);
        printf("\n");
        dyHeapRemoveMin(temp, taskCompare);
    }
    dyDelete(temp);
}

/**
 * Handles the given command.
 * @param list
 * @param command
 */
void handleCommand(DynamicArray* list, char command)
{
    // FIXME: Implement
    /* assert(list!=0); */
    FILE* fileName;
    
    if(command == 'l') /* load from file */
    {
        printf("Please enter file name: ");
        char name[256];
        scanf("%s", name);
        
        if((fileName = fopen(name, "r")))
        {
            listLoad(list, fileName);
            fclose(fileName);
        }
        else
            printf("File does not exist.\n");
    }
    else if(command == 's') /* save to file */
    {
        printf("Please enter file name: ");
        char name[256];
        scanf("%s", name);
   
        FILE* toDoList;
        toDoList = fopen(name, "w");
        
        listSave(list, toDoList);
        fclose(toDoList);
        printf("Your list has been saved as %s.\n", name);
    }
    else if(command == 'a') /* add a new task */
    {
        printf("Please enter the task description: ");
        char name[256];
        scanf("%s", name);
        
        int priority;
        
        do
        {
            printf("Please enter a priority (0-999): ");
            scanf("%d", &priority);
            if((priority < 0) || (priority > 999))
            {
                printf("Invalid priority number.\n");
            }
        }while((priority < 0) || (priority > 999));
        
        
        Task* newTask = taskNew(priority, name);
        dyHeapAdd(list, newTask, taskCompare);
        printf("The task '%s' has been added to your to-do list.\n", name);
        
    }
    else if(command == 'g') /* get first new task */
    {
        if(dySize(list) > 0)
        {
            Task* min = dyHeapGetMin(list);
            printf("Your first task is: %s.\n", min->name);
        }
        else if(dySize(list) == 0)
        {
            printf("List is empty.\n");
        }
    }
    else if(command == 'r') /* remove the first task */
    {
        if(dySize(list) > 0)
        {
            Task* temp = dyHeapGetMin(list); /* Added to resolve memory leak. */
            dyHeapRemoveMin(list, taskCompare);
            taskDelete(temp); /* Added to resolve memory leak. */
            printf("Task has been removed.\n");
        }
        else if(dySize(list) == 0)
        {
            printf("List is empty, no tasks to remove.\n");
        }
    }
    else if(command == 'p') /* Print the list */
    {
        if(dySize(list) > 0)
        {
            listPrint(list);
        }
        else
            printf("List is empty.\n");
    }
}

int main()
{
    // Implement
    printf("\n\n** TO-DO LIST APPLICATION **\n\n");
    DynamicArray* list = dyNew(8);
    char command = ' ';
    do
    {
        printf("Press:\n"
               "'l' to load to-do list from a file\n"
               "'s' to save to-do list to a file\n"
               "'a' to add a new task\n"
               "'g' to get the first task\n"
               "'r' to remove the first task\n"
               "'p' to print the list\n"
               "'e' to exit the program\n"
        );
        command = getchar();
        // Eat newlines
       /* while (getchar() != '\n'); Original code */
        while(command == '\n')  /* Altered code as code was requiring char input 2x */
        {
                command = getchar();
        }
        handleCommand(list, command);
    }
    while (command != 'e');
 /* free dynamically allocated List pointers in array to avoid memory leaks */
         /* Fix it */
    while(dySize(list) > 0) // was != 0
    {
        Task* temp;
        int last = dySize(list)-1;
        temp = dyGet(list, last);
        taskDelete(temp);
        dyRemoveAt(list, last);
    }
    dyDelete(list);
    return 0;
}