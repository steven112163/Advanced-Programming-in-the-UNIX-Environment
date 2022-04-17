#include <logger.hpp>

int chmod(const char* pathname, mode_t mode) {
    static int (*linux_chmod)(const char*, mode_t) = NULL;
    if (linux_chmod == NULL)
        linux_chmod = (int (*)(const char*, mode_t))dlsym(RTLD_NEXT, "chmod");

    int status{linux_chmod(pathname, mode)};
    std::cerr << "chmod(\"" << pathname << "\", " << mode << ") = " << status
              << std::endl;

    return status;
}

int chown(const char* pathname, uid_t owner, gid_t group) {
    static int (*linux_chown)(const char*, uid_t, gid_t) = NULL;
    if (linux_chown == NULL)
        linux_chown =
            (int (*)(const char*, uid_t, gid_t))dlsym(RTLD_NEXT, "chown");

    int status{linux_chown(pathname, owner, group)};
    std::cerr << "chown(\"" << pathname << "\", " << owner << ", " << group
              << ") = " << status << std::endl;

    return status;
}

int close(int fd) {
    static int (*linux_close)(int) = NULL;
    if (linux_close == NULL)
        linux_close = (int (*)(int))dlsym(RTLD_NEXT, "close");

    int status{linux_close(fd)};
    std::cerr << "close(\"" << fd << "\") = " << status << std::endl;

    return status;
}

int creat(const char* path, mode_t mode) {
    static int (*linux_creat)(const char*, mode_t) = NULL;
    if (linux_creat == NULL)
        linux_creat = (int (*)(const char*, mode_t))dlsym(RTLD_NEXT, "creat");

    int status{linux_creat(path, mode)};
    std::cerr << "creat(\"" << path << "\", " << mode << ") = " << status
              << std::endl;

    return status;
}

int fclose(FILE* stream) {
    static int (*linux_fclose)(FILE*) = NULL;
    if (linux_fclose == NULL)
        linux_fclose = (int (*)(FILE*))dlsym(RTLD_NEXT, "fclose");

    int status{linux_fclose(stream)};
    std::cerr << "fclose(\"" << stream << ") = " << status << std::endl;

    return status;
}

FILE* fopen(const char* pathname, const char* mode) {
    static FILE* (*linux_fopen)(const char*, const char*) = NULL;
    if (linux_fopen == NULL)
        linux_fopen =
            (FILE * (*)(const char*, const char*)) dlsym(RTLD_NEXT, "fopen");

    FILE* stream{linux_fopen(pathname, mode)};
    std::cerr << "fopen(\"" << pathname << "\", \"" << mode
              << "\") = " << stream << std::endl;

    return stream;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    static size_t (*linux_fread)(void*, size_t, size_t, FILE*) = NULL;
    if (linux_fread == NULL)
        linux_fread =
            (size_t(*)(void*, size_t, size_t, FILE*))dlsym(RTLD_NEXT, "fread");

    size_t number_of_items{linux_fread(ptr, size, nmemb, stream)};
    std::cerr << "fread(\"" << ptr << "\", " << size << ", " << nmemb
              << ", \"" << stream << "\") = " << number_of_items << std::endl;

    return number_of_items;
}

size_t fwrite(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    static size_t (*linux_fwrite)(void*, size_t, size_t, FILE*) = NULL;
    if (linux_fwrite == NULL)
        linux_fwrite =
            (size_t(*)(void*, size_t, size_t, FILE*))dlsym(RTLD_NEXT, "fwrite");

    size_t number_of_items{linux_fwrite(ptr, size, nmemb, stream)};
    std::cerr << "fwrite(\"" << ptr << "\", " << size << ", " << nmemb
              << "\", \"" << stream << "\") = " << number_of_items << std::endl;

    return number_of_items;
}

int open(const char* pathname, int flags) {
    static int (*linux_open)(const char*, int) = NULL;
    if (linux_open == NULL)
        linux_open = (int (*)(const char*, int))dlsym(RTLD_NEXT, "open");

    int fd{linux_open(pathname, flags)};
    std::cerr << "open(\"" << pathname << "\", " << flags << ") = " << fd
              << std::endl;

    return fd;
}

int open(const char* pathname, int flags, mode_t mode) {
    static int (*linux_open)(const char*, int, mode_t) = NULL;
    if (linux_open == NULL)
        linux_open =
            (int (*)(const char*, int, mode_t))dlsym(RTLD_NEXT, "open");

    int fd{linux_open(pathname, flags, mode)};
    std::cerr << "open(\"" << pathname << "\", " << flags << ", " << mode
              << ") = " << fd << std::endl;

    return fd;
}

ssize_t read(int fd, void* buf, size_t count) {
    static ssize_t (*linux_read)(int, void*, size_t) = NULL;
    if (linux_read == NULL)
        linux_read = (ssize_t(*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");

    ssize_t number_of_bytes{linux_read(fd, buf, count)};
    std::cerr << "read(\"" << fd << "\", \"" << buf << "\", " << count
              << ") = " << number_of_bytes << std::endl;

    return number_of_bytes;
}

int remove(const char* pathname) {
    static int (*linux_remove)(const char*) = NULL;
    if (linux_remove == NULL)
        linux_remove = (int (*)(const char*))dlsym(RTLD_NEXT, "remove");

    int status{linux_remove(pathname)};
    std::cerr << "remove(\"" << pathname << "\") = " << status << std::endl;

    return status;
}

int rename(const char* oldpath, const char* newpath) {
    static int (*linux_rename)(const char*, const char*) = NULL;
    if (linux_rename == NULL)
        linux_rename =
            (int (*)(const char*, const char*))dlsym(RTLD_NEXT, "rename");

    int status{linux_rename(oldpath, newpath)};
    std::cerr << "remove(\"" << oldpath << "\", \"" << newpath
              << "\") = " << status << std::endl;

    return status;
}

FILE* tmpfile(void) {
    static FILE* (*linux_tmpfile)(void) = NULL;
    if (linux_tmpfile == NULL)
        linux_tmpfile = (FILE * (*)(void)) dlsym(RTLD_NEXT, "tmpfile");

    FILE* stream{linux_tmpfile()};
    std::cerr << "tmpfile() = " << stream << std::endl;

    return stream;
}

ssize_t write(int fd, const void* buf, size_t count) {
    static ssize_t (*linux_write)(int, const void*, size_t) = NULL;
    if (linux_write == NULL)
        linux_write =
            (ssize_t(*)(int, const void*, size_t))dlsym(RTLD_NEXT, "write");

    ssize_t number_of_bytes{linux_write(fd, buf, count)};
    std::cerr << "write(\"" << fd << "\", \"" << buf << "\", " << count
              << ") = " << number_of_bytes << std::endl;

    return number_of_bytes;
}