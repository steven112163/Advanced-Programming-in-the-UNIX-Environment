#include <logger.hpp>

int chmod(const char* pathname, mode_t mode) {
    int status{linux_chmod(pathname, mode)};

    std::stringstream ss;
    ss << "[logger] chmod(\"" << pathname << "\", " << std::setfill('0')
       << std::setw(3) << std::oct << mode << ") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int chown(const char* pathname, uid_t owner, gid_t group) {
    int status{linux_chown(pathname, owner, group)};

    std::stringstream ss;
    ss << "[logger] chown(\"" << pathname << "\", " << owner << ", " << group
       << ") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int close(int fd) {
    int status{linux_close(fd)};

    std::stringstream ss;
    ss << "[logger] close(\"" << fd << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int creat(const char* pathname, mode_t mode) {
    int status{linux_creat(pathname, mode)};

    std::stringstream ss;
    ss << "[logger] creat(\"" << pathname << "\", " << std::setfill('0')
       << std::setw(3) << std::oct << mode << ") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int fclose(FILE* stream) {
    int status{linux_fclose(stream)};

    std::stringstream ss;
    ss << "[logger] fclose(\"" << stream << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

FILE* fopen(const char* pathname, const char* mode) {
    FILE* stream{linux_fopen(pathname, mode)};

    std::stringstream ss;
    ss << "[logger] fopen(\"" << pathname << "\", \"" << mode
       << "\") = " << stream << std::endl;
    print_log(ss.str());

    return stream;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t number_of_items{linux_fread(ptr, size, nmemb, stream)};

    std::stringstream ss;
    ss << "[logger] fread(\"" << ptr << "\", " << size << ", " << nmemb
       << ", \"" << stream << "\") = " << number_of_items << std::endl;
    print_log(ss.str());

    return number_of_items;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t number_of_items{linux_fwrite(ptr, size, nmemb, stream)};

    std::stringstream ss;
    ss << "[logger] fwrite(\"" << ptr << "\", " << size << ", " << nmemb
       << ", \"" << stream << "\") = " << number_of_items << std::endl;
    print_log(ss.str());

    return number_of_items;
}

int open(const char* pathname, int flags, ...) {
    va_list args{};
    va_start(args, flags);
    mode_t mode{va_arg(args, mode_t)};

    int fd{linux_open(pathname, flags, mode)};

    std::stringstream ss;
    ss << "[logger] open(\"" << pathname << "\", " << std::setfill('0')
       << std::setw(3) << std::oct << flags << ", " << std::setfill('0')
       << std::setw(3) << std::oct << mode << ") = " << fd << std::endl;
    print_log(ss.str());

    va_end(args);

    return fd;
}

ssize_t read(int fd, void* buf, size_t count) {
    ssize_t number_of_bytes{linux_read(fd, buf, count)};

    std::stringstream ss;
    ss << "[logger] read(\"" << fd << "\", \"" << buf << "\", " << count
       << ") = " << number_of_bytes << std::endl;
    print_log(ss.str());

    return number_of_bytes;
}

int remove(const char* pathname) {
    int status{linux_remove(pathname)};

    std::stringstream ss;
    ss << "[logger] remove(\"" << pathname << "\") = " << status << std::endl;
    print_log(ss.str());

    return status;
}

int rename(const char* oldpath, const char* newpath) {
    int status{linux_rename(oldpath, newpath)};

    std::stringstream ss;
    ss << "[logger] rename(\"" << oldpath << "\", \"" << newpath
       << "\") = " << status << std::endl;
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
    ssize_t number_of_bytes{linux_write(fd, buf, count)};

    std::stringstream ss;
    ss << "[logger] write(\"" << fd << "\", \"" << buf << "\", " << count
       << ") = " << number_of_bytes << std::endl;
    print_log(ss.str());

    return number_of_bytes;
}