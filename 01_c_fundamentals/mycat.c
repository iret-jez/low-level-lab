#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("%s", "Not enough arguments");
        return 1;
    }
    
    char buffer[256];
    FILE* handler;
    for (int i = 1; i < argc; ++i) {
        handler = fopen(argv[i], "r");
        while (fgets(buffer, sizeof(buffer), handler) != NULL) {
            printf("%s", buffer);
        }
    }

    return 0;
}
