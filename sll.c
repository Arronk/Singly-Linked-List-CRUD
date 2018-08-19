/*
    Linked List implementation using C programming Language
    Operations: Basic CRUD functionality for linked list sorted by Driver ID
    Programmed by: Arron Kennedy
    Contact: @azathecoder (Twitter)
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include "mystruct.h"

// Declare Global Variables
sllnode *head = NULL;
bool run = true;

int main(void)
{

    // Declare local variables
    int action;

    menu();

    while(run)
    {
        do
        {
            action = get_int("Main Menu - Select Action (1-8): ");
        }
        while (action < 1 || action > 8);
        printf("\n");

        // Initiate function to complete requested user action
        switch (action)
        {
            case 1 :
                insert();
                break;
            case 2 :
                find();
                break;
            case 3 :
                updateRecord();
                break;
            case 4 :
                deleteRecord();
                break;
            case 5 :
                printRecords();
                break;
            case 6 :
                deleteAll();
                break;
            case 7 :
                menu();
                break;
            case 8 :
                run = false;
                break;
        }
    }

    // Free memory if the list has not been deleted
    sllnode *ptr = head;

    while (ptr != NULL)
    {
        sllnode *next = ptr -> next;
        free(ptr);
        ptr = next;
    }
    printf("Exiting Program\n");
}

