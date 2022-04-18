#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

int main() {
    char buf[64] = {0};
    FILE* tmp_file{};
    FILE* test_file{};

    int fd_a = creat("./aaaa", S_IRUSR | S_IWUSR);
    chmod("./aaaa", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    chown("./aaaa", geteuid(), getegid());
    rename("./aaaa", "./bbbb");

    int fd_b = open("./bbbb", O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    write(fd_b, "cccc.", 5);
    close(fd_b);

    fd_b = open("./bbbb", 0, 0);
    read(fd_b, buf, 64);
    close(fd_b);

    tmp_file = tmpfile();
    fwrite("cccc.", 1, 5, tmp_file);
    fclose(tmp_file);

    test_file = fopen("./bbbb", "r");
    fread(buf, 1, 64, test_file);
    fclose(test_file);
    remove("./bbbb");

    char output_buf[] = "sample done.\n";
    write(1, output_buf, 13);

    return 0;
}