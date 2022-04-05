#include <dlfcn.h>

#include <iostream>
#include <string>

static void* handle{dlopen("libc.so.6", RTLD_LAZY)};
static int (*old_chmod)(const char* pathname, mode_t mode) = NULL;
static int (*old_chown)(const char* pathname, uid_t owner, gid_t group) = NULL;
static int (*old_close)(int fd) = NULL;
static int (*old_creat)(const char* path, mode_t mode) = NULL;
static int (*old_fclose)(FILE* stream) = NULL;
static FILE* (*old_fopen)(const char* pathname, const char* mode) = NULL;
static size_t (*old_fread)(void* ptr, size_t size, size_t nmemb,
                           FILE* stream) = NULL;
static size_t (*old_fwrite)(void* ptr, size_t size, size_t nmemb,
                            FILE* stream) = NULL;
static int (*old_open)(const char* pathname, int flags) = NULL;
static int (*old_open_with_mode)(const char* pathname, int flags,
                                 mode_t mode) = NULL;
static ssize_t (*old_read)(int fd, void* buf, size_t count) = NULL;
static int (*old_remove)(const char* pathname) = NULL;
static int (*old_rename)(const char* oldpath, const char* newpath) = NULL;
static FILE* (*old_tmpfile)(void) = NULL;
static ssize_t (*old_write)(int fd, const void* buf, size_t count) = NULL;

int chmod(const char* pathname, mode_t mode);
int chown(const char* pathname, uid_t owner, gid_t group);
int close(int fd);
int creat(const char* path, mode_t mode);
int fclose(FILE* stream);
FILE* fopen(const char* pathname, const char* mode);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(void* ptr, size_t size, size_t nmemb, FILE* stream);
int open(const char* pathname, int flags);
int open(const char* pathname, int flags, mode_t mode);
ssize_t read(int fd, void* buf, size_t count);
int remove(const char* pathname);
int rename(const char* oldpath, const char* newpath);
FILE* tmpfile(void);
ssize_t write(int fd, const void* buf, size_t count);