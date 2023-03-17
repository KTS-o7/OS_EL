#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_past_24_hours_access(struct stat fileStat) {
    time_t now;
    time(&now);
    time_t past_24_hours = now - (24 * 60 * 60); // 24 hours ago
    printf("Access times from the past 24 hours:\n");
    if (fileStat.st_atime > past_24_hours) {
        printf("- %s", ctime(&fileStat.st_atime));
    }
    if (fileStat.st_mtime > past_24_hours) {
        printf("- %s", ctime(&fileStat.st_mtime));
    }
    if (fileStat.st_ctime > past_24_hours) {
        printf("- %s", ctime(&fileStat.st_ctime));
    }
    if (fileStat.st_atime <= past_24_hours && fileStat.st_mtime <= past_24_hours && fileStat.st_ctime <= past_24_hours) {
        printf("- File has not been accessed in the past 24 hours.\n");
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s file/directory_name(s)\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        struct stat fileStat;
        if (stat(argv[i], &fileStat) < 0) {
            printf("Error getting file/directory information for %s\n", argv[i]);
            continue;
        }
        printf("\nName:%s\n",argv[i]);

        // File type
        if (S_ISREG(fileStat.st_mode)) {
            printf("File type: Regular file\n");
        } else if (S_ISDIR(fileStat.st_mode)) {
            printf("File type: Directory\n");
        } else {
            printf("File type: Other\n");
        }

        // Number of links
        printf("Number of links: %ld\n", fileStat.st_nlink);

        // Time of last access
        printf("Time of last access: %s", ctime(&fileStat.st_atime));

        // Read, write, and execute permissions
        printf("Read permission: ");
        if (fileStat.st_mode & S_IRUSR) {
            printf("Given ");
        } else {
            printf("Denied ");
        }
        printf("\nWrite permission: ");
        if (fileStat.st_mode & S_IWUSR) {
            printf("Given ");
        } else {
            printf("Denied ");
        }
        printf("\nExecute permission: ");
        if (fileStat.st_mode & S_IXUSR) {
            printf("Given\n");
        } else {
            printf("Denied\n");
        }

        // Access times from the past 24 hours
        print_past_24_hours_access(fileStat);
    }

    return 0;
}
