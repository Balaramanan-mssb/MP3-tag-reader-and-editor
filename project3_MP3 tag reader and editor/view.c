#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"

void view_tag(char const *filename)
{
    printf("INFO : Opening MP3 file -> %s\n", filename);
    // Open the MP3 file in binary read mode
    FILE *fptr = fopen(filename, "rb"); 

    if (fptr == NULL)
    {
        printf("ERROR : Unable to open file\n");
        return;
    }
    printf("INFO : File opened successfully\n");
    // Skip the first 10 bytes which contain the main ID3 header
    fseek(fptr, 10, SEEK_SET);
    printf("INFO : Skipped ID3 header (10 bytes)\n");
    
    char tag[5];
    int size;
    char info[1000];
    int count = 0;

    // We keep looping through the file frames until we've found all 6 tags we care about
    while (count != 6)
    {
        // Read the 4-byte Frame ID (e.g., "TIT2")
        fread(tag, 4, 1, fptr);
        tag[4] = '\0';
        printf("\nINFO : Frame ID detected -> %s\n", tag);

        // Read the 4-byte size of the current frame
        fread(&size, 4, 1, fptr);
        
        // MP3 sizes are stored in Big-Endian; we need to flip the bytes for our system
        convert(&size);

        // Skip the 3 bytes used for frame flags (we don't need them for viewing)
        fseek(fptr, 3, SEEK_CUR);

        // Read the actual text data based on the size we just calculated
        // We read size - 1 to account for the encoding byte
        fread(info, size - 1, 1, fptr);
        info[size - 1] = '\0'; // Ensure the string is null-terminated for printing

        // Check the Tag ID and print the corresponding human-readable info
        if (strcmp(tag, "TIT2") == 0)
        {
            printf("Title->%s\n", info);
            count++;
        }
        if (strcmp(tag, "TYER") == 0)
        {
            printf("YEAR->%s\n", info);
            count++;
        }
        if (strcmp(tag, "TALB") == 0)
        {
            printf("ALBUM->%s\n", info);
            count++;
        }
        if (strcmp(tag, "TPE1") == 0)
        {
            printf("Artist->%s\n", info);
            count++;
        }
        if (strcmp(tag, "TCON") == 0)
        {
            printf("Genre->%s\n", info);
            count++;
        }
        if (strcmp(tag, "COMM") == 0)
        {
            printf("Comment->%s\n", info);
            count++;
        }
    }
    fclose(fptr); 
    printf("INFO : View operation completed\n");
}

void convert(int *size)
{
    // Treat the integer as an array of 4 bytes (char pointer)
    char *p = (char*)size;
    int l = 3; // Index of the last byte
    int i = 0; // Index of the first byte
    
    // Swap the bytes: 0 with 3, and 1 with 2
    while (i < l)
    {
        char temp = p[i];
        p[i] = p[l];
        p[l] = temp;
        i++;
        l--;
    }
}