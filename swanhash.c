/*
    Hash Table + Linked List implementation using C programming Language
    Operations: Basic CRUD functionality for Hash Table linked
    Programmed by: Arron Kennedy
    Contact: @azathecoder (Twitter)
*/

#include <cs50.h>
#include <string.h>
#include "hashstruct.h"

// Declare Global Variables
bool run = true;
sllnode *hashtable[HASHTABLE] = {NULL};

int main(void)
{

    // Declare local variables
    int action;

    // Print main menu to prompt user for operations
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

    deleteAll();
    printf("Exiting Program\n");
}

