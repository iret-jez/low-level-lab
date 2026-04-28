#include <stdio.h>
#include <ctype.h>


int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("%s", "Not enough arguments");
        return 1;
    }

    FILE* handler = fopen(argv[1], "rb");
    unsigned char buffer[16];
    size_t offset = 0;
    size_t bytes_read;

    printf("Offset    |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | ASCII\n");
    printf("----------|-------------------------------------------------|-----------------\n");

    while ((bytes_read = fread(buffer, sizeof(buffer[0]), 16, handler)) > 0) {
        printf("%08zX  | ", offset);

        for (size_t i = 0; i < 16; ++i) {
            if (i < bytes_read) {
                printf("%02X ", buffer[i]);
            } else {
                printf("   ");
            }
        }
        printf("| ");

        for (size_t j = 0; j < bytes_read; ++j) {
            if (isprint(buffer[j])) {
                printf("%c", buffer[j]);
            } else {
                printf(".");
            }
        }
        printf("\n");

        offset += bytes_read;
    }

    fclose(handler);
    return 0;
}
