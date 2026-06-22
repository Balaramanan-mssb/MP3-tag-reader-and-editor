/*
DOCUMENTATION:
--------------------------------------------------------------------------
Name: Saravanan k
Reg.no: 25031_072
Start date: 17/12/2025
End date: 17/01/2026
Description : Project_3 -> MP3_Tag Reader/Editor Application in C Language.
--------------------------------------------------------------------------
--------------------------------------------------------------------------
*/

#include <stdio.h>
#include <ctype.h>
#include "view.h"   // Custom header for viewing MP3 tags
#include "edit.h"   // Custom header for editing MP3 tags
#include <string.h>

/* 
 * This function checks if the provided filename actually ends with ".mp3".
 * It searches for the last occurrence of '.' and compares the suffix.
 */
int validate_filenamed(char *filename)
{
    // Find the last dot in the filename to locate the extension
    char *ext = strrchr(filename, '.');
    
    // Check if the extension exists and matches ".mp3" exactly
    if (ext != NULL && strcmp(ext, ".mp3") == 0)
    {
        return 1; // It's a valid MP3 file
    }
    return 0; // Not an MP3 file
}

/* 
 * Displays a quick reminder of how the program should be called 
 * if the user provides the wrong arguments.
 */
void print_error_info()
{
    printf("\n--------------------- ERROR MESSAGE ---------------------------------------\n\n");
    printf("ERROR : ./a.out : INVALID ARGUMENTS\n"); printf("USAGE :\n"); 
    printf("To view please pass like : ./a.out -v mp3filename\n"); 
    printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n"); 
    printf("To get help pass like : ./a.out --help\n\n"); 
    printf("\n----------------------------------------------------------------------------\n\n");
}

/* 
 * A detailed manual for the user, explaining what each flag 
 * (-t, -a, etc.) does for editing metadata.
 */
void print_help()
{
    printf("\n--------------------- HELP MENU ---------------------\n\n"); 
    printf("1. -v -> to view mp3 file contents\n"); 
    printf("2. -e -> to edit mp3 file contents\n");
    printf("2.1 -t -> to edit song title\n"); 
    printf("2.2 -a -> to edit artist name\n"); 
    printf("2.3 -A -> to edit album name\n"); 
    printf("2.4 -y -> to edit year\n"); 
    printf("2.5 -m -> to edit content\n"); 
    printf("2.6 -c -> to edit comment\n\n");
}

int main()
{
    char option[3];
    while (1)
    {
        printf("\n----->mp3---->menu\n");
        printf("1.view  (-v)\n");
        printf("2.Edit  (-e)\n");
        printf("3.help  (-h)\n");
        printf("4.exit  (-q)\n");

        printf("Which operation you want to do(only use command): ");
        scanf("%s", option);

        if (strcmp(option, "-v") == 0)
        {
            char filename[50];
            printf("Enter the file name: ");
            scanf("%s", filename);
            if (validate_filenamed(filename))
            {
                view_tag(filename);
            }
            else
            {
                printf("Invalid filename!\n");
            }
        }
        else if (strcmp(option, "-e") == 0)
        {
            char filename1[50], tagname[50], newdata[50];

            printf("Enter the file name: ");
            scanf("%s", filename1);

            if (validate_filenamed(filename1))
            {
                printf("Enter the tag name: ");
                scanf("%s", tagname);
                printf("Enter the new data: ");
                scanf("%s", newdata);
                edit_data(filename1, tagname, newdata);
            }
            else
                printf("Invalid filename!\n");
        }
        else if (strcmp(option, "-h") == 0)
        {
            print_help();
        }
        else if (strcmp(option, "-q") == 0)
        {
            printf("Exiting program...\n");
            break;
        }
        else
        {
            print_error_info();
        }
    }
    return 0;
}
