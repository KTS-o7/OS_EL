#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define PAGE_SIZE 409
#define NUM_PAGES 10
#define NUM_FRAMES 4

int main()
{
    FILE *fp;
    char filename[] = "2.c";
    char buffer[PAGE_SIZE];
    int i, j, k, page_faults = 0;
    int page_table[NUM_PAGES];
    int frame_table[NUM_FRAMES];

    // Initialize page table
    for (i = 0; i < NUM_PAGES; i++)
    {
        page_table[i] = -1;
    }

    // Initialize frame table
    for (i = 0; i < NUM_FRAMES; i++)
    {
        frame_table[i] = -1;
    }

    // Open file
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        return 1;
    }

    // Get file size
    struct stat st;
    if (stat(filename, &st) == -1)
    {
        fprintf(stderr, "Error getting file size for %s: %s\n", filename, strerror(errno));
        return 1;
    }
    printf("File size: %ld bytes\n", st.st_size);

    // Read file
    printf("Reading from file...\n");

    for (i = 0; i < NUM_PAGES; i++)
    {
        size_t bytes_read = fread(buffer, 1, PAGE_SIZE, fp);
        if (bytes_read == 0)
        {
            if (feof(fp))
            {
                fprintf(stderr, "Reached end of file before reading all pages\n");
            }
            else if (ferror(fp))
            {
                fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
            }
            else
            {
                fprintf(stderr, "Unknown error reading file %s\n", filename);
            }
            break;
        }

        // Map page to frame
        j = i % NUM_FRAMES;
        page_faults++;
        if (page_table[i] == -1)
        {
            page_table[i] = j;
            frame_table[j] = i;
        }
        else
        {
            k = page_table[i];
            page_table[i] = j;
            frame_table[j] = i;
            frame_table[k] = -1;
        }
    }

    // Print page table
    printf("Virtual address space size: %d bytes\n", PAGE_SIZE * NUM_PAGES);
    printf("Number of page frames: %d\n", NUM_FRAMES);
    printf("Page table:\n");
    for (i = 0; i < NUM_PAGES; i++)
    {
        if (page_table[i] == -1)
        {
            printf("Page %d: Not present\n", i);
        }
        else
        {
            printf("Page %d: frame %d\n", i, page_table[i]);
        }
    }
    printf("Number of page faults: %d\n", page_faults);

    // Close file
    fclose(fp);

    return 0;
}
