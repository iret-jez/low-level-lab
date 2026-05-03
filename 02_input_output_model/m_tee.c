#include <unistd.h>
//#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
//#include <errno.h>

#define BUF_SIZE 256

ssize_t write_all(int fd, const void* buf, size_t count)
{
    size_t total = 0;
    const char* p = buf;

    while (total < count) {
        ssize_t n = write(fd, p + total, count - total);

        if (n <= 0) {
            write(STDERR_FILENO, "Write error\n", 12);
            return -1;
        }

        total += n;
    }

    return total;
}

int main(int argc, char** argv)
{   
    int append = 0;
    char* filename;

    if (argc == 2) {
        filename = argv[1];
    } else if (argc == 3 && strcmp(argv[1], "-a") == 0) {
        append = 1;
        filename = argv[2];
    } else {
        write(STDERR_FILENO, "Usage: m_tee [-a] file\n", 23);
        return 1;
    }

    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = open(filename, flags, mode);

    if (fd == -1) {
        write(STDERR_FILENO, "open failed\n", 12);
        return 1;
    }

    char buffer[BUF_SIZE];

    ssize_t was_read;
    while ((was_read = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0) {
        write_all(STDOUT_FILENO, buffer, was_read);
        write_all(fd, buffer, was_read);
    }
    
    close(fd);
    return 0;
}
