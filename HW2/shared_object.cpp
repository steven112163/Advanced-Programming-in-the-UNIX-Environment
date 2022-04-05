#include <shared_object.hpp>

int chmod(const char* pathname, mode_t mode) {
    if (old_chmod == NULL && handle != NULL)
        old_chmod = (int (*)(const char*, mode_t))dlsym(handle, "chmod");

    int status{old_chmod(pathname, mode)};
    std::cerr << "chmod(\"" << pathname << "\", " << mode << ") = " << status
              << std::endl;

    return status;
}

int chown(const char* pathname, uid_t owner, gid_t group) {
    if (old_chown == NULL && handle != NULL)
        old_chown = (int (*)(const char*, uid_t, gid_t))dlsym(handle, "chown");

    int status{old_chown(pathname, owner, group)};
    std::cerr << "chown(\"" << pathname << "\", " << owner << ", " << group
              << ") = " << status << std::endl;

    return status;
}

int close(int fd) {
    if (old_close == NULL && handle != NULL)
        old_close = (int (*)(int))dlsym(handle, "close");

    int status{old_close(fd)};
    std::cerr << "close(\"" << fd << "\") = " << status << std::endl;

    return status;
}

int creat(const char* path, mode_t mode) {
    if (old_creat == NULL && handle != NULL)
        old_creat = (int (*)(const char*, mode_t))dlsym(handle, "creat");

    int status{creat(path, mode)};
    std::cerr << "creat(\"" << path << "\", " << mode << ") = " << status
              << std::endl;

    return status;
}

int fclose(FILE* stream) {
    if (old_fclose == NULL && handle != NULL)
        old_fclose = (int (*)(FILE*))dlsym(handle, "fclose");

    int status{old_fclose(stream)};
    std::cerr << "fclose(\"" << stream << ") = " << status << std::endl;

    return status;
}

FILE* fopen(const char* pathname, const char* mode) {
    if (old_fopen == NULL && handle != NULL)
        old_fopen =
            (FILE * (*)(const char*, const char*)) dlsym(handle, "fopen");

    FILE* stream{old_fopen(pathname, mode)};
    std::cerr << "fopen(\"" << pathname << "\", \"" << mode
              << "\") = " << stream << std::endl;

    return stream;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    if (old_fread == NULL && handle != NULL)
        old_fread =
            (size_t(*)(void*, size_t, size_t, FILE*))dlsym(handle, "fread");

    size_t number_of_items{old_fread(ptr, size, nmemb, stream)};
    std::cerr << "fread(\"" << ptr << "\", " << size << ", " << nmemb
              << "\", \"" << stream << "\") = " << number_of_items << std::endl;

    return number_of_items;
}

size_t fwrite(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    if (old_fwrite == NULL && handle != NULL)
        old_fwrite =
            (size_t(*)(void*, size_t, size_t, FILE*))dlsym(handle, "fwrite");

    size_t number_of_items{old_fwrite(ptr, size, nmemb, stream)};
    std::cerr << "fwrite(\"" << ptr << "\", " << size << ", " << nmemb
              << "\", \"" << stream << "\") = " << number_of_items << std::endl;

    return number_of_items;
}

int open(const char* pathname, int flags) {
    if (old_open == NULL && handle != NULL)
        old_open = (int (*)(const char*, int))dlsym(handle, "open");

    int fd{old_open(pathname, flags)};
    std::cerr << "open(\"" << pathname << "\", " << flags << ") = " << fd
              << std::endl;

    return fd;
}

int open(const char* pathname, int flags, mode_t mode) {
    if (old_open_with_mode == NULL && handle != NULL)
        old_open_with_mode =
            (int (*)(const char*, int, mode_t))dlsym(handle, "open");

    int fd{old_open_with_mode(pathname, flags, mode)};
    std::cerr << "open(\"" << pathname << "\", " << flags << ", " << mode
              << ") = " << fd << std::endl;

    return fd;
}

ssize_t read(int fd, void* buf, size_t count) {
    if (old_read == NULL && handle != NULL)
        old_read = (ssize_t(*)(int, void*, size_t))dlsym(handle, "read");

    ssize_t number_of_bytes{old_read(fd, buf, count)};
    std::cerr << "read(\"" << fd << "\", \"" << buf << "\", " << count
              << ") = " << number_of_bytes << std::endl;

    return number_of_bytes;
}

int remove(const char* pathname) {
    if (old_remove == NULL && handle != NULL)
        old_remove = (int (*)(const char*))dlsym(handle, "remove");

    int status{old_remove(pathname)};
    std::cerr << "remove(\"" << pathname << "\") = " << status << std::endl;

    return status;
}

int rename(const char* oldpath, const char* newpath) {
    if (old_rename == NULL && handle != NULL)
        old_rename = (int (*)(const char*, const char*))dlsym(handle, "rename");

    int status{old_rename(oldpath, newpath)};
    std::cerr << "remove(\"" << oldpath << "\", \"" << newpath
              << "\") = " << status << std::endl;

    return status;
}

FILE* tmpfile(void) {
    if (old_tmpfile == NULL && handle != NULL)
        old_tmpfile = (FILE * (*)(void)) dlsym(handle, "tmpfile");

    FILE* stream{old_tmpfile()};
    std::cerr << "tmpfile() = " << stream << std::endl;

    return stream;
}

ssize_t write(int fd, const void* buf, size_t count) {
    if (old_write == NULL && handle != NULL)
        old_write =
            (ssize_t(*)(int, const void*, size_t))dlsym(handle, "write");

    ssize_t number_of_bytes{old_write(fd, buf, count)};
    std::cerr << "write(\"" << fd << "\", \"" << buf << "\", " << count
              << ") = " << number_of_bytes << std::endl;

    return number_of_bytes;
}