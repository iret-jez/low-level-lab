#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("%s", "Not enough arguments");
        return 1;
    }
    
    char buffer[256];
    FILE* handler = fopen(argv[1], "r");
    int lines = 0;
    int words = 0;
    size_t bytes = 0;
    while (fgets(buffer, sizeof(buffer), handler) != NULL) {
        lines++;
        bytes += strlen(buffer);
        char* p = buffer;
        while (*p) {
            if (isspace((char) *p)) {
                words++;
            }
            p++;
        }
    }

    printf("Lines: %d\nWords: %d\nBytes: %zd", lines, words, bytes);

    return 0;
}

