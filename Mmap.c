
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
    int fd = open("file.txt", O_RDWR); // open file in read-write mode
    if (fd == -1)
    {
        perror("open"); // print error message and exit if file opening fails
        exit(1);
    }

    // get the size of the file
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == (off_t)-1)
    {
        perror("lseek"); // print error message and exit if getting file size fails
        exit(1);
    }

    // map the file into memory
    void *map = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        perror("mmap"); // print error message and exit if memory mapping fails
        exit(1);
    }

    // use the memory mapped file as a buffer for writing
    char *buffer = (char *)map;
    buffer[0] = 'H';
    buffer[1] = 'e';
    buffer[2] = 'l';
    buffer[3] = 'l';
    buffer[4] = 'o';
    buffer[5] = ',';
    buffer[6] = ' ';
    buffer[7] = 'W';
    buffer[8] = 'o';
    buffer[9] = 'r';
    buffer[10] = 'l';
    buffer[11] = 'd';
    buffer[12] = '!';
    buffer[13] = '\n';

    // unmap the memory and close the file
    munmap(map, file_size);
    close(fd);

    return 0;
}
