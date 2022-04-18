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

std::string convert_char_buf(const char* ptr) {
    int idx{0};
    std::string result{};
    while (ptr[idx] != '\0' && idx < 32) {
        if (isprint(ptr[idx]))
            result += ptr[idx];
        else
            result += '.';
        idx++;
    }

    return result;
}

std::string resolve_fd(const int& fd) {
    char buf[100];
    std::string fd_name{"/proc/self/fd/" + std::to_string(fd)};
    ssize_t size{readlink(fd_name.c_str(), buf, 100)};

    if (size == -1) return fd_name;

    buf[size] = '\0';
    char real_pathname[PATH_MAX];
    char* result{realpath(buf, real_pathname)};
    if (result == nullptr) return std::string(buf);
    return std::string(real_pathname);
}

std::string resolve_file(FILE* stream) { return resolve_fd(fileno(stream)); }