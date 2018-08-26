#include <stdio.h>
#include <ctype.h>

#define HASHTABLE 10

// SINGLY LINKED LIST NODE
typedef struct sllist
{
    int driverId;
    char *driverName;
    char *startDate;
    struct sllist* next;
}
sllnode;

// Integer has function https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int int_hash_func (unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x % HASHTABLE;
}

// Declare Global Variables
extern bool run;
extern sllnode *hashtable[HASHTABLE];

// Init functions
sllnode *populateRecord();

// Display main menu
void menu()
{
    printf("=================================================================================\n");
    printf("                                Driver List Main Menu                            \n");
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
    // Creates and populates new record and checks if any errors occurred during mermory allocation
    sllnode *record = populateRecord();

    // Prints error message and exits program if there is an error allocating memory
    if (record == NULL)
    {
        printf("Error occurred when trying to allocate memory for new record\n");
        run = false;
    }

    // Run driverId through int hash function to determine what SLL to add to
    unsigned int bucket_number = int_hash_func(record -> driverId);

    // If the chosen bucket does not hold any records
    if (hashtable[bucket_number] == NULL)
    {
        // allocate memory in bucket to attach SLL
        sllnode *bucketlist = calloc(1, sizeof(sllnode));

        // Prints error message and exits program if there is an error allocating memory
        if (bucketlist == NULL)
            {
                printf("Error occurred when trying to allocate memory for new record\n");
                run = false;
            }

        // Point bucket to allocated memory
        hashtable[bucket_number] = bucketlist;

        // Attach record to head of the list and prompt the user
        hashtable[bucket_number] -> next = record;
        printf("Driver %i was added to the list!\n\n", record->driverId);
    }
    else
    {
        // If list exists point new record to first record in the list and change the head pointer to the new record
        record -> next = hashtable[bucket_number] -> next;
        hashtable[bucket_number] -> next = record;
    }
}

sllnode* populateRecord()
{
    // allocate new node to store record
    sllnode *record = malloc(sizeof(sllnode));

    bool reset = false;

    record -> driverId = get_int("Enter Driver ID: ");

    // Run driverId through int hash function to determine what SLL to check for duplicate ID
    unsigned int bucket_number = int_hash_func(record -> driverId);

    sllnode *ptr = hashtable[bucket_number];

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
            ptr = hashtable[bucket_number];
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
    return record;
}

// Search function
bool find()
{
    // Requests user for search term and finds bucket
    unsigned int value = get_int("Enter Driver ID: ");
    unsigned int bucket_number = int_hash_func(value);

    for (sllnode *ptr = hashtable[bucket_number]; ptr != NULL; ptr = ptr->next)
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
    // Requests user for search term and finds bucket
    unsigned int value = get_int("Enter Driver ID: ");
    unsigned int bucket_number = int_hash_func(value);

    // If there is a list it searches it
    for (sllnode *ptr = hashtable[bucket_number]; ptr != NULL; ptr = ptr->next)
    {
        if (hashtable[bucket_number] != NULL)
        {
            // If the driver ID is found it notifies the user and then makes them update both the name and commencement date
            if (ptr->driverId == value)
            {
                printf("Record found! Please enter the correct details.\n\n");
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

    // If bucket is NULL alert the user
    if (hashtable[bucket_number] ==  NULL)
    {
        printf("Bucket is empty - no records here!\n\n");
    }
}

void deleteRecord()
{


    // Declare variable to check if a node was found and deleted
    bool flag = false;
    bool notConfirmed = true;

    // Requests user for search term and finds bucket
    unsigned int value = get_int("Enter Driver ID: ");
    unsigned int bucket_number = int_hash_func(value);

    // Delcare pointers to assist with search and delete
    sllnode *ptr = hashtable[bucket_number], *previous = NULL;

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
                    // If the record to be deleted wasn't the head update the previous pointer to record immediately after the record to be deleted
                    previous -> next = ptr -> next;

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
    int size = sizeof(hashtable) / sizeof(hashtable[0]);
    int count = 0;

    // Loop through the array of pointers looking for non null values (records in singly linked lists)
    for (int i = 0; i < size; i++)
    {
        // If a list of records is found
        if (hashtable[i] != NULL)
        {
            // Declare a pointer to loop over the SLL
            sllnode *ptr = hashtable[i];

            // while records exist iterate through the list
            while (ptr != NULL)
            {
                // Prints every user record in the list
                if (ptr -> driverId > 0)
                {
                    printf("Bucket: %i\nDriver ID: %i\nDriver Name: %s\nCommencement Date: %s\n\n", i, ptr -> driverId, ptr -> driverName, ptr -> startDate);
                }
                ptr = ptr -> next;
            }

            count++;
        }


    }

    if (count == 0)
    {
        printf("No records have been created!\n\n");
    }
}


void deleteAll()
{

    int size = sizeof(hashtable) / sizeof(hashtable[0]);

    // Loop through the array of pointers looking for non null values (records in singly linked lists)
    for (int i = 0; i < size; i++)
    {
        // If a list of records is found
        if (hashtable[i] != NULL)
        {
            // Declare a pointer to loop over the SLL
            sllnode *ptr = hashtable[i];

            // while records exist iterate through the list
            while (ptr != NULL)
            {
                // Declare a variable that points to the next record in the list, delete the current record then move the next record
                sllnode *next = ptr -> next;
                free(ptr);
                ptr = next;
            }

            // To ensure no memory leaks set the array pointer back to NULL after all memory has been free'd
            hashtable[i] = NULL;
        }

    }
    printf("All records have been deleted\n\n");
}
