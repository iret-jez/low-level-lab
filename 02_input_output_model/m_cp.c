#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 256

int main(int argc, char** argv)
{
    const char* source_file;
    const char* dest_file;

    if (argc < 3) {
        write(STDERR_FILENO, "Usage: m_cp file1 file2\n", 24);
        return 1;
    }

    source_file = argv[1];
    dest_file = argv[2];

    int flags_source = O_RDONLY;
    int flags_dest = O_WRONLY | O_CREAT | O_TRUNC;
    int mode_dest = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    int fd_source = open(source_file, flags_source);
    if (fd_source == -1) {
        write(STDERR_FILENO, "open failed\n", 12);
        return 1;
    }

    int fd_dest = open(dest_file, flags_dest, mode_dest);
    if (fd_dest == -1) {
        write(STDERR_FILENO, "open failed\n", 12);
        return 1;
    }

    char buffer[BUF_SIZE];
    ssize_t was_read;
    off_t zero_run = 0;
    while ((was_read = read(fd_source, buffer, BUF_SIZE)) > 0) {

        ssize_t data_start = -1;
        for (ssize_t i = 0; i < was_read; ++i) {

            if (buffer[i] == 0) {           

                if (data_start != -1) {
                    
                    size_t len = i - data_start;
                    if (write(fd_dest, buffer + data_start, len) == -1) {
                        write(STDERR_FILENO, "error in write\n", 15);
                        return 1;
                    }
                }

                data_start = -1;
                zero_run++;

            } else {

                if (zero_run > 0) {
                    if (lseek(fd_dest, zero_run, SEEK_CUR) == -1) {
                        write(STDERR_FILENO, "error in lseek\n", 15);
                        return 1;
                    }
                    zero_run = 0;
                }

                if (data_start == -1) {
                    data_start = i;
                }
            }
        }

        if (data_start != -1) {
            size_t len = was_read - data_start;
            if (write(fd_dest, buffer + data_start, len) == -1) {
                write(STDERR_FILENO, "error in write\n", 15);
                return 1;
            }
        }
    }

    if (was_read == -1) {
            write(STDERR_FILENO, "read error\n", 11);
        }

    if (zero_run > 0) {
        if (lseek(fd_dest, zero_run, SEEK_CUR) == -1) {
            write(STDERR_FILENO, "error in lseek\n", 15);
            return 1;
        }
        
        off_t pos = lseek(fd_dest, 0, SEEK_CUR);
        ftruncate(fd_dest, pos);
    }

    close(fd_source);
    close(fd_dest);
    return 0;
}
