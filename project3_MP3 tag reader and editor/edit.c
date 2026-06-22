#include <stdio.h>
#include <string.h>
#include "edit.h"
#include "view.h"


void edit_data(const char *mp3_filename, char *tagname, char *newdata)
{
    // Open the source file for reading and a temporary file for writing
    FILE *fptr1 = fopen(mp3_filename, "rb");
    FILE *fptr2 = fopen("temp.mp3", "wb");

    if (fptr1 == NULL)
    {
        printf("ERROR : Source file not opened\n");
        return;
    }
    if (fptr2 == NULL)
    {
        printf("ERROR : Temp file not opened\n");
        return;
    }

    // Step 1: Handle the MP3 ID3 header (usually the first 10 bytes)
    char header[10];
    fread(header, 10, 1, fptr1);
    fwrite(header, 10, 1, fptr2);
    printf("INFO : ID3 header copied successfully\n");
    // Step 2: Loop through the metadata frames (searching for common tags)
    char tag[5];
    int size;
    int newsize;
    char info[2000];

    // We check for the 6 standard tags we support
    for (int i = 0; i < 6; i++)
    {
        // Read the 4-character Frame ID (like TIT2, TPE1, etc.)
        fread(tag, 4, 1, fptr1);
        printf("Tag read success\n");
        tag[4] = '\0';
        printf("INFO : Frame read -> %s\n", tag);
        char atag[3], flags[3];

        // Map the technical ID3 Frame ID to our user-friendly flag names
        if (strcmp(tag, "TIT2") == 0)
        {
            strcpy(atag, "-t"); // Title
        }
        else if (strcmp(tag, "TYER") == 0)
        {
            strcpy(atag, "-y"); // Year
        }
        else if (strcmp(tag, "TALB") == 0)
        {
            strcpy(atag, "-A"); // Album
        }
        else if (strcmp(tag, "TPE1") == 0)
        {
            strcpy(atag, "-a"); // Artist
        }
        else if (strcmp(tag, "TCON") == 0)
        {
            strcpy(atag, "-g"); // Genre
        }
        else if (strcmp(tag, "COMM") == 0)
        {
            strcpy(atag, "-c"); // Comment
        }

        // Read the frame size and convert it (handling Big-Endian/Syncsafe format)
        fread(&size, 4, 1, fptr1);
        convert(&size);
        
        // Read the 3-byte flags associated with the tag
        fread(flags, 3, 1, fptr1);

        // Read the actual metadata content (the string)
        fread(info, size - 1, 1, fptr1);
        info[size - 1] = '\0';

        // Step 3: Check if this is the tag the user wants to edit
        if (strcmp(tagname, atag) == 0)
        {
            // If it matches, write the NEW data to the temp file
            newsize = strlen(newdata) + 1;
            int new_frame_size = newsize;
            convert(&new_frame_size); // Convert size back for the MP3 format

            fwrite(tag, 4, 1, fptr2);      // Write same Tag ID
            fwrite(&new_frame_size, 4, 1, fptr2);   // Write new content size
            fwrite(flags, 3, 1, fptr2);    // Write original flags
            fwrite(newdata, newsize - 1, 1, fptr2); // Write new data string
        }
        else
        {
            // If it doesn't match, just copy the original data to the temp file
            fwrite(tag, 4, 1, fptr2);
            int old_frame_size = size;
            convert(&old_frame_size);
            fwrite(&old_frame_size, 4, 1, fptr2);
            fwrite(flags, 3, 1, fptr2);
            fwrite(info, size - 1, 1, fptr2);
        }
    }

    // Step 4: Copy the rest of the file (audio data) character by character
    char ch;
    while ((ch = fgetc(fptr1)) != EOF)
    {
        fputc(ch, fptr2);
    }

    // Step 5: Clean up and replace the old file with the new one
    fclose(fptr1);
    fclose(fptr2);

    // Note: This specifically targets 'Song.mp3' as per original code logic
    remove("Song.mp3");
    rename("temp.mp3", "Song.mp3");
}