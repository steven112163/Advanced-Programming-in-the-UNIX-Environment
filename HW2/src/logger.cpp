#include <logger.hpp>

int chmod(const char* pathname, mode_t mode) {
    int status{linux_chmod(pathname, mode)};

    char real_pathname[PATH_MAX];
    char* result{realpath(pathname, real_pathname)};

    std::stringstream ss;
    ss << "[logger] chmod(\"";
    if (result == nullptr)
        ss << pathname;
    else
        ss << real_pathname;
    ss << "\", " << std::setfill('0') << std::setw(3) << std::oct << mode
       << ") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int chown(const char* pathname, uid_t owner, gid_t group) {
    int status{linux_chown(pathname, owner, group)};

    char real_pathname[PATH_MAX];
    char* result{realpath(pathname, real_pathname)};

    std::stringstream ss;
    ss << "[logger] chown(\"";
    if (result == nullptr)
        ss << pathname;
    else
        ss << real_pathname;
    ss << "\", " << owner << ", " << group << ") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int close(int fd) {
    std::stringstream ss;
    ss << "[logger] close(\"";
    ss << resolve_fd(fd);

    int status{linux_close(fd)};

    ss << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int creat(const char* pathname, mode_t mode) {
    int status{linux_creat(pathname, mode)};

    char real_pathname[PATH_MAX];
    char* result{realpath(pathname, real_pathname)};

    std::stringstream ss;
    ss << "[logger] creat(\"";
    if (result == nullptr)
        ss << pathname;
    else
        ss << real_pathname;
    ss << "\", " << std::setfill('0') << std::setw(3) << std::oct << mode
       << ") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int fclose(FILE* stream) {
    std::stringstream ss;
    ss << "[logger] fclose(\"";
    ss << resolve_file(stream);

    int status{linux_fclose(stream)};

    ss << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

FILE* fopen(const char* pathname, const char* mode) {
    FILE* stream{linux_fopen(pathname, mode)};

    char real_pathname[PATH_MAX];
    char* result{realpath(pathname, real_pathname)};

    std::stringstream ss;
    ss << "[logger] fopen(\"";
    if (result == nullptr)
        ss << pathname;
    else
        ss << real_pathname;
    ss << "\", \"" << mode << "\") = " << stream << std::endl;
    print_log(ss.str());

    return stream;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t number_of_items{linux_fread(ptr, size, nmemb, stream)};

    std::stringstream ss;
    ss << "[logger] fread(\"" << convert_char_buf((char*)ptr) << "\", " << size
       << ", " << nmemb << ", \"";
    ss << resolve_file(stream);
    ss << "\") = " << number_of_items << std::endl;
    print_log(ss.str());

    return number_of_items;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    std::stringstream ss;
    ss << "[logger] fwrite(\"" << convert_char_buf((char*)ptr) << "\", " << size
       << ", " << nmemb << ", \"";
    ss << resolve_file(stream);
    size_t number_of_items{linux_fwrite(ptr, size, nmemb, stream)};

    ss << "\") = " << number_of_items << std::endl;
    print_log(ss.str());

    return number_of_items;
}

int open(const char* pathname, int flags, ...) {
    va_list args{};
    va_start(args, flags);
    mode_t mode{va_arg(args, mode_t)};

    int fd{linux_open(pathname, flags, mode)};

    char real_pathname[PATH_MAX];
    char* result{realpath(pathname, real_pathname)};

    std::stringstream ss;
    ss << "[logger] open(\"";
    if (result == nullptr)
        ss << pathname;
    else
        ss << real_pathname;
    ss << "\", " << std::setfill('0') << std::setw(3) << std::oct << flags
       << ", " << std::setfill('0') << std::setw(3) << std::oct << mode
       << ") = " << fd << std::endl;
    print_log(ss.str());

    va_end(args);

    return fd;
}

ssize_t read(int fd, void* buf, size_t count) {
    std::stringstream ss;
    ss << "[logger] read(\"";
    ss << resolve_fd(fd);

    ssize_t number_of_bytes{linux_read(fd, buf, count)};

    ss << "\", \"" << convert_char_buf((char*)buf) << "\", " << count
       << ") = " << number_of_bytes << std::endl;
    print_log(ss.str());

    return number_of_bytes;
}

int remove(const char* pathname) {
    char real_pathname[PATH_MAX];
    char* result{realpath(pathname, real_pathname)};

    std::stringstream ss;
    ss << "[logger] remove(\"";
    if (result == nullptr)
        ss << pathname;
    else
        ss << real_pathname;

    int status{linux_remove(pathname)};
    ss << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int rename(const char* oldpath, const char* newpath) {
    std::stringstream ss;
    ss << "[logger] rename(\"";

    char real_pathname[PATH_MAX];
    char* result{realpath(oldpath, real_pathname)};
    if (result == nullptr)
        ss << oldpath;
    else
        ss << real_pathname;

    int status{linux_rename(oldpath, newpath)};
    ss << "\", \"";

    result = realpath(newpath, real_pathname);
    if (result == nullptr)
        ss << newpath;
    else
        ss << real_pathname;

    ss << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

FILE* tmpfile(void) {
    FILE* stream{linux_tmpfile()};

    std::stringstream ss;
    ss << "[logger] tmpfile() = " << stream << std::endl;
    print_log(ss.str());

    return stream;
}

ssize_t write(int fd, const void* buf, size_t count) {
    std::stringstream ss;
    ss << "[logger] write(\"";
    ss << resolve_fd(fd);

    ssize_t number_of_bytes{linux_write(fd, buf, count)};

    ss << "\", \"" << convert_char_buf((char*)buf) << "\", " << count
       << ") = " << number_of_bytes << std::endl;
    print_log(ss.str());

    return number_of_bytes;
}