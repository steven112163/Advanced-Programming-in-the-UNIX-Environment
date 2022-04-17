#include <linux_cmd.hpp>

int linux_chmod(const char* pathname, mode_t mode) {
    static int (*__chmod)(const char*, mode_t) = NULL;
    if (__chmod == NULL)
        __chmod = (int (*)(const char*, mode_t))dlsym(RTLD_NEXT, "chmod");

    return __chmod(pathname, mode);
}

int linux_chown(const char* pathname, uid_t owner, gid_t group) {
    static int (*__chown)(const char*, uid_t, gid_t) = NULL;
    if (__chown == NULL)
        __chown = (int (*)(const char*, uid_t, gid_t))dlsym(RTLD_NEXT, "chown");

    return __chown(pathname, owner, group);
}

int linux_close(int fd) {
    static int (*__close)(int) = NULL;
    if (__close == NULL) __close = (int (*)(int))dlsym(RTLD_NEXT, "close");

    return __close(fd);
}

int linux_creat(const char* pathname, mode_t mode) {
    static int (*__creat)(const char*, mode_t) = NULL;
    if (__creat == NULL)
        __creat = (int (*)(const char*, mode_t))dlsym(RTLD_NEXT, "creat");

    return __creat(pathname, mode);
}

int linux_fclose(FILE* stream) {
    static int (*__fclose)(FILE*) = NULL;
    if (__fclose == NULL) __fclose = (int (*)(FILE*))dlsym(RTLD_NEXT, "fclose");

    return __fclose(stream);
}

FILE* linux_fopen(const char* pathname, const char* mode) {
    static FILE* (*__fopen)(const char*, const char*) = NULL;
    if (__fopen == NULL)
        __fopen =
            (FILE * (*)(const char*, const char*)) dlsym(RTLD_NEXT, "fopen");

    return __fopen(pathname, mode);
}

size_t linux_fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    static size_t (*__fread)(void*, size_t, size_t, FILE*) = NULL;
    if (__fread == NULL)
        __fread =
            (size_t(*)(void*, size_t, size_t, FILE*))dlsym(RTLD_NEXT, "fread");

    return __fread(ptr, size, nmemb, stream);
}

size_t linux_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    static size_t (*__fwrite)(const void*, size_t, size_t, FILE*) = NULL;
    if (__fwrite == NULL)
        __fwrite = (size_t(*)(const void*, size_t, size_t, FILE*))dlsym(
            RTLD_NEXT, "fwrite");

    return __fwrite(ptr, size, nmemb, stream);
}

int linux_open(const char* pathname, int flags, mode_t mode) {
    static int (*__open)(const char*, int, ...) = NULL;
    if (__open == NULL)
        __open = (int (*)(const char*, int, ...))dlsym(RTLD_NEXT, "open");

    return __open(pathname, flags, mode);
}

ssize_t linux_read(int fd, void* buf, size_t count) {
    static ssize_t (*__read)(int, void*, size_t) = NULL;
    if (__read == NULL)
        __read = (ssize_t(*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");

    return __read(fd, buf, count);
}

int linux_remove(const char* pathname) {
    static int (*__remove)(const char*) = NULL;
    if (__remove == NULL)
        __remove = (int (*)(const char*))dlsym(RTLD_NEXT, "remove");

    return __remove(pathname);
}

int linux_rename(const char* oldpath, const char* newpath) {
    static int (*__rename)(const char*, const char*) = NULL;
    if (__rename == NULL)
        __rename =
            (int (*)(const char*, const char*))dlsym(RTLD_NEXT, "rename");

    return __rename(oldpath, newpath);
}

FILE* linux_tmpfile(void) {
    static FILE* (*__tmpfile)(void) = NULL;
    if (__tmpfile == NULL)
        __tmpfile = (FILE * (*)(void)) dlsym(RTLD_NEXT, "tmpfile");

    return __tmpfile();
}

ssize_t linux_write(int fd, const void* buf, size_t count) {
    static ssize_t (*__write)(int, const void*, size_t) = NULL;
    if (__write == NULL)
        __write =
            (ssize_t(*)(int, const void*, size_t))dlsym(RTLD_NEXT, "write");

    return __write(fd, buf, count);
}