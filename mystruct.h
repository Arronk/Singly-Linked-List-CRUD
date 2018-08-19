#include <cs50.h>
#include <stdio.h>
#include <string.h>

// SINGLY LINKED LIST NODE
typedef struct sllist
{
    int driverId;
    char *driverName;
    char *startDate;
    struct sllist* next;
}
sllnode;

// Declare Global Variables
extern sllnode *head;
extern bool run;

// Init functions
sllnode *populateRecord();

// Display main menu
void menu()
{
    printf("=================================================================================\n");
    printf("                                Sorted Driver List                               \n");
    printf("=================================================================================\n");
    printf("Please choose one of the following actions by entering the corresponding Integer:\n\n");
    printf("1. Add a new Driver Record\n");
    printf("2. Search for a Driver Record\n");
    printf("3. Update a Driver Record\n");
    printf("4. Delete a Driver Record\n");
    printf("5. Print the Driver List\n");
    printf("6. Delete the entire list\n");
    printf("7. Redisplay Menu\n");
    printf("8. Exit the Program\n");
    printf("=================================================================================\n\n");
}

// Function which inserts a new record, sorted as per the driver ID
void insert()
{

    // If no list exists create new list
    if (head == NULL)
    {
        // Creates and populates new record and checks if any errors occurred during mermory allocation
        sllnode *record = populateRecord();
        if (record == NULL)
        {
            printf("Error occurred when trying to allocate memory for new record\n");
            run = false;
        }

        // Direct head pointer to head of newly created list
        head = record;
    }
    else
    {

        sllnode *record = populateRecord();
        if (record == NULL)
        {
            printf("Error occurred when trying to allocate memory for new record\n");
            run = false;
        }

        // Insert at the beginning of list if driver ID is lower then head record
        if(record -> driverId < head -> driverId)
        {
            record -> next = head;
            head = record;
        }
        else
        {
            // Otherwise Iterate through list to find where to insert new record
            sllnode* ptr = head;
            while (ptr -> next != NULL && ptr -> next -> driverId < record -> driverId)
            {
                ptr = ptr -> next;
            }

            // Update the list pointers to insert the new record
            record -> next = ptr -> next;
            ptr -> next = record;
        }
    }
}

sllnode* populateRecord()
{
    // allocate new node to store record
    sllnode *record = malloc(sizeof(sllnode));
    bool reset = false;

    sllnode *ptr = head;

    // Prompts user for record data and stores it into the record
    record -> driverId = get_int("Enter Driver ID: ");

    // Loops throught list to check if Driver Id provided is unique
    while (ptr != NULL)
    {
        // Required to turn the reset flag to false if a double up is found
        reset = false;

        // If a double up is found
        if (ptr -> driverId == record -> driverId)
        {
            // reprompt the user until a a unique value is given
            record -> driverId = get_int("This Driver Id is not unique, please enter a unique Driver Id: ");

            // reset the while loop pointer to the head of the list
            ptr = head;
            reset = true;
        }

        // If the loop is reset due to a double entry it will not increment the pointer until the next iteration
        if(reset == false)
        {
            ptr = ptr -> next;
        }
    }

    record -> driverName = get_string("Enter Driver Name: ");
    record -> startDate = get_string("Enter Commencement Date (DD/MM/YYYY): ");
    record -> next = NULL;
    printf("Driver %i was added to the list!\n\n", record->driverId);
    return record;
}


// Search function
bool find()
{
    // Requests user for search term
    int value = get_int("Enter Driver ID: ");

    for (sllnode *ptr = head; ptr != NULL; ptr = ptr->next)
    {
        // If the search term is found in the list print the details and return true
        if (ptr->driverId == value)
        {
            printf("Record found!\nDriver ID: %i\nDriver Name: %s\nCommencement Date: %s\n\n", ptr -> driverId, ptr -> driverName, ptr -> startDate);
            return true;
        }
    }
    // If the search term is not found print the result and return false
    printf("Driver %i is not in the list!\n\n", value);
    return false;
}

// Updates records in the list
void updateRecord()
{
    // Prompts user for Driver ID to access record that requires updating
    int value = get_int("Enter Driver ID: ");

    // If there are no records the user is notified
    if(head == NULL)
    {
        printf("There are no records available to update.\n\n");
    }

    // If there is a list it searches it
    for (sllnode *ptr = head; ptr != NULL; ptr = ptr->next)
    {
        // If the driver ID is found it notifies the user and then makes them update both the name and commencement date
        if (ptr->driverId == value)
        {
            printf("Record found! Please enter the correct details.\n\n");
            ptr -> driverId = get_int("Correct Driver ID: ");
            ptr -> driverName = get_string("Correct Driver Name: ");
            ptr -> startDate = get_string("Correct Commencement Date (DD/MM/YYYY): ");
            printf("\n");
            break;
        }
        if (ptr -> next == NULL)
        {
            // If Id was not found in the list it lets the user know
            printf("No Record matches the provided Driver ID\n\n");
        }
    }
}

void deleteRecord()
{

    // Delcare pointers to assist with search and delete
    sllnode *ptr = head, *previous = NULL;

    // Declare variable to check if a node was found and deleted
    bool flag = false;
    bool notConfirmed = true;

    // Prompts user for Driver ID to access record that requires updating
    int value = get_int("Enter Driver ID: ");

    // Iterates through the list until the end
    while (ptr != NULL)
    {
        // If record is found
       if (ptr -> driverId == value)
       {
            // Show the user the record found and confirm if they want to delete it
            printf("Record found!\nDriver ID: %i\nDriver Name: %s\nCommencement Date: %s\n\n", ptr -> driverId, ptr -> driverName, ptr -> startDate);

            while (notConfirmed)
            {

                char *confirmDeletion = get_string("Are you sure you want to delete this record (Y/N)? ");
                if (strcmp(confirmDeletion, "Y") == 0)
                {
                    // If the record found is the head of the list
                    if (previous == NULL)
                    {
                        // Update the head of the list to the second record
                        head = ptr -> next;
                    }
                    else
                    {
                        // If the record to be deleted wasn't the head update the previous pointer to record immediately after the record to be deleted
                        previous -> next = ptr -> next;
                    }

                // Check nodefound flag to true and free deleted node to ensure no memory leaks
                notConfirmed = false;
                flag = true;
                free(ptr);
                printf("Record deleted!\n\n");
                break;
                }

                // Return to main menu if choose not to delete record
                else if (strcmp(confirmDeletion, "N") == 0)
                {
                    flag = true;
                    printf("\n");
                    break;
                }
                else
                {
                    printf("Please only enter Y or N\n");
                }
            }
            break;

        }

        // If value not found assign the previous pointer to ptr and move the ptr forward to the next item in the list
        previous = ptr;
        ptr = ptr -> next;
    }

    // If the search term matches no records alert the user
    if(flag == false)
    {
        printf("Record not found!\n\n");
    }
}

void printRecords()
{
    // Delcare pointer to iterate through the list
    sllnode *ptr = head;

    if(head == NULL)
    {
        printf("The list is empty!!\n\n");
    }
    // Iterates through the list until the end
    while (ptr != NULL)
    {
        // Prints every user record in the list
        printf("Driver ID: %i\nDriver Name: %s\nCommencement Date: %s\n\n", ptr -> driverId, ptr -> driverName, ptr -> startDate);
        ptr = ptr -> next;
    }
}


void deleteAll()
{

    sllnode *ptr = head;

    // while records exist iterate through the list
    while (ptr != NULL)
    {
        // Declare a variable that points to the next record in the list, delete the current record then move the next record
        sllnode *next = ptr -> next;
        free(ptr);
        ptr = next;
    }

    head = NULL;
    printf("All records have been deleted\n\n");
}