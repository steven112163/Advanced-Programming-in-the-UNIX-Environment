#include <cstdarg>
#include <iomanip>
#include <iostream>
#include <linux_cmd.hpp>
#include <sstream>
#include <string>
#include <util.hpp>

int chmod(const char* pathname, mode_t mode);
int chown(const char* pathname, uid_t owner, gid_t group);
int close(int fd);
int creat(const char* path, mode_t mode);
int fclose(FILE* stream);
FILE* fopen(const char* pathname, const char* mode);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(void* ptr, size_t size, size_t nmemb, FILE* stream);
int open(const char* pathname, int flags, ...);
ssize_t read(int fd, void* buf, size_t count);
int remove(const char* pathname);
int rename(const char* oldpath, const char* newpath);
FILE* tmpfile(void);
ssize_t write(int fd, const void* buf, size_t count);