#include <util.hpp>

int get_fd() {
    char* filename{getenv("OUTPUT_FILE")};

    int fd{};
    if (filename == nullptr)
        fd = fileno(stderr);
    else
        fd = linux_open(filename, O_WRONLY | O_CREAT | O_APPEND,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1) {
        std::string output_str{"[logger] Failed to open " +
                               std::string(filename) +
                               ". Error: " + strerror(errno) + "\n"};
        linux_write(fileno(stderr), output_str.c_str(), output_str.length());
    }

    return fd;
}

void close_fd(const int& fd) {
    if (fd != fileno(stderr)) linux_close(fd);
}

void print_log(const std::string& log) {
    int fd{get_fd()};
    linux_write(fd, log.c_str(), log.length());
    close_fd(fd);
}